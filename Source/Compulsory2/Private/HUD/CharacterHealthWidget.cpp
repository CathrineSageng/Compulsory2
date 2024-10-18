
#include "HUD/CharacterHealthWidget.h"
#include "Components/TextBlock.h"

void UCharacterHealthWidget::UpdateHealth(int32 CurrentHealth, int32 MaxHealth)
{
    // To check if the HealthText has initialized correctly
    if (HealthText)
    {
        FText HealthTextValue = FText::Format(FText::FromString("Life: {0}/{1}"), CurrentHealth, MaxHealth);
        HealthText->SetText(HealthTextValue);
    }
}
