#include "Components/MyCharacterHealthComponent.h"

UMyCharacterHealthComponent::UMyCharacterHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentHits = 0;

    // Set default values for health
    MaxHealth = 4;          // Example default value for maximum health
    CurrentHealth = MaxHealth;  // Character starts with full health
}

int32 UMyCharacterHealthComponent::GetCurrentHealth() const
{
    return CurrentHealth;
}

int32 UMyCharacterHealthComponent::GetMaxHealth() const
{
    return MaxHealth;
}

void UMyCharacterHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMyCharacterHealthComponent::ApplyDamage(int32 Damage)
{
    // Reduce the current health by the amount of damage
    CurrentHealth -= Damage;

    // Ensure health doesn't drop below zero
    if (CurrentHealth < 0)
    {
        CurrentHealth = 0;
    }

    // Check if the character is dead
    if (CurrentHealth <= 0)
    {
        if (OnCharacterDeath.IsBound())
        {
            OnCharacterDeath.Broadcast(); // Notify any bound listeners of death
        }
    }
}

