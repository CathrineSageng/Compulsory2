// Fill out your copyright notice in the Description page of Project Settings.

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

	void UpdateEnemyCount(int32 EnemiesLeft);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyCountText;
	
};
