// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Engine/DamageEvents.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(WeaponMesh);

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitBox->SetBoxExtent(FVector(30.f, 60.f, 30.f));
	WeaponHitBox->SetGenerateOverlapEvents(true);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&AWeaponActor::OnWeaponHitBoxOverlap
	);
}

void AWeaponActor::StartAttack()
{
	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	HitActors.Empty();

	EnableHitBox();

	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&AWeaponActor::EndAttack,
		AttackDuration,
		false
	);
}

void AWeaponActor::EndAttack()
{
	bIsAttacking = false;
	DisableHitBox();

	OnAttackEnded.Broadcast();

	//UE_LOG(LogTemp, Warning, TEXT("Weapon EndAttack"));
}

void AWeaponActor::EnableHitBox()
{
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponActor::DisableHitBox()
{
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponActor::OnWeaponHitBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!bIsAttacking)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();

	if (!OtherActor || OtherActor == OwnerActor)
	{
		return;
	}

	if (HitActors.Contains(OtherActor))
	{
		return;
	}

	HitActors.Add(OtherActor);

	OtherActor->TakeDamage(
		AttackDamage,
		FDamageEvent(),
		OwnerActor ? OwnerActor->GetInstigatorController() : nullptr,
		this
	);
}
