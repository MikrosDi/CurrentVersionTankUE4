// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MortirProjectile.generated.h"

UCLASS()
class TANKPROJECT_API AMortirProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	
	AMortirProjectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* FlyEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* BoomEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlastRegion")
	bool BlastRegionActiveOrDeactive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
	int SelfDestroedSec = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Damage = 20;
	
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	 bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void BeginPlay() override;
	
	FTimerHandle TimerDestroed;
	void SelfDestroed();
};
