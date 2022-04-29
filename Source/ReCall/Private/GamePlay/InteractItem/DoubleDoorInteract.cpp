// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InteractItem/DoubleDoorInteract.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"

void ADoubleDoorInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_DoubleDoor;
		PlayerRef->InteractRef = this;
	}
}

void ADoubleDoorInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_None;
		PlayerRef->InteractRef = nullptr;
	}
}

void ADoubleDoorInteract::OnInteract_Implementation()
{
	TipsComp->SetVisibility(false);
}

