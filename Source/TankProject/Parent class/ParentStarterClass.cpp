// Fill out your copyright notice in the Description page of Project Settings.

#include "ParentStarterClass.h"
#include "TankPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AParentStarterClass::AParentStarterClass()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh -> SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMesh -> SetupAttachment(BodyMesh);

	CannonPosition = CreateDefaultSubobject<UArrowComponent>("CannonPosition");
	CannonPosition->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	
}

void AParentStarterClass::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
		HealthComponent->TakeDamage(Damage);
}

void AParentStarterClass::ReceivePoints(int InPoints)
{
	Points += InPoints;
	OnPointsReceived(InPoints);
}


void AParentStarterClass::BeginPlay()
{
	Super::BeginPlay();
}

void AParentStarterClass::Destroyed()
{
	Super::Destroyed();
}

void AParentStarterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParentStarterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AParentStarterClass::OnDeath()
{
	//DestroyEffect->ActivateSystem();
	//AudioDestroyEffect->Play();
}

