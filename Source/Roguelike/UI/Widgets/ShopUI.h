#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopUI.generated.h"

class UTextBlock;
class UButton;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopClosed);

UCLASS()
class ROGUELIKE_API UShopUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnShopClosed OnShopClosed;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMaxHealthUpgradeClicked();

	UFUNCTION()
	void OnHealClicked();

	UFUNCTION()
	void OnWeaponUpgradeClicked();

	UFUNCTION()
	void OnCloseClicked();

private:
	// MaxHealth
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHealthUpgradeNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHealthUpgradeDescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHealthUpgradePriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MaxHealthUpgradeBuyButton;


	// Heal
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealDescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HealBuyButton;


	// Weapon
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponUpgradeNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponUpgradeDescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponUpgradePriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WeaponUpgradeBuyButton;


	// Close
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	// Purchase once
	bool bMaxHealthUpgradePurchased = false;
	bool bHealPurchased = false;
	bool bWeaponUpgradePurchased = false;


	// DataTable
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName MaxHealthUpgradeRowName = TEXT("MaxHealthUpgrade");

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName HealRowName = TEXT("Heal");

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName WeaponUpgradeRowName = TEXT("WeaponUpgrade");
};
