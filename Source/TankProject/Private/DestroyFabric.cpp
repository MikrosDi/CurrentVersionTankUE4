// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyFabric.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADestroyFabric::ADestroyFabric()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	BuildingMesh->SetupAttachment(RootComponent);

	CreateEffect = CreateDefaultSubobject<UParticleSystemComponent>("CreateEffect");
	CreateEffect->SetupAttachment(BuildingMesh);


}

// Called when the game starts or when spawned
void ADestroyFabric::BeginPlay()
{
	Super::BeginPlay();
	CreateEffect->ActivateSystem();
}

// Called every frame
void ADestroyFabric::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

