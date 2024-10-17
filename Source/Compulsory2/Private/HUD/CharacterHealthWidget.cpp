// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterHealthWidget.h"
#include "Components/TextBlock.h"

void UCharacterHealthWidget::UpdateHealth(int32 CurrentHealth, int32 MaxHealth)
{
    if (HealthText)
    {
        FText HealthTextValue = FText::Format(FText::FromString("Life: {0}/{1}"), CurrentHealth, MaxHealth);
        HealthText->SetText(HealthTextValue);
    }
}
