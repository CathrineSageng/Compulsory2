// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EnemyCounterWidget.h"
#include "Components/TextBlock.h"

void UEnemyCounterWidget::UpdateEnemyCount(int32 EnemiesLeft)
{
    if (EnemyCountText)
    {
        FText EnemyCountTextValue = FText::Format(FText::FromString("Enemies Left: {0}"), EnemiesLeft);
        EnemyCountText->SetText(EnemyCountTextValue);
    }
}
