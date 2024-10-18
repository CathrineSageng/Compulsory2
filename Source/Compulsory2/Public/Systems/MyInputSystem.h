
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputActionValue.h"
#include "MyInputSystem.generated.h"

class UMyInputComponent;
class UInputAction;
class APlayerController;
class UMyLookComponent;
class UMyMovementComponent;

UCLASS()
class COMPULSORY2_API UMyInputSystem : public UObject
{
    GENERATED_BODY()

public:
    void InitializeInput(APlayerController* PlayerController, UMyInputComponent* InputComponent, UMyMovementComponent* MovementComponent, UMyLookComponent* LookComponent);

    // Process movement input
    UMyMovementComponent* CachedMovementComponent;
    // Process Look input 
    UMyLookComponent* CachedLookComponent;

private:
    void HandleMoveInput(const FInputActionValue& Value);
    void HandleLookInput(const FInputActionValue& Value);
    void StopMoveInput();
    void StopLookInput();


};
