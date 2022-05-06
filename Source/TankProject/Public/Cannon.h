#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UENUM()

enum class ECannonType:uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	
	FireTrace = 1 UMETA(DisplayName = "Use trace") ,
	 
};


UCLASS()
class TANKPROJECT_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 0.4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Fire params")
	float LaserDamage = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireType = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<AProjectile> ProjectileClass;
	
	
	void Shoot(int& AmmoBB);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void ResetShootState();
	
	FTimerHandle TimerHandle;
	bool bReadyToShoot = true;
	

};