#include "PlayerMainUI.h"
#include "Components/TextBlock.h"

#include "Characters/Player/PlayerCharacter.h"

void UPlayerMainUI::UpdateGold(int32 CurrentGold)
{
	if (!GoldText)
	{
		return;
	}

	GoldText->SetText(FText::AsNumber(CurrentGold));
}

void UPlayerMainUI::InitializePlayer(APlayerCharacter* InPlayerCharacter)
{
	if (!InPlayerCharacter)
	{
		return;
	}

	UpdateGold(InPlayerCharacter->GetGold());

	InPlayerCharacter->OnGoldChanged.AddDynamic(
		this,
		&UPlayerMainUI::UpdateGold
	);
}
