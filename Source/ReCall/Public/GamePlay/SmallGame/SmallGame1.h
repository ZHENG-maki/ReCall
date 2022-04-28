// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmallGame1.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API USmallGame1 : public UUserWidget
{
	GENERATED_BODY()

public:
	//绑定按钮
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	class UButton* Button1;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button2;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button3;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button4;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button5;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button6;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button7;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button8;
	UPROPERTY(Meta = (BindWidgetOptional), BlueprintReadWrite)
	UButton* Button9;
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	bool CheckCurrentButtonAngle();
};
