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

    // bool for checking if the item is picked up 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bIsPickedUp;

    // The radius the item can be picked up 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float PickupRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemEffect;
};
