// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UENUM()
enum class EWidgetID : uint8
{
	Wid_MainMenu = 0x0,
	Wid_EscapeMenu = 0x1,
	Wid_PauseMenu = 0x2,
};

UCLASS()
class TANKPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

protected:
	
	virtual void BeginPlay() override;
	
public:

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* ShowWidget(const EWidgetID WidgetID, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION(BlueprintPure)
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

protected:
	
	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);

	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
};
