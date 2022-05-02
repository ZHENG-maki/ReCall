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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Attack")
		class UBoxComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Attack")
		class AController* WeaponOwner;
	
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

	void ActiveDisplayMeshCollision();

	void DeactiveDisplayMeshCollision();

	UFUNCTION()
		void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ActiveAttackCollision();

	UFUNCTION(BlueprintCallable)
		void DeactiveAttackCollision();
};
