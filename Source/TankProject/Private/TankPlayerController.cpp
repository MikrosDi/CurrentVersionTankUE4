// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "EnemyPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::OnMoveForward);
	InputComponent->BindAxis("RotationRight", this, &ATankPlayerController::OnRotateRight);

	InputComponent->BindAxis("RotateTurret", this, &ATankPlayerController::OnRotateTurret);
	
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATankPlayerController::OnShoot);
	InputComponent->BindAction("Shoot", IE_Released, this, &ATankPlayerController::FireStop);
	
	InputComponent->BindAction("TurretShoot", IE_Pressed, this, &ATankPlayerController::OnTurretShoot);
	
	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed, this, &ATankPlayerController::FireStart);
	InputComponent->BindAction("Fire",EInputEvent::IE_Released, this, &ATankPlayerController::FireStop);
	
	InputComponent->BindAction("SwapController",EInputEvent::IE_Pressed, this, &ATankPlayerController::OnSwapController);
	
	
	InputComponent->BindAction("SwapWeapon", IE_Pressed, this, &ATankPlayerController::OnSwapWeapon);
	bShowMouseCursor = true;
}

void ATankPlayerController::OnMoveForward(float Value)
{
	if(PlayerTank)
		PlayerTank->MoveForward(Value);
}
void ATankPlayerController::OnRotateRight(float Value)
{
	if(PlayerTank)
		PlayerTank->RotateRight(Value);
}

void ATankPlayerController::OnRotateTurret(float Value)
{
	if(PlayerTank)
		PlayerTank->RotateTurretState(Value);
}


void ATankPlayerController::OnShoot()
{
	if(PlayerTank)
		PlayerTank->Shoot();
}

void ATankPlayerController::OnTurretShoot()
{
	if(PlayerTank)
		PlayerTank->TurretShoot();
}

void ATankPlayerController::FireStart()
{
	if(PlayerTank)
		PlayerTank->FireStart();
}

void ATankPlayerController::FireStop()
{
	if(PlayerTank)
		PlayerTank->FireStop();
}


void ATankPlayerController::OnSwapWeapon()
{
	if(PlayerTank)
		PlayerTank->Swap();
}

void ATankPlayerController::OnSwapController()
{
	if(PlayerTank)
		PlayerTank->SwapControllerTurret();
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = CastChecked<ATankPawn>(GetPawn());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition,MouseDirection);

	auto Z = FMath::Abs(PlayerTank->GetActorLocation().Z - MousePosition.Z);
	MouseWorldPosition = MousePosition - MouseDirection * Z /MouseDirection.Z;
	
}





