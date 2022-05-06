// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "RadioButtonsWidgetStyle.h"

DECLARE_DELEGATE_OneParam(FOnRadioButtonChanged, int32) /*NewSelectedIndex*/

DECLARE_DELEGATE_OneParam(FOnChangeCount, int32) /*NewStreng*/

class TANKPROJECT_API SSlateRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlateRadioButtons)
	  {
	  }
	SLATE_ATTRIBUTE(int32, Count);

	SLATE_ATTRIBUTE(int32, CurrentIndex);
	
	SLATE_EVENT(FOnRadioButtonChanged,OnRadioButtonChanged)

	SLATE_STYLE_ARGUMENT(FRadioButtonsStyle, Style)
	
	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnRadioButtonChanged OnRadioButtonChanged;

	FOnChangeCount OnChangeCount;
	
	TAttribute<int32> Count = 1;

	TSharedPtr<SVerticalBox> LeftOnTop;

	int32 CurrentIndex = 0;

	const FCheckBoxStyle* CheckBoxStyle = nullptr;

	const FTextBlockStyle* TextStyle = nullptr;

	

private:

	TSharedRef<SWidget> CreateCheckbox(int32 InIndex, FString InText);
	
	ECheckBoxState IsChecked(int32 InIndex) const;

	void OnCheckboxStateChanged(ECheckBoxState NewState, int32 InIndex);

	void OnCountChangedFunc(int32 NewCount);

	void CreateVerticalBox(int32 SlotsCount);
	
};
