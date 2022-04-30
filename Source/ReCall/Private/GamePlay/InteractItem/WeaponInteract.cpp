// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InteractItem/WeaponInteract.h"
#include "..\..\..\Public\GamePlay\InteractItem\WeaponInteract.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

AWeaponInteract::AWeaponInteract()
{
	SocketName = "EquipWeaponSocket";
}

void AWeaponInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_Weapon;
		PlayerRef->InteractRef = this;
	}
}

void AWeaponInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_None;
		PlayerRef->InteractRef = nullptr;
	}
}

void AWeaponInteract::OnInteract_Implementation()
{
	TipsComp->SetVisibility(false);

	if (PlayerRef->CurrentPlayerState == ECurrentPlayerState::EPS_Normal)
	{
		EquipWeapon();
	}
	else if(PlayerRef->CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
	{
		UnloadWeapon();
	}
}

void AWeaponInteract::EquipWeapon()
{
	if (PlayerRef)
	{
		const USkeletalMeshSocket* SkeletalMeshSocket = PlayerRef->GetMesh()->GetSocketByName(*SocketName);
		if (SkeletalMeshSocket)
		{
			SkeletalMeshSocket->AttachActor(this, PlayerRef->GetMesh());
			PlayerRef->CurrentPlayerState = ECurrentPlayerState::EPS_Equip;
			TipsComp->SetVisibility(false);
		}
	}
}

void AWeaponInteract::UnloadWeapon()
{
	if (PlayerRef && !PlayerRef->GetMovementComponent()->IsFalling() && PlayerRef->CurrentPlayerState == ECurrentPlayerState::EPS_Equip)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetActorRotation(FRotator(0.0f));
		SetActorScale3D(FVector(1.0f));
	}
}


