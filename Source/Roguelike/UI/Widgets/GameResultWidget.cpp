#include "UI/Widgets/GameResultWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(
			this,
			&UGameResultWidget::OnRestartButtonClicked
		);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(
			this,
			&UGameResultWidget::OnQuitButtonClicked
		);
	}
}

void UGameResultWidget::SetResultText(bool bIsClear)
{
	if (!ResultText)
	{
		return;
	}

	ResultText->SetText(
		bIsClear
		? FText::FromString(TEXT("Game Clear"))
		: FText::FromString(TEXT("Game Over"))
	);
}

void UGameResultWidget::OnRestartButtonClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();

	UGameplayStatics::SetGamePaused(this, false);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
	}

	const FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));

	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}

void UGameResultWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(
		this,
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
	);
}
