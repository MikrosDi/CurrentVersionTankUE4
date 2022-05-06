// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlateRadioButtons.h"
#include "SlateOptMacros.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlateRadioButtons::Construct(const FArguments& InArgs)
{
    OnRadioButtonChanged = InArgs._OnRadioButtonChanged;
	Count = InArgs._Count;
	CheckBoxStyle = &InArgs._Style->CheckBoxStyle;
	TextStyle = &InArgs._Style->TextStyle;
	//MainStyle = InArgs._Style;

	int32 SlotsNum = Count.Get();

	CreateVerticalBox(SlotsNum);
	
	if(CurrentIndex < SlotsNum)
	{
		CurrentIndex = InArgs._CurrentIndex.Get();
	}

	OnChangeCount.BindSP(this, &SSlateRadioButtons::OnCountChangedFunc);

	
	/*
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot() //SlotsNum
		[
          CreateCheckbox(0, TEXT("Option 1"))
		]
		+ SVerticalBox::Slot()
		[
		  CreateCheckbox(1, TEXT("Option 2"))
		]
		+ SVerticalBox::Slot()
		[
		  CreateCheckbox(2, TEXT("Option 3"))
		]
		
	];*/
}

TSharedRef<SWidget> SSlateRadioButtons::CreateCheckbox(int32 InIndex, FString InText)
{
	
	
	return  SNew(SCheckBox)
	.IsChecked_Raw(this, &SSlateRadioButtons::IsChecked, InIndex)
	.OnCheckStateChanged_Raw(this, &SSlateRadioButtons::OnCheckboxStateChanged, InIndex)
	.Style(CheckBoxStyle)
	[
	  SNew(STextBlock)
	  .Text(FText::FromString(InText))
	  .TextStyle(TextStyle)
	  
	];
}

ECheckBoxState SSlateRadioButtons::IsChecked(int32 InIndex) const
{
	return InIndex == CurrentIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SSlateRadioButtons::OnCheckboxStateChanged(ECheckBoxState NewState, int32 InIndex)
{
	if(NewState == ECheckBoxState::Checked)
	{
		CurrentIndex = InIndex;
		OnRadioButtonChanged.ExecuteIfBound(CurrentIndex);
	}
}

void SSlateRadioButtons::CreateVerticalBox(int32 SlotsCount)
{
	LeftOnTop = SNew(SVerticalBox);

	for(int i = 0; i < SlotsCount; i++)
	{
		LeftOnTop->AddSlot()
		[
		CreateCheckbox(i, FString::Printf(TEXT("Option %d"), i+1))
		];
		ChildSlot
		[
		 LeftOnTop.ToSharedRef()
		];
	}
}

void SSlateRadioButtons::OnCountChangedFunc(int32 NewCount)
{
	Count = NewCount;
	LeftOnTop.Reset();
	CreateVerticalBox(NewCount);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
