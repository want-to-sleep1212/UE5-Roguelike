#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainUI.generated.h"

class UTextBlock;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API UPlayerMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateGold(int32 CurrentGold);

	void InitializePlayer(APlayerCharacter* InPlayerCharacter);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;
};
