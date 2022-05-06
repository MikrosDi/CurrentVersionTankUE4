// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/Button.h"
#include "MyCustomButton.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API UMyCustomButton : public UButton
{
	GENERATED_BODY()
public:
	UMyCustomButton();

#if WITH_EDITOR

	virtual const FText GetPaletteCategory() override;
#endif
	
	
};
