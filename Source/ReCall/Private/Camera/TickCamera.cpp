// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TickCamera.h"
#include "Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATickCamera::ATickCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsFollowCharacter = true;

	CameraX = 0.0f;
	CameraY = 300.0f;
	CameraZ = 100.0f;

	CameraRY = -5.0f;

	BlendLoactionSpeed = 11.0f;

	InterRotatorSpeed = 200.0f;
}

void ATickCamera::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	SetActorRotation(FRotator(GetActorRotation().Pitch + CameraRY, GetActorRotation().Yaw, GetActorRotation().Roll));

	if (CharacterRef)
	{
		Cast<APlayerController>(CharacterRef->GetController())->SetViewTargetWithBlend(this);
		CharacterRef->SetTickCameraRef(this);
	}
}

void ATickCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterRef)
	{
		FRotator LookAtYaw(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CharacterRef->GetActorLocation()));
		FRotator LookAtRotator(GetActorRotation().Pitch, LookAtYaw.Yaw, 0.0f);
		FRotator InterRotator = FMath::RInterpTo(GetActorRotation(), LookAtRotator, DeltaTime, InterRotatorSpeed);
		SetActorRotation(InterRotator);
		if (bIsFollowCharacter)
		{
			FVector TargetLocation = CharacterRef->GetActorLocation() + FVector(CameraX, CameraY, CameraZ);
			FVector NextLoaction = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, BlendLoactionSpeed);
			SetActorLocation(NextLoaction);
		}
	}
}

