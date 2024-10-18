
#include "HUD/EnemyCounterWidget.h"
#include "Components/TextBlock.h"

void UEnemyCounterWidget::UpdateEnemyCount(int32 EnemiesLeft)
{
    // To check if the EnemyCountText has initialized correctly
    if (EnemyCountText)
    {
        FText EnemyCountTextValue = FText::Format(FText::FromString("Enemies Left: {0}"), EnemiesLeft);
        EnemyCountText->SetText(EnemyCountTextValue);
    }
}
