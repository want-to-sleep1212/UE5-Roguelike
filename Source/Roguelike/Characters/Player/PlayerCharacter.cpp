// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"
#include "InputAction.h"

#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Math/UnrealMathUtility.h"

#include "Weapons/WeaponActor.h"
#include "Components/Combat/CombatComponent.h"

#include "Components/Health/HealthComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = false;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(
			this,
			&APlayerCharacter::Die
		);
	}
}

// Called every frame
//void APlayerCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedPlayerInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedPlayerInputComponent is nullptr"));
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (EnhancedSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedSubsystem is nullptr"));
		return;
	}

	if (IMC_Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IMC_Character is nullptr"));
		return;
	}

	EnhancedSubsystem->AddMappingContext(IMC_Character, 1);

	if (IA_Move == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move is nullptr"));
		return;
	}

	if (IA_Dash == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Dash is nullptr"));
		return;
	}
	
	if (IA_Attack == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Attack is nullptr"));
		return;
	}

	if (IA_Look == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Look is nullptr"));
		return;
	}

	EnhancedPlayerInputComponent->BindAction(
		IA_Move,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::MoveInput
	);
	EnhancedPlayerInputComponent->BindAction(
		IA_Move,
		ETriggerEvent::Completed,
		this,
		&APlayerCharacter::MoveInputCompleted
	);
	EnhancedPlayerInputComponent->BindAction(
		IA_Move,
		ETriggerEvent::Canceled,
		this,
		&APlayerCharacter::MoveInputCompleted
	);
	EnhancedPlayerInputComponent->BindAction(
		IA_Dash,
		ETriggerEvent::Started,
		this,
		&APlayerCharacter::DashInput
	);
	EnhancedPlayerInputComponent->BindAction(
		IA_Attack,
		ETriggerEvent::Started,
		this,
		&APlayerCharacter::AttackInput
	);
	EnhancedPlayerInputComponent->BindAction(
		IA_Look,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::LookInput
	);
}

void APlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	MoveInputValue = Value.Get<FVector2D>();

	if (LifeState == ELifeState::Dead ||
		ControlState == EControlState::Stunned)
	{
		return;
	}

	if (MovementState == EMovementState::Dash)
	{
		return;
	}

	if (MoveInputValue.IsNearlyZero())
	{
		MovementState = EMovementState::Idle;
		return;
	}

	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr"));
		return;
	}

	MovementState = EMovementState::Move;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = YawRotation.Vector();
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveDirection =
		ForwardDirection * MoveInputValue.Y +
		RightDirection * MoveInputValue.X;

	MoveDirection.Normalize();
	LastMoveDirection = MoveDirection;

	AddMovementInput(MoveDirection);
}

void APlayerCharacter::MoveInputCompleted(const FInputActionValue& Value)
{
	MoveInputValue = FVector2D::ZeroVector;

	if (MovementState != EMovementState::Dash)
	{
		MovementState = EMovementState::Idle;
	}
}

void APlayerCharacter::DashInput()
{
	if (LifeState == ELifeState::Dead ||
		ControlState == EControlState::Stunned)
	{
		return;
	}

	if (MovementState == EMovementState::Dash)
	{
		return;
	}

	if (MoveInputValue.IsNearlyZero())
	{
		return;
	}

	MovementState = EMovementState::Dash;

	FVector DashDirection = LastMoveDirection;
	SetActorRotation(DashDirection.Rotation());

	// 무적 상태 추가
	ActiveEffects.Add(FStatusEffect(
		EStatusEffectType::Invincible,
		DashInvincibleTime
	));

	LaunchCharacter(
		DashDirection * DashPower,
		true,
		false
	);

	GetWorldTimerManager().SetTimer(
		DashTimerHandle,
		this,
		&APlayerCharacter::EndDash,
		DashDuration,
		false
	);

	GetWorldTimerManager().SetTimer(
		InvincibleTimerHandle,
		this,
		&APlayerCharacter::EndInvincible,
		DashInvincibleTime,
		false
	);
}

void APlayerCharacter::EndDash()
{
	if (MoveInputValue.IsNearlyZero())
	{
		MovementState = EMovementState::Idle;
	}
	else
	{
		MovementState = EMovementState::Move;
	}

	// TODO: 무적 컴포넌트 방식이면 타이머로 자동 해제하거나 여기서 해제
}

void APlayerCharacter::EndInvincible()
{
	ActiveEffects.RemoveAll([](const FStatusEffect& Effect)
	{
		return Effect.Type == EStatusEffectType::Invincible;
	});
}

void APlayerCharacter::AttackInput()
{
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->StartAttack();
}

void APlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	const float LookSensitivity = 1.f;

	AddControllerYawInput(LookAxisVector.X/* * LookSensitivity*/);
	AddControllerPitchInput(LookAxisVector.Y/* * LookSensitivity*/);
}

float APlayerCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
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

	if (ActualDamage <= 0.0f || !HealthComponent)
	{
		return 0.0f;
	}

	HealthComponent->ApplyDamage(ActualDamage);

	return ActualDamage;
}

void APlayerCharacter::Die()
{
	if (LifeState == ELifeState::Dead)
	{
		return;
	}

	LifeState = ELifeState::Dead;

	UE_LOG(LogTemp, Warning, TEXT("Player Dead"));

	// 우선은 입력/이동만 막아두기
	GetCharacterMovement()->DisableMovement();

	// 나중에 할 것들
	// - 사망 애니메이션 재생
	// - 게임 오버 UI 표시
	// - 리스폰 또는 재시작 처리
}

float APlayerCharacter::GetCurrentHealth() const
{
	return HealthComponent
		? HealthComponent->GetCurrentHealth()
		: 0.0f;
}

float APlayerCharacter::GetMaxHealth() const
{
	return HealthComponent
		? HealthComponent->GetMaxHealth()
		: 0.0f;
}

EMovementState APlayerCharacter::GetMovementState() const
{
	return MovementState;
}

EControlState APlayerCharacter::GetControlState() const
{
	return ControlState;
}

ELifeState APlayerCharacter::GetLifeState() const
{
	return LifeState;
}

bool APlayerCharacter::CanStartAttack() const
{
	if (LifeState == ELifeState::Dead ||
		ControlState == EControlState::Stunned)
	{
		return false;
	}

	if (MovementState == EMovementState::Dash)
	{
		return false;
	}

	return true;
}
