// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacterBase.h"
#include "Characters/Enemy/AIControllerBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);
	AttackVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), "AttackSocket");
	DeactiveAttackCollision();

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	MaxHealth = 100.0f;
	Health = MaxHealth;

	EnemyDamage = 10.0f;

	CurrentEnemyState = ECurrentEnemyState::EEMS_Idle;

	AIPerceptionCmp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionCmp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	AIPerceptionCmp->ConfigureSense(*SightConfig);
	//优先级，视觉优先
	AIPerceptionCmp->SetDominantSense(UAISense_Sight::StaticClass());

	if (AIPerceptionCmp)
	{
		SightConfig->SightRadius = 800.0f;
		SightConfig->LoseSightRadius = 2000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	}

	//AIPerceptionCmp->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacterBase::OnSightPerceptionUpdated);
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	AIControllerBaseRef = Cast<AAIControllerBase>(GetController());

	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacterBase::OnAttackVolumeOverlapBegin);
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacterBase::OnAttackVolumeOverlapEnd);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacterBase::OnAttackCollisionOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacterBase::OnAttackCollisionOverlapEnd);

	//AIControllerBaseRef->Patrol();

	StartLocation = GetActorLocation();
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
	{
		ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
		if (Player)
		{
			bAttackVolumeOverlapping = true;
			Attack();
		}
	}
}

void AEnemyCharacterBase::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
	{
		ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
		if (Player)
		{
			bAttackVolumeOverlapping = false;
			if (CurrentEnemyState != ECurrentEnemyState::EEMS_Attacking)
			{

			}
		}
	}
}

void AEnemyCharacterBase::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
	{
		ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
		if (Player)
		{
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Player, EnemyDamage, AIControllerBaseRef, this, DamageTypeClass);
			}
		}
	}
}

void AEnemyCharacterBase::OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemyCharacterBase::Attack()
{
	if (CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
	{
		if (AIControllerBaseRef)
		{
			AIControllerBaseRef->StopMovement();
		}
		if (CurrentEnemyState != ECurrentEnemyState::EEMS_Attacking)
		{
			CurrentEnemyState = ECurrentEnemyState::EEMS_Attacking;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AttackMontage)
			{
				const float PlayRate = FMath::RandRange(0.9f, 1.1f);
				const FString SectionName = FString::FromInt(FMath::RandRange(1, 1));
				AnimInstance->Montage_Play(AttackMontage, PlayRate);
				AnimInstance->Montage_JumpToSection(FName(*SectionName), AttackMontage);
			}
		}
	}
}

void AEnemyCharacterBase::AttackEnd()
{
	if (CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
	{
		CurrentEnemyState = ECurrentEnemyState::EEMS_Idle;

		if (bAttackVolumeOverlapping && CurrentEnemyState != ECurrentEnemyState::EEMS_Dead)
		{
			Attack();
		}
	}
}

void AEnemyCharacterBase::ActiveAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemyCharacterBase::DeactiveAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacterBase::Die()
{
	CurrentEnemyState = ECurrentEnemyState::EEMS_Dead;

	DeactiveAttackCollision();
	AttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AEnemyCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - Damage <= 0.0f)
	{
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= Damage;
	}

	return Health;
}

void AEnemyCharacterBase::OnSightPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	/*
	if (!AIControllerBaseRef->bIsChaseTarget)
	{
		for (auto Actor : UpdatedActors)
		{
			if (Cast<ACharacterBase>(Actor))
				AIControllerBaseRef->ChaseTarget(Cast<ACharacterBase>(Actor));
		}
	}
	*/
}

