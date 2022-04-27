// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyAnimInstanceBase.h"
#include "Characters/Enemy/EnemyCharacterBase.h"

void UEnemyAnimInstanceBase::NativeInitializeAnimation()
{
	EnemyRef = Cast<AEnemyCharacterBase>(TryGetPawnOwner());
}

void UEnemyAnimInstanceBase::UpdataAnimInfo()
{
	if (!EnemyRef)
	{
		EnemyRef = Cast<AEnemyCharacterBase>(TryGetPawnOwner());
	}
	if (EnemyRef)
	{
		FVector SpeedVector = EnemyRef->GetVelocity();
		Speed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f).Size();
	}
}
