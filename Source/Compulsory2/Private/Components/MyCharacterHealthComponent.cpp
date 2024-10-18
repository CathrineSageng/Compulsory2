#include "Components/MyCharacterHealthComponent.h"

UMyCharacterHealthComponent::UMyCharacterHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentHits = 0;
    //Dafault value for health 
    MaxHealth = 4;          
    CurrentHealth = MaxHealth;  
}

//Gets the current health for character 
int32 UMyCharacterHealthComponent::GetCurrentHealth() const
{
    return CurrentHealth;
}

//Gets the maximun value for the character 
int32 UMyCharacterHealthComponent::GetMaxHealth() const
{
    return MaxHealth;
}

void UMyCharacterHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

//This function applies damage on the character 
void UMyCharacterHealthComponent::ApplyDamage(int32 Damage)
{
    CurrentHealth -= Damage;

    // The health can not go below 0
    if (CurrentHealth < 0)
    {
        CurrentHealth = 0;
    }

    // Check if the character is dead
    if (CurrentHealth <= 0)
    {
        if (OnCharacterDeath.IsBound())
        {
            OnCharacterDeath.Broadcast(); 
        }
    }
}

