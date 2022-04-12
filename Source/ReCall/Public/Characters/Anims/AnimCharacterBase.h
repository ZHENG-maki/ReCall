// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API UAnimCharacterBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
	bool bIsJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
	bool bIsBeginGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Info")
	class ACharacterBase* PlayerRef;

public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void UpdataAnimInfo();
};
