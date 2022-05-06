// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "EnemyPawn.h"
#include "TargetController.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	virtual void SetupInputComponent() override;

	void OnMoveForward(float Value);
	void OnRotateRight(float Value);
	void OnRotateTurret(float Value);

	void OnSwapController();
	
	void OnShoot();

	void OnTurretShoot();
	
	void FireStart();
	void FireStop();
	
	void OnSwapWeapon();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	
	FVector MouseWorldPosition;

	virtual FVector GetShootTarget() const 
	{
			return MouseWorldPosition;
	}

private:
	
	UPROPERTY()
	ATankPawn* PlayerTank;
};






