// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.generated.h"

UENUM(BlueprintType)
enum class EInteractItem : uint8
{
	EII_None,
	EII_Door,
	EII_Chair,
	EII_Weapon
};

UCLASS()
class RECALL_API AInteract : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteract();

	UPROPERTY(VisibleAnywhere, Category = "Interact")
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Interact")
	class UBoxComponent* OverlapBoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Interact")
	class UMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact|UI")
	class UWidgetComponent* TipsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	class ACharacterBase* PlayerRef;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
