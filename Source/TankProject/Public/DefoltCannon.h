// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "DefoltCannon.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKPROJECT_API ADefoltCannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	
ADefoltCannon();
	
	void FireStart();
	void FireStop();
	int Ammo = 40;
	void UploadAmmo(int ShellCount);

protected:

	virtual void DoFire();

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category= "Fire parans")
	TSubclassOf<ADefoltCannon>DefoltCannonType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	
	UPROPERTY(EditAnywhere)
	float ReloadTime = 0.3f;

	FTimerHandle FireTimerHandle;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	bool bIsFiring = false;

};
