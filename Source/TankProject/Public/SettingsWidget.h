// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RadioButtons.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "SettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	//UEditableText* EditableText_168;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URadioButtons* RadioButtnon;

	

	void NativePreConstruct();
};
