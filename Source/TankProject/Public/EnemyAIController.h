// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "EnemyPawn.h"
#include "TargetController.h"
#include "TurretCannon.h"
#include "Engine/TargetPoint.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

UCLASS()
class TANKPROJECT_API AEnemyAIController : public AAIController, public ITargetController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Move")
	FName WaypointTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Move", Meta=(MakeEditWidget=true))
	TArray<FVector>PatrollingPoints;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI|Move")
	float MovementAccuracy = 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float TargetingSpeed = 0.1f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float TargetingRate = 0.005f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float AimSlack = 5;


	
	
	void AddTarget(AActor* Target);
	
	void RemoveTarget(AActor* Target);
	
	int AmmoII = 100;

	//AEnemyAIController();
	
	virtual FVector GetShootTarget() const override;
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	void Fire();

	void FindNextTarget();
	
private:
	int32 CurrentPatrolPointIndex= 0;
	
	
	TWeakObjectPtr<AActor>Target;
	
	UPROPERTY()
	TArray<AActor*> OtherActors;
	
	UPROPERTY()
	class AEnemyPawn* EnemyPawn;
	

}; 	
