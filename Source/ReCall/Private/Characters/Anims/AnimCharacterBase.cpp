// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Anims/AnimCharacterBase.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAnimCharacterBase::NativeInitializeAnimation()
{
	PlayerRef = Cast<ACharacterBase>(TryGetPawnOwner());

	bIsBeginGame = true;
}

void UAnimCharacterBase::UpdataAnimInfo()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<ACharacterBase>(TryGetPawnOwner());
	}
	if (PlayerRef)
	{
		FVector SpeedVector = PlayerRef->GetVelocity();
		Speed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f).Size();

		bIsFalling = PlayerRef->GetMovementComponent()->IsFalling();
	}
}
