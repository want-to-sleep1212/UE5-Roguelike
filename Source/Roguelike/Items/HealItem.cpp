// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Components/Health/HealthComponent.h"

// Sets default values
AHealItem::AHealItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	PickupCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AHealItem::OnOverlapBegin
	);
}

//// Called every frame
//void AHealItem::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

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
