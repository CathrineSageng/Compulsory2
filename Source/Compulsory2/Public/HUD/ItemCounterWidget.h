#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemCounterWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPULSORY2_API UItemCounterWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Sets the item count text
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateItemCount(int32 ItemCount);

    // Reference to the text block in the widget
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountText;

};
