// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TurretCannon.generated.h"

UENUM()

enum class ETurretCannonType:uint8
{
	FireProjectile1 = 0 UMETA(DisplayName = "Use projectile"),
	
	FireTrace1 = 1 UMETA(DisplayName = "Use trace") ,
	 
};


UCLASS()
class TANKPROJECT_API ATurretCannon : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate1 = 0.4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	float LaserDamage1 = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange1 = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireType1 = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ETurretCannonType Type1 = ETurretCannonType::FireProjectile1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	
	void TurretShoot(int& AmmoII);
	
public:	
	// Sets default values for this actor's properties
	ATurretCannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetShootStateTurret();
	
	FTimerHandle TurretTimerHandle;
	bool bReadyToShootTurret = true;


	
};
