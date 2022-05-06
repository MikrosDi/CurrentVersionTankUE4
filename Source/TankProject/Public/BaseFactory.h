// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTarget.h"
#include "EnemyPawn.h"
#include "HealthComponent.h"
#include "LevelTrigger.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "DestroyFabric.h"
#include "BaseFactory.generated.h"


UCLASS()
class TANKPROJECT_API ABaseFactory : public AActor, public IDamageTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseFactory();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* SpawnEnemyPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Factory")
	TArray<ATargetPoint*> Waypoints;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Factory")
	TSubclassOf<AEnemyPawn> TankClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Factory")
	int MaxSpawnTanks = 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Factory")
	float SpawnInterwal = 20;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Factory")
	ALevelTrigger* LevelTrigger;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeadEffect;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioDeadEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* CreateEffect;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioCreateEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fabric")
	TSubclassOf<ADestroyFabric> DestroyFabricClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* DestroyFabricSpawnPoint;

	
	virtual void TakeDamage(FDamageData Damage) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	void OnDeath();
	void OnHealthChanged(float CurrentHealth);
	void OnTankSpawnTick();
	void SelfDestroed();

	
	int TankSpawned = 0;
	FTimerHandle TimerHandle;
	FTimerHandle TimerDestroed;

	UPROPERTY()
	class ADestroyFabric* DestroyFabric;
};
