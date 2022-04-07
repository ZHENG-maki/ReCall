// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Characters/Anims/AnimCharacterBase.h"
#include "Animation/AnimSequence.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

    bIsOpenThiredCamera = true;

    GetCapsuleComponent()->InitCapsuleSize(82.0f, 30.0f);

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 150.0f;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SocketOffset = FVector(0.f, 0.f, 75.f);
    SpringArmComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
    if (!bIsOpenThiredCamera)
    {
        //GetCharacterMovement()->bOrientRotationToMovement = false;
    }
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

    if (bIsOpenThiredCamera)
    {
        PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
        PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    }

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::PrepareJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::StartJump);
}

void ACharacterBase::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);  

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::PrepareJump()
{
    if (Cast<UAnimCharacterBase>(GetMesh()->GetAnimInstance()))
    {
        AnimCharacterBaseRef = Cast<UAnimCharacterBase>(GetMesh()->GetAnimInstance());
    }
    if (AnimCharacterBaseRef)
    {
        //停止运动
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->StopMovementImmediately();

        AnimCharacterBaseRef->bIsJump = true;
    }

}

void ACharacterBase::StartJump()
{
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    AnimCharacterBaseRef->bIsJump = false;
    Jump();
}
 
