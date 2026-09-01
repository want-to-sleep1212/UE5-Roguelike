#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguelikePlayerController.generated.h"

class UPlayerMainUI;

UCLASS()
class ROGUELIKE_API ARoguelikePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	// 호출 순서 문제를 막기 위해 BeginPlay, OnPossess 두 곳에서 호출
	// 이 클래스의 객체가 Player와 UI를 소유하고 있어야 성공
	// 중복 수행을 막기 위해 bPlayerUIInitialized로 초기화 확인
	void TryInitializePlayerUI();

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (DisplayName = "Player Main UI Class"))
	TSubclassOf<UPlayerMainUI> PlayerMainUIClass;

	UPROPERTY()
	TObjectPtr<UPlayerMainUI> PlayerMainUI;

	bool bPlayerUIInitialized = false;
};
