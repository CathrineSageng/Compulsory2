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
    // Will update the text UI and show the current number of items that has been collected.
    // ItemCount is the current number of collected items.
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateItemCount(int32 ItemCount);

    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountText;

};
