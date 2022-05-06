// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ATurret::ATurret(): AParentStarterClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RangeSphere = CreateDefaultSubobject<USphereComponent>("RangeSphere");
	RangeSphere -> SetupAttachment(RootComponent);
	RangeSphere -> OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnBeginOverlap);
	RangeSphere -> OnComponentEndOverlap.AddDynamic(this, &ATurret::OnEndOverlap);
	
	HealthComponent->OnDeath.AddUObject(this, &ATurret::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::OnHealthChanged);

	DeadEffect2 = CreateDefaultSubobject<UParticleSystemComponent>("DeadEffect");
	DeadEffect2->SetupAttachment(BodyMesh);
	
	AudioDeadEffect2 = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioDeadEffect2->SetupAttachment(BodyMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ATurret::Targeting, TargetingRate,true);
	
	if(CannonClass)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		TurretCannon = GetWorld()->SpawnActor<ATurretCannon>(CannonClass, Transform);
		TurretCannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		TurretCannon->SetInstigator(this);
	}
}

void ATurret::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
	   HealthComponent->TakeDamage(Damage);
}

int ATurret::GetPoint() const
{
	return ScorePoints;
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	if(TurretCannon)
	{
		TurretCannon->Destroy();
	}
}

void ATurret::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                             UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	OtherActors.Add(Actor);
	
	if(!Target.IsValid())
	{
		
		FindNextTarget();
	}
}

void ATurret::OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* OtherComponent, int I)
{
	OtherActors.Remove(Actor);
	if(Actor == Target)
	{
		FindNextTarget();
	}
}

void ATurret::FindNextTarget()
{
	float MinRange = 100000;
	Target = nullptr;
	
	for(auto Actor : OtherActors)
	{
		float Range = FVector::Distance(Actor->GetActorLocation(),GetActorLocation());
		if(Range < MinRange)
		{
			MinRange=Range;
			Target = Actor;
		}
		DrawDebugLine(GetWorld(),TurretMesh->GetComponentLocation(),Target->GetActorLocation(),FColor::Green,false,1);
	}
}

void ATurret::Fire()
{
	if(!Target.IsValid())
		return;
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
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

void ATurret::Targeting()
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
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ProcessTargeting(DeltaTime);
}

void ATurret::OnHealthChanged(float CurrentHealth)
{
	GEngine->AddOnScreenDebugMessage(987665, 1, FColor::White,
		FString::Printf(TEXT("Enemy Health: %f"), CurrentHealth));
}

void ATurret::OnDeath()
{
	auto Temp = GetActorLocation();
	
	DeadEffect2->ActivateSystem();
	AudioDeadEffect2->Play();
	SetActorLocation({-1000, -1000, -1000});
	DeadEffect2->SetWorldLocation(Temp);
	AudioDeadEffect2->SetWorldLocation(Temp);

	GetWorld()->GetTimerManager().SetTimer(TimerDestroed,this,&ATurret::SelfDestroed,3,false);
	
	GEngine->AddOnScreenDebugMessage(-123124, 1, FColor::White,
		FString::Printf(TEXT("Enemy Turret Destroed")));
}

void ATurret::SelfDestroed()
{
	Destroy();
}

