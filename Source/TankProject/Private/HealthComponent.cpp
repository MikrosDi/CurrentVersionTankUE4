// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "IScorable.h"
#include "TankProject/Parent class/ParentStarterClass.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float PrevValue = CurrentHealth;
	CurrentHealth -= DamageData.DamageValue;
	if(CurrentHealth < 0)
	{
		CurrentHealth = 0;
	}
	if(!FMath::IsNearlyEqual(PrevValue,CurrentHealth))
	{
		if(OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(CurrentHealth);
	}
	if(FMath::IsNearlyZero(CurrentHealth))
	{
		if(OnDeath.IsBound())
			OnDeath.Broadcast();
		auto Scorable = Cast<IScorable>(GetOwner());
		if (Scorable)
		{
			AActor* Actor = DamageData.Instigator;
			AParentStarterClass* ParentInstigator = Cast<AParentStarterClass>(Actor);
			if (ParentInstigator)
			{
				ParentInstigator->ReceivePoints(Scorable->GetPoint());
			}
		}
	}
}
// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}



