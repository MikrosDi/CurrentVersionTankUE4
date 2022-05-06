// Fill out your copyright notice in the Description page of Project Settings.

#include "MortireTurret.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "FrendlyTurret.h"
#include "MortirProjectile.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMortireTurret::AMortireTurret(): AParentStarterClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RangeSphere = CreateDefaultSubobject<USphereComponent>("RangeSphere");
	RangeSphere -> SetupAttachment(RootComponent);
	RangeSphere -> OnComponentBeginOverlap.AddDynamic(this, &AMortireTurret::OnBeginOverlap);
	RangeSphere -> OnComponentEndOverlap.AddDynamic(this, &AMortireTurret::OnEndOverlap);
	
	HealthComponent->OnDeath.AddUObject(this, &AMortireTurret::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMortireTurret::OnHealthChanged);

	DeadEffect2 = CreateDefaultSubobject<UParticleSystemComponent>("DeadEffect");
	DeadEffect2->SetupAttachment(BodyMesh);
	
	AudioDeadEffect2 = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioDeadEffect2->SetupAttachment(BodyMesh);


}

// Called when the game starts or when spawned
void AMortireTurret::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMortireTurret::Targeting, TargetingRate,true);
	
	if(CannonClass)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		TurretCannon = GetWorld()->SpawnActor<ATurretCannon>(CannonClass, Transform);
		TurretCannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		TurretCannon->SetInstigator(this);
		
	}
	TurretPosition = GetActorLocation();
}

void AMortireTurret::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
	   HealthComponent->TakeDamage(Damage);
}

int AMortireTurret::GetPoint() const
{
	return ScorePoints;
}

void AMortireTurret::Destroyed()
{
	Super::Destroyed();

	if(TurretCannon)
	{
		TurretCannon->Destroy();
	}
}

void AMortireTurret::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                             UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	OtherActors.Add(Actor);
	
	if(!Target.IsValid())
	{
		
		FindNextTarget();
	}
}

void AMortireTurret::OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* OtherComponent, int I)
{
	OtherActors.Remove(Actor);
	if(Actor == Target)
	{
		FindNextTarget();
	}
}

void AMortireTurret::FireAnyway()
{
	if(TurretCannon)
	{
		TurretCannon->TurretShoot(AmmoII);
	}
}

void AMortireTurret::FindNextTarget()
{
	float MinRange = 100000;
	Target = nullptr;
	
	for(auto Actor : OtherActors)
	{
		float Range = FVector::Distance(Actor->GetActorLocation(),GetActorLocation());
		if(Range < MinRange)
		{
			auto FrendlyTurret = Cast<AFrendlyTurret>(Actor);
			MinRange=Range;
			Target = Actor, FrendlyTurret;
		}
		DrawDebugLine(GetWorld(),TurretMesh->GetComponentLocation(),Target->GetActorLocation(),FColor::Green,false,1);
	}
}

void AMortireTurret::Fire()
{
	if(!Target.IsValid())
		return;
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(EnemyPawn);
	Params.AddIgnoredActor(TurretCannon);
	Params.bTraceComplex = true;
		if(GetWorld()->LineTraceSingleByChannel(Result,TurretMesh->GetComponentLocation(),Target->GetActorLocation(),ECollisionChannel::ECC_Visibility,Params))
	{
		
		if(Result.Actor == Target.Get())
		{
			
			if(TurretCannon)
			{
			//	GEngine->AddOnScreenDebugMessage(1241115, 1, FColor::Purple,FString::Printf(TEXT("BAG Baag")));
			TurretCannon->TurretShoot(AmmoII);
			}
		}
	}
}

void AMortireTurret::Targeting()
{
		if(!Target.IsValid())
		{
			FindNextTarget();
			if(!Target.IsValid())
			{
				return;
			}
		}
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(),Target->GetActorLocation());
	
	if (!TargetZ)
	{
		auto TTarget = Cast<ATankPawn>(Target);
		TargetZ = TTarget->BodyMesh->GetComponentLocation().Z;
	}
	auto TargetPosition = Target->GetActorLocation();
	auto x = FVector::Dist2D(TurretPosition, TargetPosition) / 125;

	float P = FMath::RadiansToDegrees(FMath::Atan(1/FMath::Tan(S2 + FMath::Sqrt(S4 - G * (G * FMath::Pow(x, 2) + 2 * S2 * (0.5))) / (G * x))));
	if (x > 4.5f)
	{
		P = 45.0f;
	}
	float RotateY = TargetRotation.Yaw;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), { P, RotateY, 0.f }, TargetingSpeed));

	FRotator t = { P, RotateY, 0.f };
	
	     TargetRotation.Yaw = TurretMesh->GetComponentRotation().Yaw;
	     TargetRotation.Roll = TurretMesh->GetComponentRotation().Roll;
	     TargetRotation.Pitch = TurretMesh->GetComponentRotation().Pitch-30;
	
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), TargetRotation, TargetingSpeed));
		auto TargetingDirection = TurretMesh->GetForwardVector();
		auto PlayerDirection =Target->GetActorLocation() - GetActorLocation();
		PlayerDirection.Normalize();
		auto Angle=FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection,PlayerDirection)));
		if(Angle < AimSlack)
		{
			Fire();
		}
}

// Called every frame
void AMortireTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ProcessTargeting(DeltaTime);
}

void AMortireTurret::OnHealthChanged(float CurrentHealth)
{
	//GEngine->AddOnScreenDebugMessage(987665, 1, FColor::White,
	//	FString::Printf(TEXT("Enemy Health: %f"), CurrentHealth));
}

void AMortireTurret::OnDeath()
{
	auto Temp = GetActorLocation();
	DeadEffect2->ActivateSystem();
	AudioDeadEffect2->Play();
	SetActorLocation({-1000, -1000, -1000});
	DeadEffect2->SetWorldLocation(Temp);
	AudioDeadEffect2->SetWorldLocation(Temp);
	GetWorld()->GetTimerManager().SetTimer(TimerDestroed,this,&AMortireTurret::SelfDestroed,3,false);
	
	//GEngine->AddOnScreenDebugMessage(-123124, 1, FColor::White,
	//	FString::Printf(TEXT("Enemy Turret Destroed")));
}

void AMortireTurret::SelfDestroed()
{
	Destroy();
}