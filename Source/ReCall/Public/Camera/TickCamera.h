// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TickCamera.generated.h"

/**
 * 
 */
UCLASS()
class RECALL_API ATickCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	ATickCamera();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetbIsFollowCharacter(bool bIs) { bIsFollowCharacter = bIs; }
	FORCEINLINE bool GetbIsFollowCharacter() { return bIsFollowCharacter; }

	FORCEINLINE void SetCameraY(float Value) { CameraY = Value; }

private:
	class ACharacterBase* CharacterRef;

public:
	UPROPERTY(EditAnywhere)
	bool bIsFollowCharacter;

	//相机偏移量
	UPROPERTY(EditAnywhere)
	float CameraX;
	UPROPERTY(EditAnywhere)
	float CameraY;
	UPROPERTY(EditAnywhere)
	float CameraZ;

	UPROPERTY(EditAnywhere)
	float CameraRY;

	UPROPERTY(EditAnywhere)
	float BlendLoactionSpeed;

	UPROPERTY(EditAnywhere)
	float InterRotatorSpeed;
};
