// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretCannon.h"

#include "DamageTarget.h"
#include "DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ATurretCannon::ATurretCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioEffect->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>("ShootEffect");
	ShootEffect->SetupAttachment(Mesh);
	
}

void ATurretCannon::TurretShoot(int& AmmoII)
    {
		if(!bReadyToShootTurret)
			return;
	
		switch (Type1)
		{
		case ETurretCannonType::FireProjectile1:
			{
				GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red,FString(TEXT("")));
				auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,ProjectileSpawnPoint->GetComponentLocation(),ProjectileSpawnPoint->GetComponentRotation());
				Projectile->SetInstigator(GetInstigator());
				ShootEffect->ActivateSystem();
				AudioEffect->Play();
				//GEngine->AddOnScreenDebugMessage(-1234, 1, FColor::Blue, FString::Printf(TEXT("AmmoII - %i"), AmmoII));
				break;
			}
	
		case ETurretCannonType::FireTrace1:
			{
				GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green,FString(TEXT("")));
				ShootEffect->ActivateSystem();
				AudioEffect->Play();
				FHitResult Result;
				FCollisionObjectQueryParams Params;
				FCollisionQueryParams Params1;
				Params1.AddIgnoredActor(this);
				Params1.AddIgnoredComponent(ProjectileSpawnPoint);
				Params.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
				Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		
				auto Start = ProjectileSpawnPoint->GetComponentLocation();
				auto End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange1;
		
				bool HasHit = GetWorld()->LineTraceSingleByObjectType(
					Result, Start, End, Params);
		
				if(HasHit)
				{
					End = Result.Location;
					if(Result.Actor.IsValid())
					{
						auto Target = Cast<IDamageTarget>(Result.Actor);
						if(Target)
						{
							FDamageData DamageData;
							DamageData.Instigator = GetInstigator();
							DamageData.DamageValue = LaserDamage1;
							Target->TakeDamage(DamageData);
						}
					}
				}
				DrawDebugLine(GetWorld(),Start, End, FColor::Purple,false, 0.1, 0, 13);
				break;
			}
		}
	
			bReadyToShootTurret = false;
	        GetWorld()->GetTimerManager().SetTimer(TurretTimerHandle, FTimerDelegate::CreateUObject(this, &ATurretCannon::ResetShootStateTurret), 1 / FireRate1, false);
    }


void ATurretCannon::ResetShootStateTurret()
{
	
	bReadyToShootTurret = true;
}

// Called when the game starts or when spawned
void ATurretCannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



