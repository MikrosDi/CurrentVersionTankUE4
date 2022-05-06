// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomButton.h"
#include "GameModeInfoCustomizer.h"
#include "MyCoreStyle.h"
#include "DragAndDrop/ActorDragDropGraphEdOp.h"



#define  LOCTEXT_NAMESPASE "UMG"
UMyCustomButton::UMyCustomButton()
{
	static  ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>MyCustomStyle(TEXT("/Game/UI/WS_MyCustomButton"));
	SButton::FArguments ButtonDefaults;
	ButtonDefaults.ButtonStyle(MyCustomStyle.Object);
	WidgetStyle = *ButtonDefaults._ButtonStyle;
}
#if WITH_EDITOR
const FText UMyCustomButton::GetPaletteCategory()
{
	//return LOCTEXT("","ButtonCustom");
	return Super::GetPaletteCategory();
}
#endif

#undef LOCTEXT_NAMESPASE