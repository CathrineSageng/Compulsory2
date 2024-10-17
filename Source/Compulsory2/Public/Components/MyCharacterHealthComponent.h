#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCharacterHealthComponent.generated.h"

// Declare the delegate type outside the class scope but inside the header file
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyCharacterHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMyCharacterHealthComponent();

    // Function to apply damage to the character
    void ApplyDamage(int32 Damage);

    // Event for when the character dies
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnCharacterDeath OnCharacterDeath;

    // Function to get the current health
    int32 GetCurrentHealth() const;

    // Function to get the max health
    int32 GetMaxHealth() const;

protected:
    virtual void BeginPlay() override;

private:
    int32 CurrentHits;

    UPROPERTY(EditDefaultsOnly, Category = "Health")
    int32 MaxHits = 4; // Maximum hits allowed before death

    // Current health of the character
    int32 CurrentHealth;

    // Max health of the character
    int32 MaxHealth;
};
