#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyItemComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyItemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMyItemComponent();

    // Tracks whether the item has been picked up
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bIsPickedUp;

    // Detection radius for picking up the item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float PickupRadius;

    // The item's effect (could be a placeholder for any effect the item has)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemEffect;
};
