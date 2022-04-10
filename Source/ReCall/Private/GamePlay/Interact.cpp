// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Interact.h"
#include "Components/BoxComponent.h"
#include "Characters/CharacterBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"

// Sets default values
AInteract::AInteract()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	OverlapBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxComp"));
	OverlapBoxComp->SetupAttachment(GetRootComponent());

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());

	TipsComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("TipsComp"));
	TipsComp->SetupAttachment(GetRootComponent());
	TipsComp->SetWidgetSpace(EWidgetSpace::World);
	TipsComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AInteract::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AInteract::OnOverlapBegin);
	OverlapBoxComp->OnComponentEndOverlap.AddDynamic(this, &AInteract::OnOverlapEnd);	
}

// Called every frame
void AInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<ACharacterBase>(OtherActor);		
	}
	if (PlayerRef)
	{
		TipsComp->SetVisibility(true);
	}
}

void AInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TipsComp->SetVisibility(false);
}