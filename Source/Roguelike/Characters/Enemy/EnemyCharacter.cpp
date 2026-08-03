// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/CombatComponent.h"

#include "Components/Health/HealthComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "UI/Components/HealthBarComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	CharacterHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealthComponent"));

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));

	HealthBarComponent->SetupAttachment(GetRootComponent());
	HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterHealthComponent)
	{
		CharacterHealthComponent->OnDeath.AddDynamic(
			this,
			&AEnemyCharacter::Die
		);
	}
}

//// Called every frame
//void AEnemyCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy TakeDamage: %f"),
		DamageAmount
	);

	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	if (ActualDamage <= 0.0f || CharacterHealthComponent == nullptr)
	{
		return 0.0f;
	}

	CharacterHealthComponent->ApplyDamage(ActualDamage);

	return ActualDamage;
}

void AEnemyCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Attack Called"));
}

void AEnemyCharacter::Die()
{
	//UE_LOG(
	//	LogTemp,
	//	Warning,
	//	TEXT("%s Died"),
	//	*GetName()
	//);

	if (LifeState == ELifeState::Dead)
	{
		return;
	}

	LifeState = ELifeState::Dead;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Die"));

	// Destroy 전에 호출
	TryDropItem();

	OnEnemyDead.Broadcast(this);

	Destroy();
}

void AEnemyCharacter::TryDropItem()
{
	if (DropItemClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItemClass is nullptr"));
		return;
	}

	const float RandomValue = FMath::FRand();

	if (RandomValue > DropChance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item drop failed. Random: %.2f, Chance: %.2f"), RandomValue, DropChance);
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 30.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AActor* DroppedItem = GetWorld()->SpawnActor<AActor>(
		DropItemClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (DroppedItem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy dropped item"));
	}
}

bool AEnemyCharacter::CanStartAttack() const
{
	if (LifeState == ELifeState::Dead)
	{
		return false;
	}

	return true;
}

UCombatComponent* AEnemyCharacter::GetCombatComponent() const
{
	return CombatComponent;
}

void AEnemyCharacter::Activate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetComponentTickEnabled(true);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	if (CombatComponent)
	{
		CombatComponent->SetWeaponVisible(true);
	}
}

void AEnemyCharacter::Deactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}

	if (CombatComponent)
	{
		CombatComponent->SetWeaponVisible(false);
	}
}
