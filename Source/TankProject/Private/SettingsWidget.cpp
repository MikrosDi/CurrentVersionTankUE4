// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"

#include "MyCoreStyle.h"
#include "RadioButtonsWidgetStyle.h"

void USettingsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(RadioButtnon)
	{
		RadioButtnon->WidgetStyle = FMyCoreStyle::Get().GetWidgetStyle<FRadioButtonsStyle>("WS_RadioButtons_Default");
	}

	
	//if(EditableText_168)
	//{
	//	EditableText_168->WidgetStyle = FMyCoreStyle::Get().GetWidgetStyle<FEditableTextStyle>("WS_RadioButtons_Default");
	//}
}
