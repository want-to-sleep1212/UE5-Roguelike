// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/CombatComponent.h"

#include "Components/Health/HealthComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "UI/Components/HealthBarComponent.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

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
	if (LifeState == ELifeState::Dead)
	{
		return;
	}

	if (!CombatComponent || !AttackMontage)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return;
	}

	if (!CombatComponent->StartAttack())
	{
		return;
	}

	const float MontageLength = AnimInstance->Montage_Play(AttackMontage);

	if (MontageLength <= 0.0f)
	{
		CombatComponent->EndAttack();
		return;
	}

	FOnMontageEnded MontageEndedDelegate;

	MontageEndedDelegate.BindUObject(
		this,
		&AEnemyCharacter::OnAttackMontageEnded
	);

	AnimInstance->Montage_SetEndDelegate(
		MontageEndedDelegate,
		AttackMontage
	);
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

void AEnemyCharacter::BeginAttackWindow()
{
	if (CombatComponent)
	{
		CombatComponent->BeginAttackWindow();
	}
}

void AEnemyCharacter::EndAttackWindow()
{
	if (CombatComponent)
	{
		CombatComponent->EndAttackWindow();
	}
}

void AEnemyCharacter::OnAttackMontageEnded(
	UAnimMontage* Montage,
	bool bInterrupted
)
{
	if (Montage != AttackMontage)
	{
		return;
	}

	if (CombatComponent)
	{
		CombatComponent->EndAttack();
	}
}
