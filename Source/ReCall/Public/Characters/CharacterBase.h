// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlay/Interact.h"
#include "CharacterBase.generated.h"

UCLASS()
class RECALL_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//第三人称使用相机臂
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool bIsOpenThiredCamera;

	//动画
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	class UAnimCharacterBase* AnimCharacterBaseRef;

	//交互
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	EInteractItem InteractItemObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	class AInteract* InteractRef;

private:
	class ATickCamera* TickCameraRef;

public:
	FORCEINLINE ATickCamera* GetTickCameraRef() { return TickCameraRef; }

	FORCEINLINE void SetTickCameraRef(ATickCamera* Ref) { TickCameraRef = Ref; }

	UFUNCTION(BlueprintCallable)
	void StopMovement();

	UFUNCTION(BlueprintCallable)
	void ResetMovement();
private:
	//移动相关
	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrepareJump();

	void StartJump();

	void OnLeftShift();

	void OnEndLeftShift();
	
	//交互
	void OnInteract();

};
