// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Possess")
	class AEnemyCharacterBase* m_EnemyCharacterRef;

	class UNavigationSystemV1* NavSystem;

	FTimerHandle PatrolTimerHandle;

	float PatrolSpeed;

	float ChaseSpeed;

	AActor* TargetActor; //要攻击或追逐的对象

	bool bIsTargetRun; //判断目标是否脱离

	//UPROPERTY(EditAnywhere, Category = "Anim|Montage")
	UAnimMontage* AttackMontage;

	UAnimInstance* AnimInstance;

	int AttackMontageIndex; //攻击动画索引 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	FTimerHandle AttackEndTimerHandle;

	bool bIsChaseTarget;

	float MaxChaseDistance;

	FTimerHandle GetTargetDistanceTimerHandle;
	
public:
	virtual void OnPossess(APawn* InPawn) override;

	bool bIsPatrol;
	void Patrol();

	//MoveToLocation完成时调用
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void CheckPatrol();

	int GetAttackMontageIndex();

	//判断目标是否在攻击范围内
	bool InAttackRange();

	void AttackBegin();

	void AttackEnd();

	void ChaseTarget(AActor* Target);

	void GetTargetDistance();

	void ResetStart();
};
