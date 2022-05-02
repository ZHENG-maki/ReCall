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
#include "GamePlay/DoorInteract.h"
#include "GamePlay/InteractItem/WeaponInteract.h"
#include "GamePlay/InteractItem/DoubleDoorInteract.h"
#include "Characters/Enemy/EnemyCharacterBase.h"
#include "GamePlay/InteractItem/WeaponInteract.h"

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

    InteractItemObj = EInteractItem::EII_None;
    InteractRef = nullptr;

    SetActorRotation(FRotator::ZeroRotator);

    CurrentPlayerState = ECurrentPlayerState::EPS_None;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

    CurrentPlayerState = ECurrentPlayerState::EPS_Normal;

    SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

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

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::OnLeftShift);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::OnEndLeftShift);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::OnInteract);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::AttackKeyDown);
}

void ACharacterBase::StopMovement()
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
}

void ACharacterBase::ResetMovement()
{
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ACharacterBase::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f) && (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);  

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f) && (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::PrepareJump()
{
    if (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
    {
		if (Cast<UAnimCharacterBase>(GetMesh()->GetAnimInstance()))
		{
			AnimCharacterBaseRef = Cast<UAnimCharacterBase>(GetMesh()->GetAnimInstance());
		}
		if (AnimCharacterBaseRef)
		{
			StopMovement();

			AnimCharacterBaseRef->bIsJump = true;
		}
    }
}

void ACharacterBase::StartJump()
{
    if (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
    {
		ResetMovement();

		AnimCharacterBaseRef->bIsJump = false;
		Jump();
    }    
}

void ACharacterBase::OnLeftShift()
{
    if (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
    {
        GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    }
}

void ACharacterBase::OnEndLeftShift()
{
    if (CurrentPlayerState == ECurrentPlayerState::EPS_Normal || CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
    {
        GetCharacterMovement()->MaxWalkSpeed = 150.0f;
    }
}

void ACharacterBase::OnInteract()
{
    switch (InteractItemObj)
    {
    case EInteractItem::EII_None:
        break;
    case EInteractItem::EII_Door:
    {
        if (InteractRef)
        {
            if (Cast<ADoorInteract>(InteractRef))
            {
                Cast<ADoorInteract>(InteractRef)->OnInteract();
            }
        }
        break;
    }
    case EInteractItem::EII_Chair:
    {
        if (InteractRef)
        {
			
        }
        break;
    }
	case EInteractItem::EII_DoubleDoor:
	{
        if (InteractRef)
        {
			if (Cast<ADoubleDoorInteract>(InteractRef))
			{
				Cast<ADoubleDoorInteract>(InteractRef)->OnInteract();
			}
        }
		break;
	}
	case EInteractItem::EII_Weapon:
	{
		if (InteractRef)
		{
			if (Cast<AWeaponInteract>(InteractRef))
			{
				Cast<AWeaponInteract>(InteractRef)->OnInteract();
			}
		}
		break;
	}
    default:
        break;
    }
}

void ACharacterBase::AttackKeyDown()
{
    if (CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
    {
        Attack();
    }
}

void ACharacterBase::Attack()
{
	if (CurrentPlayerState != ECurrentPlayerState::EPS_PlayMontage && !(GetMovementComponent()->IsFalling()) && CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
	{
        CurrentPlayerState = ECurrentPlayerState::EPS_PlayMontage;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			const float PlayRate = FMath::RandRange(1.25f, 1.75f);
            const FString SectionName = FString::FromInt(1);//FString::FromInt(FMath::RandRange(1, 1));
			AnimInstance->Montage_Play(AttackMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(FName(*SectionName), AttackMontage);
		}
	}
}

void ACharacterBase::AttackEnd()
{
    CurrentPlayerState = ECurrentPlayerState::EPS_Equip;
}
 
