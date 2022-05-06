// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RadioButtonsWidgetStyle.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "RadioButtons.generated.h"

class SSlateRadioButtons;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, int32, NewSelectedButton);

UCLASS()
class TANKPROJECT_API URadioButtons : public UWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnRadioButtonChangedEvent OnRadioButtonChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadioButtonChange")
	int32 Count = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadioButtonChange")
	int32 CurrentIndex = 0;

	UPROPERTY(EditAnywhere, Category= Appeatance, Meta =(DisplayName = Style))
	FRadioButtonsStyle WidgetStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URadioButtons* RadioButtonStyle;
	
	UFUNCTION(BlueprintCallable)
	void ChangeCount(int32 NewCount);
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	
	TSharedPtr<SSlateRadioButtons> RadioButtons;

	void OnRadioButtonsChangedFunc(int32 NewSelectedButton);


	
};
