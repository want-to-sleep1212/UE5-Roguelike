#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameResultWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class ROGUELIKE_API UGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetResultText(bool bIsClear);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultText;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
