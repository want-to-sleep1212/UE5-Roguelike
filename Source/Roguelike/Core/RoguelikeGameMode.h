#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoguelikeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARoguelikeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void GameOver();
	void GameClear();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameResultWidget> GameResultWidgetClass;

private:
	void ShowGameResult(bool bIsClear);

private:
	UPROPERTY()
	UGameResultWidget* GameResultWidget;
};
