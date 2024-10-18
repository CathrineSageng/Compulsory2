
#include "HUD/ItemCounterWidget.h"
#include "Components/TextBlock.h"

void UItemCounterWidget::UpdateItemCount(int32 ItemCount)
{
    // To check if the ItemCountText has initialized correctly
    if (ItemCountText)
    {
        ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("Items Collected: %d"), ItemCount)));
    }
}