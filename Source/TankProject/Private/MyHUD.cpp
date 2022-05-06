// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

AMyHUD::AMyHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMyHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UUserWidget* AMyHUD::ShowWidget(const EWidgetID WidgetID, const int32 ZOrder)
{
	HideWidget();
	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(WidgetID);
	if(ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr,ZOrder);
	}
	return CurrentWidget;
}

void AMyHUD::HideWidget()
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* AMyHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	CurrentWidget = CreateWidget(GetOwningPlayerController(),WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}




