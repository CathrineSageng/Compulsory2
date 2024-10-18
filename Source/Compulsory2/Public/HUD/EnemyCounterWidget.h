
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyCounterWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPULSORY2_API UEnemyCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Updated the count down of the enemy.
	// int32 EnemiesLeft is the number of how many enemies are left in the game.
	void UpdateEnemyCount(int32 EnemiesLeft);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyCountText;
	
};
