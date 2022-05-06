// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "Cannon.h"
#include "TurretCannon.h"
#include "DamageTarget.h"
#include "DefoltCannon.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankProject/Parent class/ParentStarterClass.h"
#include "TankPawn.generated.h"


class ACannon;
UCLASS(Blueprintable)
class TANKPROJECT_API ATankPawn : public AParentStarterClass
{
	GENERATED_BODY()
	

	
public:
	// Sets default values for this pawn's properties
	ATankPawn();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* ArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Fire parans")
	TSubclassOf<ACannon>CannonType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Fire parans")
	TSubclassOf<ADefoltCannon>DefoltCannonType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Fire parans")
	TSubclassOf<ATurretCannon>TurretCannonType;
	
	UPROPERTY()
	ACannon* Cannon;
	
	UPROPERTY()
	ADefoltCannon* DefoltCannon;

	UPROPERTY()
	ATurretCannon* TurretCannon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementAcceleration = 0.06;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed =50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationTurretSpeed = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationAcceleration = 0.06;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurretAcceleration = 1;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* TakeHitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioTakeHitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeadEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuitGame")
	int QuitGameSec = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* GameOverWidget;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	//TSubclassOf<UUserWidget> BackToGameMenu;
	
	FString PlayerName = "Mikros";
	
	int SwapWep = 0;

	bool SwapController = false;
	
	void MoveForward(float Scale);
	void RotateRight(float Scale);
	void RotateTurretState(float Scale);
	
	void RotateCannon(float DeltaTime);
	void RotateTurret(float DeltaTime);
	
	void Shoot();
	void SetupCannon(const TSubclassOf<ACannon>& CannonClass);

	void TurretShoot();
	void TurretSetupCannon(const TSubclassOf<ATurretCannon>& CannonClass);
	
	void FireStart();
	void FireStop();
	void SetupDefoltCannon(const TSubclassOf<ADefoltCannon>& CannonClass);

	virtual void TakeDamage(FDamageData Damage) override;

	void SwapControllerTurret();
	
	void Swap();
	
	void UploadAmmo(const int ShellCount);

	virtual void OnPointsReceived(int InPoints) override;

protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
private:

	FTimerHandle QuitGame;
	void QuitGameStopSimulation();
	void MoveTank(float DeltaTime);
    void RotateTank(float DeltaTime);
	
	void OnDeath();
	void OnHealthChanged(float CurrentHealth);
	
	float MoveScaleTarget = 0;
	float MoveScaleCurrent = 0;
	
	float RotationScaleTarget = 0;
	float RotationScaleCurrent = 0;
	
	float RotationScaleTurretTarget = 0;
	float RotationScaleTurretCurrent = 0;
	
	int AmmoBB = 10;
	int AmmoII = 9999;

	UPROPERTY()
	class ATankPlayerController* TankController;

};