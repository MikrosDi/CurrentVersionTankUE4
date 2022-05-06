

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

class UButton;

UCLASS(Abstract)
class TANKPROJECT_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* NewGameBtn;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//UButton* SettingsBtn;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NewGameAnimation;
	
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
//	UWidgetAnimation* SettingsAnimation;
	
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
//	UWidgetAnimation* QuitAnimation;

	
    UFUNCTION()
	void OnStartClick();

//	UFUNCTION()
  //  void OnSettingsClick();
	
	UFUNCTION()
	void OnQuitClick();
	
};

