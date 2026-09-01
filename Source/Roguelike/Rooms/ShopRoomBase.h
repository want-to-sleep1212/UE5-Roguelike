#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"
#include "ShopRoomBase.generated.h"

class UShopUI;

UCLASS()
class ROGUELIKE_API AShopRoomBase : public ARoomBase
{
	GENERATED_BODY()
	
public:
	virtual bool StartRoom() override;

protected:
	virtual void OnRoomTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

private:
	UFUNCTION()
	void HandleShopClosed();

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UShopUI> ShopUIClass;

	UPROPERTY()
	TObjectPtr<UShopUI> ShopUI;

	TWeakObjectPtr<APlayerController> ShopPlayerController;
};
