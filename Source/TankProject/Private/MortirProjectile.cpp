
#include "MortirProjectile.h"
#include "DamageTarget.h"
#include "Turret.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"


AMortirProjectile::AMortirProjectile()
{		
	Collision->SetSimulatePhysics(true);
	Collision->BodyInstance.bNotifyRigidBodyCollision = true;
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	
	FlyEffect = CreateDefaultSubobject<UParticleSystemComponent>("FlyEffect");
	FlyEffect->SetupAttachment(Mesh);

	BoomEffect = CreateDefaultSubobject<UParticleSystemComponent>("BoomEffect");
	BoomEffect->SetupAttachment(Mesh);
}

void AMortirProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	TArray<FHitResult> Targets;                                  
	GetWorld()->SweepMultiByChannel(Targets, GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(BlastRegion->GetScaledSphereRadius()));
	
	for(auto& HitTarget:Targets)
	{
		
			auto Target = Cast<IDamageTarget>(HitTarget.Actor);
			if(Target)
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.Instigator = GetInstigator();
				Target->TakeDamage(DamageData);
			}
		
			if(Cast<ABaseProjectile>(HitTarget.Actor))
			{
				continue;
			}
		
		if(HitTarget.Actor.IsValid())
		{
			const auto Root = Cast<UPrimitiveComponent>(HitTarget.Actor->GetRootComponent());
			
			if(Root && Root->IsSimulatingPhysics() && Root)
			{
				
				auto Impulse = GetActorForwardVector() * 10000 ;
				Root->AddImpulseAtLocation(Impulse, HitTarget.Location);
			}
			
			auto Temp = GetActorLocation();
			BoomEffect->ActivateSystem();
			SetActorLocation({-500, -500, -500});
			BoomEffect->SetWorldLocation(Temp);
		}
	}
	
}


void AMortirProjectile::BeginPlay()
{
	Super::BeginPlay();

	//if(BlastRegionActiveOrDeactive)
//	{
	//	Collision->BodyInstance.bNotifyRigidBodyCollision = true;
	//	Collision->SetCollisionObjectType(ECC_MAX);
	//}
	//else
	//	Collision->OnComponentBeginOverlap.AddDynamic(this,&AMortirProjectile::)
	
	GetWorld()->GetTimerManager().SetTimer(TimerDestroed, this,&AMortirProjectile::SelfDestroed, SelfDestroedSec, false);
}

void AMortirProjectile::SelfDestroed()
{
	Destroy();
};
	