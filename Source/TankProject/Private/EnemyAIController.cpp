// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "FrendlyTurret.h"
#include "TankPawn.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!EnemyPawn)
		return;
	if(PatrollingPoints.Num() > 0)
	{
		FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
		FVector PawnLocation = GetPawn()->GetActorLocation();
		
		if(FVector::Distance(CurrentPoint,PawnLocation)<MovementAccuracy)
		{
			CurrentPatrolPointIndex++;
			if(CurrentPatrolPointIndex >= PatrollingPoints.Num())
			{
				CurrentPatrolPointIndex=0;
			}
		
		}
		FVector Direction = CurrentPoint - PawnLocation;
		Direction.Normalize();
	
		auto ForwardVector = EnemyPawn->GetActorForwardVector();
		auto RightVector = EnemyPawn->GetActorRightVector();
		float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector,Direction)));
		float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct( RightVector, Direction)));
	
		int RotationValue = 0;
		if(ForwardAngle > 5)
		{
			if(ForwardAngle > 45)
			{
				EnemyPawn->MoveForward(0);
			}
			else
			{
				EnemyPawn->MoveForward(1);
			}
	
			if(RightAngle > 90)
				RotationValue = -1;
			
			else
				RotationValue = 1;
		}
		if(RotationValue != 0)
		{
			EnemyPawn->RotateRight(RotationValue);
		}
		else
		{
			EnemyPawn->RotateRight(0);
		}
		
	}

	
	if(Target.IsValid())
	{
		
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemyPawn->TurretMesh->GetComponentLocation(),Target->GetActorLocation());
		EnemyPawn->TurretMesh->SetWorldRotation(FMath::Lerp(EnemyPawn->TurretMesh->GetComponentRotation(), TargetRotation, TargetingSpeed));
		auto TargetingDirection = EnemyPawn->TurretMesh->GetForwardVector();
		auto PlayerDirection = Target->GetActorLocation() - EnemyPawn->GetActorLocation();
		PlayerDirection.Normalize();
		auto Angle=FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection,PlayerDirection)));
		if(Angle < AimSlack)
		{
			Fire();
		}
	} else if (EnemyPawn)
	{
		auto LookAt = EnemyPawn->TurretMesh->GetComponentLocation() + EnemyPawn->GetActorForwardVector() * 100;
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemyPawn->TurretMesh->GetComponentLocation(),LookAt);
		EnemyPawn->TurretMesh->SetWorldRotation(FMath::Lerp(EnemyPawn->TurretMesh->GetComponentRotation(), TargetRotation, TargetingSpeed));
		
	}
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	EnemyPawn = Cast<AEnemyPawn>(InPawn);
	
	
	if(!EnemyPawn)
	{
		return;
	}
	
	WaypointTag = EnemyPawn->WaypointTag;

	if(!WaypointTag.IsNone())
	{
		TArray<AActor*> Actors;
		TArray<AWaypointActor*> Waypoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointActor::StaticClass(),Actors);
		for(auto Actor : Actors)
		{
			auto Waypoint = CastChecked<AWaypointActor>(Actor);
			if(Waypoint->Tags.Contains(WaypointTag))
				Waypoints.Add(Waypoint);
		}
		Waypoints.Sort([] (AWaypointActor& a, const AWaypointActor& b)
		{
			return a.WaypointIndex > b.WaypointIndex;
		});
		for(auto Waypoint : Waypoints)
		{
			PatrollingPoints.Add(Waypoint->GetActorLocation());
		}
		
	}
	else if(EnemyPawn->Waypoints.Num()>0)
	{
		for(auto Waypoint : EnemyPawn->Waypoints)
		{
			PatrollingPoints.Add(Waypoint->GetActorLocation());
		}
	}
		for (int index = 1; index < PatrollingPoints.Num(); index++ )
		{
			DrawDebugLine(GetWorld(),PatrollingPoints[index-1],PatrollingPoints[index],FColor::Green,true);
		}
		if(PatrollingPoints.Num() > 0)
			DrawDebugLine(GetWorld(),PatrollingPoints[0],PatrollingPoints[PatrollingPoints.Num()-1],FColor::Green,true);
	
}
FVector AEnemyAIController::GetShootTarget() const
{
	
	if (Target.IsValid())
		return Target->GetActorLocation();
	
	if (EnemyPawn)
		return EnemyPawn->TurretMesh->GetComponentLocation() + EnemyPawn->GetActorForwardVector() * 100;
	return FVector::ZeroVector;
	
}

void AEnemyAIController::AddTarget(AActor* Actor)
{
	if (Actor == GetPawn()) // Test
		return;
	OtherActors.Add(Actor);
	
	if(!Target.IsValid())
	{
		FindNextTarget();
		
	}
}

void AEnemyAIController::RemoveTarget(AActor* Actor)
{
	OtherActors.Remove(Actor);
	if(Actor == Target)
	{
		FindNextTarget();
	}
}

void AEnemyAIController::FindNextTarget()
{
	if(!EnemyPawn)
	{
		return;
	}
	float MinRange = 100000;
	Target = nullptr;
	for(auto Actor : OtherActors)
	{
		float Range = FVector::Distance(Actor->GetActorLocation(),EnemyPawn->GetActorLocation());
		if(Range < MinRange)
		{
			MinRange=Range;
			auto TankPawn = Cast<ATankPawn>(Actor);
			auto FrendlyTurret = Cast<AFrendlyTurret>(Actor);
			if (TankPawn || FrendlyTurret)
			{
				Target = Actor;
			}
		}
	}
}



void AEnemyAIController::Fire()
{
	
	
		if(!Target.IsValid())
		{
			return;
		}
		FHitResult Result;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(EnemyPawn);
		Params.bTraceComplex = true;
				
		if(GetWorld()->LineTraceSingleByChannel(Result,EnemyPawn->TurretMesh->GetComponentLocation(),Target->GetActorLocation(),ECollisionChannel::ECC_Visibility,Params))
			
		{
			if(Result.Actor == Target.Get())
				//GEngine->AddOnScreenDebugMessage(1241115, 1, FColor::Purple,FString::Printf(TEXT("BAG Baag")));
			{
			if(Target.IsValid())
				{
					EnemyPawn->TurretShoot();
				}
			}
		}
	}
