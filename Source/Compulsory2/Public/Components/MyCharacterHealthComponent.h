
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCharacterHealthComponent.generated.h"

// This delegate will be called when the character dies 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyCharacterHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMyCharacterHealthComponent();

    void ApplyDamage(int32 Damage);

    // Event for when the character dies
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnCharacterDeath OnCharacterDeath;

    // The current health on the character 
    int32 GetCurrentHealth() const;

    // Gets the maximum health on the character 
    int32 GetMaxHealth() const;

protected:
    virtual void BeginPlay() override;

private:
    // The currents hits the character has taken 
    int32 CurrentHits;

    // The maximum hits om the character is 4 
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    int32 MaxHits = 4; 

    // Current health on the character
    int32 CurrentHealth;

    // Max health on the character
    int32 MaxHealth;
};
