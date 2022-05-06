// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "LevelTrigger.generated.h"

UCLASS()
class TANKPROJECT_API ALevelTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ALevelTrigger();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USpotLightComponent* InactiveLight;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USpotLightComponent* ActiveLight;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* CreateEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* CreateEffect2;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Level")
	bool IsActive = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level")
	FName LevelName;
	
	
	void SetActive(bool Active);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapTrigger(UPrimitiveComponent* Component, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult);

};
