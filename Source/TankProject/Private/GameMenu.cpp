// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "Components/Button.h"


void UGameMenu::NativeConstruct()
{
	if(NewGameBtn)
	{
		NewGameBtn->OnPressed.AddDynamic(this,&ThisClass::OnStartClick);
	}
//	if(SettingsBtn)
//	{
//		SettingsBtn->OnPressed.AddDynamic(this,&ThisClass::OnSettingsClick);
//	}
	
	if(QuitBtn)
	{
		QuitBtn->OnPressed.AddDynamic(this,&ThisClass::OnQuitClick);
	}
}

void UGameMenu::NativeDestruct()
{
	if(NewGameBtn)
	{
		NewGameBtn->OnPressed.RemoveAll(this);
	}
	
//	if(SettingsBtn)
//	{
//	SettingsBtn->OnPressed.RemoveAll(this);
	//}

	if(QuitBtn)
	{
		QuitBtn->OnPressed.RemoveAll(this);
	}

	Super::NativeDestruct();
}



void UGameMenu::OnStartClick()
{
	if(NewGameAnimation)
	{
		PlayAnimation(NewGameAnimation);
	}
		
}

//void UGameMenu::OnSettingsClick()
//{
//	if(SettingsBtn)
//	{
//		PlayAnimation(SettingsAnimation);
//	}
//}

void UGameMenu::OnQuitClick()
{
	if(QuitBtn)
	{
		
		GEngine->Exec(GetWorld(),TEXT("Quit"));
	}
}





