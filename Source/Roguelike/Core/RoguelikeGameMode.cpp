#include "Core/RoguelikeGameMode.h"

#include "UI/Widgets/GameResultWidget.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

void ARoguelikeGameMode::GameOver()
{
	ShowGameResult(false);
}

void ARoguelikeGameMode::GameClear()
{
	ShowGameResult(true);
}

void ARoguelikeGameMode::ShowGameResult(bool bIsClear)
{
	if (!GameResultWidgetClass)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	GameResultWidget = CreateWidget<UGameResultWidget>(
		PlayerController,
		GameResultWidgetClass
	);

	if (!GameResultWidget)
	{
		return;
	}

	GameResultWidget->SetResultText(bIsClear);
	GameResultWidget->AddToViewport();

	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeUIOnly());

	UGameplayStatics::SetGamePaused(this, true);
}
