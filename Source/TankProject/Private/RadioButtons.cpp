// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtons.h"

#include "MyCoreStyle.h"
#include "SSlateRadioButtons.h"

void URadioButtons::ChangeCount(int32 NewCount)
{
	if(NewCount != Count)
	{
		Count = NewCount;
		RadioButtons->OnChangeCount.Execute(Count);
	}
}

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	RadioButtons.Reset();
}



TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	RadioButtons = SNew(SSlateRadioButtons)
	   .OnRadioButtonChanged_UObject(this, &ThisClass::OnRadioButtonsChangedFunc)
	   .Count(Count)
	   .CurrentIndex(CurrentIndex)
	   .Style(&WidgetStyle);
	
	

	return  RadioButtons.ToSharedRef();
	
}


void URadioButtons::OnRadioButtonsChangedFunc(int32 NewSelectedButton)
{
	OnRadioButtonChanged.Broadcast(NewSelectedButton);
}
