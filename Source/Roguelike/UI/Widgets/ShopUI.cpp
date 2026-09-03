#include "UI/Widgets/ShopUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"

#include "Data/ItemData.h"

#include "Characters/Player/PlayerCharacter.h"
#include "Components/Health/HealthComponent.h"
#include "Components/Combat/CombatComponent.h"

void UShopUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ItemDataTable)
	{
		return;
	}

	const FItemData* MaxHealthUpgradeData =
		ItemDataTable->FindRow<FItemData>(
			MaxHealthUpgradeRowName,
			TEXT("MaxHealthUpgrade")
		);

	if (MaxHealthUpgradeData)
	{
		MaxHealthUpgradeNameText->SetText(MaxHealthUpgradeData->ItemName);
		MaxHealthUpgradeDescriptionText->SetText(MaxHealthUpgradeData->Description);
		MaxHealthUpgradePriceText->SetText(
			FText::AsNumber(MaxHealthUpgradeData->Price)
		);
	}

	const FItemData* HealData =
		ItemDataTable->FindRow<FItemData>(
			HealRowName,
			TEXT("Heal")
		);

	if (HealData)
	{
		HealNameText->SetText(HealData->ItemName);
		HealDescriptionText->SetText(HealData->Description);
		HealPriceText->SetText(
			FText::AsNumber(HealData->Price)
		);
	}

	const FItemData* WeaponUpgradeData =
		ItemDataTable->FindRow<FItemData>(
			WeaponUpgradeRowName,
			TEXT("WeaponUpgrade")
		);

	if (WeaponUpgradeData)
	{
		WeaponUpgradeNameText->SetText(WeaponUpgradeData->ItemName);
		WeaponUpgradeDescriptionText->SetText(WeaponUpgradeData->Description);
		WeaponUpgradePriceText->SetText(
			FText::AsNumber(WeaponUpgradeData->Price)
		);
	}



	if (MaxHealthUpgradeBuyButton)
	{
		MaxHealthUpgradeBuyButton->OnClicked.AddDynamic(
			this,
			&UShopUI::OnMaxHealthUpgradeClicked
		);
	}

	if (HealBuyButton)
	{
		HealBuyButton->OnClicked.AddDynamic(
			this,
			&UShopUI::OnHealClicked
		);
	}

	if (WeaponUpgradeBuyButton)
	{
		WeaponUpgradeBuyButton->OnClicked.AddDynamic(
			this,
			&UShopUI::OnWeaponUpgradeClicked
		);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(
			this,
			&UShopUI::OnCloseClicked
		);
	}
}

void UShopUI::OnMaxHealthUpgradeClicked()
{
	if (bMaxHealthUpgradePurchased || !ItemDataTable)
	{
		return;
	}

	const FItemData* ItemData =
		ItemDataTable->FindRow<FItemData>(
			MaxHealthUpgradeRowName,
			TEXT("MaxHealthUpgrade")
		);

	if (!ItemData)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter =
		Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter)
	{
		return;
	}

	UHealthComponent* HealthComponent =
		PlayerCharacter->GetHealthComponent();

	if (!HealthComponent)
	{
		return;
	}

	if (!PlayerCharacter->SpendGold(ItemData->Price))
	{
		return;
	}

	HealthComponent->IncreaseMaxHealth(ItemData->MaxHealthIncrease);

	bMaxHealthUpgradePurchased = true;
	MaxHealthUpgradeBuyButton->SetIsEnabled(false);
}

void UShopUI::OnHealClicked()
{
	if (bHealPurchased || !ItemDataTable)
	{
		return;
	}

	const FItemData* ItemData =
		ItemDataTable->FindRow<FItemData>(
			HealRowName,
			TEXT("HealItem")
		);

	if (!ItemData)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter =
		Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter)
	{
		return;
	}

	UHealthComponent* HealthComponent =
		PlayerCharacter->GetHealthComponent();

	if (!HealthComponent)
	{
		return;
	}

	if (!PlayerCharacter->SpendGold(ItemData->Price))
	{
		return;
	}

	HealthComponent->Heal(ItemData->HealAmount);

	bHealPurchased = true;
	HealBuyButton->SetIsEnabled(false);
}

void UShopUI::OnWeaponUpgradeClicked()
{
	if (bWeaponUpgradePurchased || !ItemDataTable)
	{
		return;
	}

	const FItemData* ItemData =
		ItemDataTable->FindRow<FItemData>(
			WeaponUpgradeRowName,
			TEXT("WeaponUpgrade")
		);

	if (!ItemData)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter =
		Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter)
	{
		return;
	}

	if (!PlayerCharacter->SpendGold(ItemData->Price))
	{
		return;
	}

	UCombatComponent* CombatComponent =
		PlayerCharacter->GetCombatComponent();

	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->IncreaseAttackDamage(ItemData->AttackDamageIncrease);

	bWeaponUpgradePurchased = true;
	WeaponUpgradeBuyButton->SetIsEnabled(false);
}

void UShopUI::OnCloseClicked()
{
	OnShopClosed.Broadcast();
}
