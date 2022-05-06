// Fill out your copyright notice in the Description page of Project Settings.

#include "DefoltCannon.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADefoltCannon::ADefoltCannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	    RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	    SetRootComponent(RootComponent);

	    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	    Mesh->SetupAttachment(RootComponent);

	    ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
    	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioEffect->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>("ShootEffect");
	ShootEffect->SetupAttachment(Mesh);
}

void ADefoltCannon::FireStart()
{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red,TEXT(""));
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle,this,&ADefoltCannon::DoFire,ReloadTime,true,0.f);
}

void ADefoltCannon::FireStop()
{
		GEngine->AddOnScreenDebugMessage(-1, 1.5,FColor::Red,TEXT(""));
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ADefoltCannon::DoFire()
{
	if(Ammo > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red,TEXT(""));
		auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,ProjectileSpawnPoint->GetComponentLocation(),ProjectileSpawnPoint->GetComponentRotation());
		Projectile->SetInstigator(GetInstigator());
		ShootEffect->ActivateSystem();
		AudioEffect->Play();
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-9926, 1, FColor::Blue, FString::Printf(TEXT("Ammo %i"), Ammo));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(333, 1.5,FColor::Red,FString(TEXT("NO BULLET")));
	}
}

void ADefoltCannon::UploadAmmo(const int ShellCount)
{
	Ammo += ShellCount;
	GEngine->AddOnScreenDebugMessage(-9926, 1, FColor::Blue, FString::Printf(TEXT("Ammo %i"), Ammo));
}

// Called when the game starts or when spawned
void ADefoltCannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADefoltCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

