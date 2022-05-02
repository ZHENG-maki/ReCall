// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AIControllerBase.h"
#include "Characters/Enemy/EnemyCharacterBase.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

AAIControllerBase::AAIControllerBase()
{
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	bIsPatrol = true;

	PatrolSpeed = 200.0f;
	ChaseSpeed = 600.0f;

	bIsTargetRun = true;

	AttackMontageIndex = 0;

	AttackRange = 350.0f;

	bIsChaseTarget = false;

	MaxChaseDistance = 1200.0f;
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_EnemyCharacterRef = Cast<AEnemyCharacterBase>(InPawn);

	AnimInstance = m_EnemyCharacterRef->GetMesh()->GetAnimInstance();
}

void AAIControllerBase::Patrol()
{
	const float PatrolRadius = 1000.0f;
	if (NavSystem)
	{
		FNavLocation RandomPoint;
		//以哪个点为中心，巡逻范围，返回找到的点
		bool bFound = NavSystem->GetRandomReachablePointInRadius(m_EnemyCharacterRef->GetActorLocation(), PatrolRadius, RandomPoint);
		if (bFound)
		{
			MoveToLocation(RandomPoint.Location);
		}
	}
}

void AAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bIsTargetRun)
	{
		bIsTargetRun = false;
		bIsPatrol = true;
		m_EnemyCharacterRef->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	}

	if (InAttackRange())
	{
		AttackBegin();
		return;
	}

	if (bIsPatrol)
	{
		GetWorldTimerManager().SetTimer(PatrolTimerHandle, this, &AAIControllerBase::CheckPatrol, 3.0f);
	}
}

void AAIControllerBase::CheckPatrol()
{
	if (bIsPatrol)
	{
		Patrol();
	}
}

int AAIControllerBase::GetAttackMontageIndex()
{
	return 0;
}

bool AAIControllerBase::InAttackRange()
{
	if (TargetActor)
	{
		//GetDistanceTo返回两个Actor之间的距离
		return (m_EnemyCharacterRef->GetDistanceTo(TargetActor) <= AttackRange);
	}
	else
	{
		return false;
	}
}

void AAIControllerBase::AttackBegin()
{
	bIsPatrol = false;

	if (m_EnemyCharacterRef->CurrentEnemyState != ECurrentEnemyState::EEMS_Dead && !bIsTargetRun && bIsChaseTarget)
	{
		m_EnemyCharacterRef->GetCharacterMovement()->StopMovementImmediately();
		if (TargetActor)
		{
			FRotator LookAtYaw = UKismetMathLibrary::FindLookAtRotation(m_EnemyCharacterRef->GetActorLocation(), TargetActor->GetActorLocation());
			m_EnemyCharacterRef->SetActorRotation(LookAtYaw);
		}

		AttackMontage = m_EnemyCharacterRef->AttackMontage;

		if (AnimInstance)
		{
			const float PlayRate = FMath::RandRange(0.9f, 1.1f);
			const FString SectionName = FString::FromInt(FMath::RandRange(1, 2));
			AnimInstance->Montage_Play(AttackMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(FName(*SectionName), AttackMontage);
		}

		GetWorldTimerManager().SetTimer(AttackEndTimerHandle, this, &AAIControllerBase::AttackEnd, AttackMontage->SequenceLength, false);
	}
}

void AAIControllerBase::AttackEnd()
{
	bIsPatrol = true;
	AttackMontage = nullptr;
	AttackMontageIndex = GetAttackMontageIndex();

	if (InAttackRange())
	{
		AttackBegin();
	}
	else
	{
		MoveToActor(TargetActor);
	}
}

void AAIControllerBase::ChaseTarget(AActor* Target)
{
	if (!bIsChaseTarget && !bIsTargetRun)
	{
		bIsChaseTarget = true;
		bIsPatrol = false;
		TargetActor = Target;

		m_EnemyCharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

		GetWorldTimerManager().SetTimer(GetTargetDistanceTimerHandle, this, &AAIControllerBase::GetTargetDistance, 1.0f, true);

		if (InAttackRange())
		{
			AttackBegin();
		}
		else
		{
			MoveToActor(TargetActor);
		}
	}
}

void AAIControllerBase::GetTargetDistance()
{
	if (FVector::Dist(m_EnemyCharacterRef->GetActorLocation(), TargetActor->GetActorLocation()) > MaxChaseDistance)
	{
		ResetStart();
	}
}

void AAIControllerBase::ResetStart()
{
	AnimInstance->Montage_Stop(0.0f);
	bIsTargetRun = true;
	AttackMontageIndex = 0;
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackEndTimerHandle);
	GetWorldTimerManager().ClearTimer(GetTargetDistanceTimerHandle);
	TargetActor = nullptr;
	bIsChaseTarget = false;
	MoveToLocation(m_EnemyCharacterRef->StartLocation);
	m_EnemyCharacterRef->Health = m_EnemyCharacterRef->MaxHealth;
	//m_EnemyCharacterRef->UpdateHealthProgressBar();
}
