
#include "FrendlyTurret.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFrendlyTurret::AFrendlyTurret(): AParentStarterClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RangeSphere = CreateDefaultSubobject<USphereComponent>("RangeSphere");
	RangeSphere -> SetupAttachment(RootComponent);
	RangeSphere -> OnComponentBeginOverlap.AddDynamic(this, &AFrendlyTurret::OnBeginOverlap);
	RangeSphere -> OnComponentEndOverlap.AddDynamic(this, &AFrendlyTurret::OnEndOverlap);
	
	HealthComponent->OnDeath.AddUObject(this, &AFrendlyTurret::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AFrendlyTurret::OnHealthChanged);

	DeadEffect2 = CreateDefaultSubobject<UParticleSystemComponent>("DeadEffect");
	DeadEffect2->SetupAttachment(BodyMesh);
	
	AudioDeadEffect2 = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioDeadEffect2->SetupAttachment(BodyMesh);
}

// Called when the game starts or when spawned
void AFrendlyTurret::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AFrendlyTurret::Targeting, TargetingRate,true);
	
	if(CannonClass)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		TurretCannon = GetWorld()->SpawnActor<ATurretCannon>(CannonClass, Transform);
		TurretCannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		TurretCannon->SetInstigator(this);
	}
	if(CannonClass1)
	{
		auto Transform1 = CannonPosition->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass1, Transform1);
		Cannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		Cannon->SetInstigator(this);
	}
}

void AFrendlyTurret::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
	   HealthComponent->TakeDamage(Damage);
}

int AFrendlyTurret::GetPoint() const
{
	return ScorePoints;
}

void AFrendlyTurret::Destroyed()
{
	Super::Destroyed();

	if(TurretCannon)
	{
		TurretCannon->Destroy();
	}
	if(Cannon)
	{
		Cannon->Destroy();
	}
}

void AFrendlyTurret::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                             UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	OtherActors.Add(Actor);
	
	if(!Target.IsValid())
	{
		
		FindNextTarget();
	}
}

void AFrendlyTurret::OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* OtherComponent, int I)
{
	OtherActors.Remove(Actor);
	if(Actor == Target)
	{
		FindNextTarget();
	}
}

void AFrendlyTurret::FindNextTarget()
{
	float MinRange = 100000;
	Target = nullptr;
	
	for(auto Actor : OtherActors)
	{
		float Range = FVector::Distance(Actor->GetActorLocation(),GetActorLocation());
		if(Range < MinRange)
		{
			auto EnemyPawn = Cast<AEnemyPawn>(Actor);
			auto Turret = Cast<ATurret>(Actor);
			MinRange=Range;
			Target = Actor,EnemyPawn, Turret;
		}
		DrawDebugLine(GetWorld(),TurretMesh->GetComponentLocation(),Target->GetActorLocation(),FColor::Green,false,1);
	}
}

void AFrendlyTurret::Fire()
{
	if(!Target.IsValid())
		return;
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(FrendlyTurret);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(TankPawn);
	Params.AddIgnoredActor(TurretCannon);
	Params.AddIgnoredActor(Cannon);
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
			if(Cannon)
			{
				Cannon->Shoot(AmmoII);
			}
		}
	}
}

void AFrendlyTurret::Targeting()
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
void AFrendlyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ProcessTargeting(DeltaTime);
}

void AFrendlyTurret::OnHealthChanged(float CurrentHealth)
{
	GEngine->AddOnScreenDebugMessage(987665, 1, FColor::White,
		FString::Printf(TEXT("Frendly Health: %f"), CurrentHealth));
}

void AFrendlyTurret::OnDeath()
{
	auto Temp = GetActorLocation();
	
	DeadEffect2->ActivateSystem();
	AudioDeadEffect2->Play();
	SetActorLocation({-1000, -1000, -1000});
	DeadEffect2->SetWorldLocation(Temp);
	AudioDeadEffect2->SetWorldLocation(Temp);

	GetWorld()->GetTimerManager().SetTimer(TimerDestroed,this,&AFrendlyTurret::SelfDestroed,3,false);
	
	GEngine->AddOnScreenDebugMessage(-123126, 1, FColor::White,
		FString::Printf(TEXT("Frendly Turret Destroed")));
}

void AFrendlyTurret::SelfDestroed()
{
	Destroy();
}