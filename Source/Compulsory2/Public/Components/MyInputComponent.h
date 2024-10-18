
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "MyInputComponent.generated.h"

// Forward declarations
class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FVector2D MovementInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FVector2D LookInput;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* SlashContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MovementAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    UMyInputComponent();

};
