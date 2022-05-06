
#pragma once

#include "CoreMinimal.h"
#include "DamageTarget.h"
#include "TankPawn.h"
#include "TurretCannon.h"
#include "GameFramework/Actor.h"
#include "TankProject/Parent class/ParentStarterClass.h"
#include "IScorable.h"
#include "FrendlyTurret.generated.h"


UCLASS()
class TANKPROJECT_API AFrendlyTurret : public AParentStarterClass, public IScorable
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* RangeSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ATurretCannon>CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ACannon>CannonClass1;

	UPROPERTY()
	ACannon* Cannon;
	
	UPROPERTY()
	ATurretCannon* TurretCannon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float TargetingSpeed = 0.1f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float TargetingRate = 0.005f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Fire params")
	float AimSlack = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Score")
	int ScorePoints = 100;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeadEffect2;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioDeadEffect2;
	
	int AmmoII = 9999;
	
	// Sets default values for this actor's properties
	AFrendlyTurret();
	
	virtual void TakeDamage(FDamageData Damage) override;

	virtual int GetPoint() const override;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnHealthChanged(float CurrentHealth);

private:
	
	void FindNextTarget();
	void Targeting();
	void Fire();
	void OnDeath();
	void SelfDestroed();
	
	UPROPERTY()
	TArray<AActor*> OtherActors;
	TWeakObjectPtr<AActor> Target;
	
	FTimerHandle TimerDestroed;

	UPROPERTY()
	class ATankPawn* TankPawn;

	//UPROPERTY()
	//class AEnemyPawn* EnemyPawn;

//	UPROPERTY()
//	class ATurret* Turret;

	UPROPERTY()
	class AFrendlyTurret* FrendlyTurret;
};