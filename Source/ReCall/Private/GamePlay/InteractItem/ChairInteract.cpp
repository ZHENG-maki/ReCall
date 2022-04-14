// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InteractItem/ChairInteract.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"

void AChairInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_Chair;
		PlayerRef->InteractRef = this;
	}
}

void AChairInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (PlayerRef)
	{
		PlayerRef->InteractItemObj = EInteractItem::EII_None;
	}
}

void AChairInteract::OnInteract_Implementation()
{
	TipsComp->SetVisibility(false);
}

