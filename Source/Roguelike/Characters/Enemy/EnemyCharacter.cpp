// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
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
	//UE_LOG(LogTemp, Warning, TEXT("Enemy TakeDamage"));
	Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	CurrentHealth -= DamageAmount;

	//UE_LOG(
	//	LogTemp,
	//	Warning,
	//	TEXT("%s Take Damage : %.1f / Current HP : %.1f"),
	//	*GetName(),
	//	DamageAmount,
	//	CurrentHealth
	//);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageAmount;
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

	// Destroy 전에 호출해야 적 위치에서 드랍됨.
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
