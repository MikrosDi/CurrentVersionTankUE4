// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageTarget.generated.h"

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator;
};


// This class does not need to be modified.


UINTERFACE(MinimalAPI)
class UDamageTarget : public UInterface
{
	GENERATED_BODY()                         
};                                           
/**
 *
 */

class TANKPROJECT_API IDamageTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
	
	virtual void TakeDamage(FDamageData Damage) = 0;
};