#include "HUD/ItemCounterWidget.h"
#include "Components/TextBlock.h"

void UItemCounterWidget::UpdateItemCount(int32 ItemCount)
{
    if (ItemCountText)
    {
        ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("Items Collected: %d"), ItemCount)));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ItemCountText is null! Make sure it’s correctly set up in the widget blueprint."));
    }
}