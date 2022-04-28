// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API UEnemyAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
		float Speed;

	class AEnemyCharacterBase* EnemyRef;

public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
		void UpdataAnimInfo();
};
