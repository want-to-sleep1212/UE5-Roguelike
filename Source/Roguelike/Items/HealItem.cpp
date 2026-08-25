// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Components/Health/HealthComponent.h"

AHealItem::AHealItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
	SetRootComponent(PickupCollision);

	PickupCollision->InitSphereRadius(50.0f);
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(PickupCollision);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	PickupCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AHealItem::OnOverlapBegin
	);
}

void AHealItem::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (bConsumed || OtherActor == nullptr)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player == nullptr)
	{
		return;
	}

	UHealthComponent* HealthComponent = Player->FindComponentByClass<UHealthComponent>();

	if (HealthComponent == nullptr)
	{
		return;
	}

	bConsumed = true;

	HealthComponent->Heal(HealAmount);

	Destroy();
}
