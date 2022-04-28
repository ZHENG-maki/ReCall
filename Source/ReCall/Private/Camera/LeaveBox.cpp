// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LeaveBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CharacterBase.h"
#include "Camera/TickCamera.h"

// Sets default values
ALeaveBox::ALeaveBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeaveBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LeaveBoxComp"));
	RootComponent = LeaveBoxComp;
}

// Called when the game starts or when spawned
void ALeaveBox::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	LeaveBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALeaveBox::OnBeginOverlap);
	LeaveBoxComp->OnComponentEndOverlap.AddDynamic(this, &ALeaveBox::OnEndOverlap);
}

// Called every frame
void ALeaveBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeaveBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TickCameraRef && CharacterRef)
	{
		TickCameraRef = CharacterRef->GetTickCameraRef();
	}
	if (TickCameraRef)
	{
		TickCameraRef->SetbIsFollowCharacter(false);
	}
}

void ALeaveBox::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TickCameraRef)
	{
		TickCameraRef->SetbIsFollowCharacter(true);
	}
}

