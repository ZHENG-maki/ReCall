// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DoorInteract.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"

void ADoorInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_Door;
		PlayerRef->InteractRef = this;
	}
}

void ADoorInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_None;
		PlayerRef->InteractRef = nullptr;
	}
}

void ADoorInteract::OnInteract_Implementation()
{
	TipsComp->SetVisibility(false);
}
