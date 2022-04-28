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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Possess")
	class AEnemyCharacterBase* m_EnemyCharacterRef;
	
public:
	virtual void OnPossess(APawn* InPawn) override;

};
