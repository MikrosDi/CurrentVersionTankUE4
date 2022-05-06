// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"
#include "EnemyAIController.h"
#include "TurretCannon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include "Turret.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyPawn::AEnemyPawn():AParentStarterClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangeSphere = CreateDefaultSubobject<USphereComponent>("RangeSphere");
	RangeSphere -> SetupAttachment(RootComponent);
	RangeSphere -> OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn::OnBeginOverlap);
	RangeSphere -> OnComponentEndOverlap.AddDynamic(this, &AEnemyPawn::OnEndOverlap);
	
	HealthComponent->OnHealthChanged.AddUObject(this, &AEnemyPawn::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &AEnemyPawn::OnDeath);
	
	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>("DeadEffect");
	DeadEffect->SetupAttachment(BodyMesh);
	
	AudioDeadEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioDeadEffect->SetupAttachment(BodyMesh);

	

}

void AEnemyPawn::MoveForward(float Scale)
{
	MoveScaleTarget = Scale;
}

void AEnemyPawn::RotateRight(float Scale)
{
	RotationScaleTarget = Scale;
}


void AEnemyPawn::TurretShoot()
{
	
	if( TurretCannon != nullptr )
	{
		TurretCannon->TurretShoot(AmmoII);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(3331, 1.5,FColor::Red,FString(TEXT("NO BULLET")));
	}
	
}

void AEnemyPawn::TurretSetupCannon(const TSubclassOf<ATurretCannon>& CannonClass)
{
	TurretCannonType = CannonClass;
	if(TurretCannonType)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		TurretCannon = GetWorld()->SpawnActor<ATurretCannon>(TurretCannonType, Transform);
		TurretCannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		TurretCannon->SetInstigator(this);
	}
}

void AEnemyPawn::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
		HealthComponent->TakeDamage(Damage);
}

void AEnemyPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black,FString::Printf(TEXT("EnemyTank created %d"),Number));
	
}

int AEnemyPawn::GetPoint() const
{
	
	return ScorePoints;
	
}

void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyAITankController = Cast<AEnemyAIController>(NewController);
	TargetController = Cast<ITargetController>(NewController);
}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TurretSetupCannon(TurretCannonType);
	TurretCannon->SetInstigator(this);
}

void AEnemyPawn::Destroyed()
{
	if(TurretCannon)
	{
		TurretCannon->Destroy();
	}
	
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTank(DeltaTime);
	RotateTank(DeltaTime);
	// RotateCannon(DeltaTime);
}

void AEnemyPawn::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
							 UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	auto AIController = Cast<AEnemyAIController>(GetController());
	if(AIController)
		AIController->AddTarget(Actor);
	
}

void AEnemyPawn::OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* OtherComponent, int I)
{
	auto AIController = Cast<AEnemyAIController>(GetController());
	if(AIController)
	AIController->RemoveTarget(Actor);
}



void AEnemyPawn::MoveTank(float DeltaTime)
{
	MoveScaleCurrent = FMath::Lerp(MoveScaleCurrent, MoveScaleTarget,MovementAcceleration);
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector() * MoveScaleCurrent * MovementSpeed * DeltaTime, false);
//	GEngine->AddOnScreenDebugMessage(-666, -5, FColor::Turquoise, FString::Printf(TEXT("Speed - %f"), MoveScaleCurrent));
}

void AEnemyPawn::RotateTank(float DeltaTime)
{
	RotationScaleCurrent = FMath::Lerp(RotationScaleCurrent, RotationScaleTarget, RotationAcceleration);
	auto Rotation = GetActorRotation();
	Rotation.Yaw += RotationScaleCurrent * RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);
	//UE_LOG(LogTankProject, Warning, TEXT("%s"), *Rotation.ToString())
}

void AEnemyPawn::RotateCannon(float DeltaTime)
{
	if(!TargetController)
		return;
	//auto TC = reinterpret_cast<ITargetController*>(TargetController);
	auto BodyMeshRotation = BodyMesh->GetComponentRotation();
	auto OldRotation = TurretMesh->GetComponentRotation();
	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(),TargetController->GetShootTarget());
	OldRotation.Yaw = TurretRotation.Yaw;
	OldRotation.Pitch = BodyMeshRotation.Pitch;
	OldRotation.Roll = BodyMeshRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(),OldRotation,RotationAcceleration));
}


void AEnemyPawn::OnDeath()
{
	auto Temp = GetActorLocation();
	
	DeadEffect->ActivateSystem();
	AudioDeadEffect->Play();
	SetActorLocation({-1000, -1000, -1000});
	DeadEffect->SetWorldLocation(Temp);
	AudioDeadEffect->SetWorldLocation(Temp);

	GetWorld()->GetTimerManager().SetTimer(TimerDestroed,this,&AEnemyPawn::SelfDestroed,3,false);
	
	//GEngine->AddOnScreenDebugMessage(9871, 3, FColor::White,FString::Printf(TEXT("Enemy Tank Destroyed")));
	//UKismetSystemLibrary::QuitGame(GetWorld(),GetWorld()->GetNumPlayerControllers(),EQuitPreference::Quit,true);
}

void AEnemyPawn::OnHealthChanged(float CurrentHealth)
{
	//GEngine->AddOnScreenDebugMessage(9871662, 3, FColor::White,FString::Printf(TEXT("Health Enemy Tank: %f"), CurrentHealth));
}

void AEnemyPawn::SelfDestroed()
{
	Destroy();
}

