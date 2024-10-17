#include "Components/MyCharacterHealthComponent.h"

UMyCharacterHealthComponent::UMyCharacterHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentHits = 0;
}

void UMyCharacterHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMyCharacterHealthComponent::ApplyDamage(int32 Damage)
{
    CurrentHits += Damage;
    if (CurrentHits >= MaxHits)
    {
        if (OnCharacterDeath.IsBound())
        {
            OnCharacterDeath.Broadcast(); // Notify any bound listeners of death
        }
    }
}

