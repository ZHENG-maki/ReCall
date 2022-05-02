// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InteractItem/WeaponInteract.h"
#include "..\..\..\Public\GamePlay\InteractItem\WeaponInteract.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Enemy/EnemyCharacterBase.h"

AWeaponInteract::AWeaponInteract()
{
	SocketName = "EquipWeaponSocket";

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());
	DeactiveAttackCollision();

	Damage = 20.0f;
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

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponInteract::OnAttackCollisionOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AWeaponInteract::OnAttackCollisionOverlapEnd);
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

void AWeaponInteract::ActiveDisplayMeshCollision()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void AWeaponInteract::DeactiveDisplayMeshCollision()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponInteract::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemyCharacterBase* BaseEnemy = Cast<AEnemyCharacterBase>(OtherActor);
		if (BaseEnemy)
		{
			
			if (DamageTypeClass)
			{
				AController* temp = UGameplayStatics::GetPlayerController(this, 0);
				UGameplayStatics::ApplyDamage(BaseEnemy, Damage, temp, this, DamageTypeClass);
			}
		}
	}
}

void AWeaponInteract::OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeaponInteract::ActiveAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeaponInteract::DeactiveAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


