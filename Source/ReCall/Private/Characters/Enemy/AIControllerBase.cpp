// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AIControllerBase.h"
#include "Characters/Enemy/EnemyCharacterBase.h"

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_EnemyCharacterRef = Cast<AEnemyCharacterBase>(InPawn);

}
