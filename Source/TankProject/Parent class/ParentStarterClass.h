
#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "DamageTarget.h"
#include "HealthComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "ParentStarterClass.generated.h"

UCLASS(Blueprintable)
class TANKPROJECT_API AParentStarterClass : public APawn, public IDamageTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AParentStarterClass();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonPosition;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
	int Points = 0;
	
	virtual void TakeDamage(FDamageData Damage) override;

	void ReceivePoints(int InPoints);
	
	virtual void OnPointsReceived(int InPoints) {}

protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;
	
	virtual void OnDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};


