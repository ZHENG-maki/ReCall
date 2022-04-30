// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

UENUM(BlueprintType)
enum class ECurrentEnemyState :uint8
{
	EEMS_Idle,
	EEMS_Attacking,
	EEMS_Dead			
};

UCLASS()
class RECALL_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

	class AAIControllerBase* AIControllerBaseRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class USphereComponent* AttackVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
		ECurrentEnemyState CurrentEnemyState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		class UBoxComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float EnemyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
		bool bAttackVolumeOverlapping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	UFUNCTION(BlueprintCallable)
		void ActiveAttackCollision();

	UFUNCTION(BlueprintCallable)
		void DeactiveAttackCollision();

	UFUNCTION(BlueprintCallable)
	void Die();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
