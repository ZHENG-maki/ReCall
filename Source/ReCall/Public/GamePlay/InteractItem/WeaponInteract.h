// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Interact.h"
#include "WeaponInteract.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API AWeaponInteract : public AInteract
{
	GENERATED_BODY()

public:
	AWeaponInteract();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString SocketName;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteract();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void UnloadWeapon();
};
