// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"
#include "InputAction.h"

#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

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



	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));



	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(GetMesh());

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitBox->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	WeaponHitBox->SetBoxExtent(FVector(30.f, 60.f, 30.f));


	WeaponHitBox->SetGenerateOverlapEvents(true);


	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("RightHand"));

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponHitBox->SetupAttachment(WeaponMesh);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&APlayerCharacter::OnWeaponHitBoxOverlap
	);

	//UE_LOG(LogTemp, Warning, TEXT("WeaponHitBox Overlap Bound"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
	if (LifeState == ELifeState::Dead ||
		ControlState == EControlState::Stunned)
	{
		return;
	}

	if (ActionState == EActionState::Attack ||
		ActionState == EActionState::Skill)
	{
		return;
	}

	if (MovementState == EMovementState::Dash)
	{
		return;
	}

	ActionState = EActionState::Attack;

	WeaponComponent->StartAttack();

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//UE_LOG(LogTemp, Warning, TEXT("WeaponHitBox Enabled"));

	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&APlayerCharacter::EndAttack,
		0.5f,
		false
	);
}

void APlayerCharacter::EndAttack()
{
	ActionState = EActionState::None;

	WeaponComponent->EndAttack();

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//UE_LOG(LogTemp, Warning, TEXT("WeaponHitBox Disabled"));
}

void APlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	const float LookSensitivity = 1.f;

	AddControllerYawInput(LookAxisVector.X/* * LookSensitivity*/);
	AddControllerPitchInput(LookAxisVector.Y/* * LookSensitivity*/);
}

void APlayerCharacter::OnWeaponHitBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	//UE_LOG(LogTemp, Warning, TEXT("Weapon Overlap: %s"),
	//	OtherActor ? *OtherActor->GetName() : TEXT("NULL"));

	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}

	if (WeaponComponent == nullptr)
	{
		return;
	}

	WeaponComponent->HandleHit(OtherActor);
}
