
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHealthWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPULSORY2_API UCharacterHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // Updates the health UI 
    // Has to parameters: X is CurrentHalth and Y is MaxHealth
    UFUNCTION(BlueprintCallable, Category = "Health")
    void UpdateHealth(int32 CurrentHealth, int32 MaxHealth);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;
};
