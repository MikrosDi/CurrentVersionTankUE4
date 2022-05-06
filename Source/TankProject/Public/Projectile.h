// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "Projectile.generated.h"



UCLASS()
class TANKPROJECT_API AProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float MoveSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float MoveRate = 0.005;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
	int SelfDestroedSec = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Damage = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlastRegion")
	bool BlastRegionActiveOrDeactive = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle Timer;
	FTimerHandle TimerDestroed;
	
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	 bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	void MoveTick();
	void SelfDestroed();
};