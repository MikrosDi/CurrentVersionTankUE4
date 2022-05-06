// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFactory.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseFactory::ABaseFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	BuildingMesh->SetupAttachment(RootComponent);

	HitCollider = CreateDefaultSubobject<UBoxComponent>("HitCollider");
	HitCollider->SetupAttachment(RootComponent);

	SpawnEnemyPoint = CreateDefaultSubobject<UArrowComponent>("SpawnEnemyPoint");
	SpawnEnemyPoint->SetupAttachment(RootComponent);

	HealthComponent= CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseFactory::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ABaseFactory::OnDeath);

	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>("DeadEffect");
	DeadEffect->SetupAttachment(BuildingMesh);
	
	AudioDeadEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioDeadEffect->SetupAttachment(BuildingMesh);

	CreateEffect = CreateDefaultSubobject<UParticleSystemComponent>("CreateEffect");
	CreateEffect->SetupAttachment(BuildingMesh);
	
	AudioCreateEffect = CreateDefaultSubobject<UAudioComponent>("AudioCreateEffect");
	AudioCreateEffect->SetupAttachment(BuildingMesh);

	DestroyFabricSpawnPoint = CreateDefaultSubobject<UArrowComponent>("DestroyFabricSpawnPoint");
	DestroyFabricSpawnPoint->SetupAttachment(RootComponent);
}

void ABaseFactory::TakeDamage(FDamageData Damage)
{
	if(HealthComponent)
	HealthComponent->TakeDamage(Damage);
}

// Called when the game starts or when spawned
void ABaseFactory::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle,this,&ABaseFactory::OnTankSpawnTick,SpawnInterwal,true);
	
	//TArray<AActor*> Actors; //Цикл на нахождение только 1 актора
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestroyFabric::StaticClass(),Actors);
	//for(auto Actor : Actors)
	//{
	//	DestroyFabric=CastChecked<ADestroyFabric>(Actor);
	//}
}

// Called every frame
void ABaseFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseFactory::OnTankSpawnTick()
{
	if(CreateEffect && AudioCreateEffect)
	{
		CreateEffect->ActivateSystem();
		AudioCreateEffect->Play();
		if(TankSpawned > MaxSpawnTanks )
			return;
		auto Tank = GetWorld()->SpawnActorDeferred<AEnemyPawn>(TankClass,SpawnEnemyPoint->GetComponentTransform());
		Tank->Waypoints = Waypoints;
		TankSpawned++;
		Tank->Number = TankSpawned;
		UGameplayStatics::FinishSpawningActor(Tank,SpawnEnemyPoint->GetComponentTransform());
	}
}

void ABaseFactory::OnHealthChanged(float CurrentHealth)
{
	GEngine->AddOnScreenDebugMessage(9871662, 3, FColor::White,FString::Printf(TEXT("Health Enemy Fabric: %f"), CurrentHealth));
}

void ABaseFactory::OnDeath()
{
	auto Temp = GetActorLocation();
	DeadEffect->ActivateSystem();
	AudioDeadEffect->Play();
	GetWorld()->SpawnActor<ADestroyFabric>(DestroyFabricClass,DestroyFabricSpawnPoint->GetComponentLocation(),DestroyFabricSpawnPoint->GetComponentRotation());
	SetActorLocation({-1000, -1000, -1000});
	//DestroyFabric->SetActorLocation(Temp);
	DeadEffect->SetWorldLocation(Temp);
	AudioDeadEffect->SetWorldLocation(Temp);
	GetWorld()->GetTimerManager().SetTimer(TimerDestroed,this,&ABaseFactory::SelfDestroed,3,false);
	GEngine->AddOnScreenDebugMessage(9872, 3, FColor::White,FString::Printf(TEXT("Enemy Fabric Destroyed")));
	GetWorldTimerManager().ClearTimer(TimerHandle);

	
	if(LevelTrigger)
	{
		LevelTrigger->SetActive(true);
		GEngine->AddOnScreenDebugMessage(98722, 3, FColor::Green,FString::Printf(TEXT("New lvl is OPEN!")));
	}
	
}

void ABaseFactory::SelfDestroed()
{
	Destroy();
}