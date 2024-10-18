
#include "Systems/MyInputSystem.h"
#include "Components/MyInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Components/MyLookComponent.h"
#include "Components/MyMovementComponent.h"

//Binds the actions for movement and looking
void UMyInputSystem::InitializeInput(APlayerController* PlayerController, UMyInputComponent* InputComponent, UMyMovementComponent* MovementComponent, UMyLookComponent* LookComponent)
{
    CachedMovementComponent = MovementComponent;
    CachedLookComponent = LookComponent;

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputComponent->SlashContext, 0);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
    {

        // Bind the movement actions
        EnhancedInputComponent->BindAction(InputComponent->MovementAction, ETriggerEvent::Triggered, this, &UMyInputSystem::HandleMoveInput);
        EnhancedInputComponent->BindAction(InputComponent->MovementAction, ETriggerEvent::Completed, this, &UMyInputSystem::StopMoveInput);

        // Bind the look actions
        EnhancedInputComponent->BindAction(InputComponent->LookAction, ETriggerEvent::Triggered, this, &UMyInputSystem::HandleLookInput);
        EnhancedInputComponent->BindAction(InputComponent->LookAction, ETriggerEvent::Completed, this, &UMyInputSystem::StopLookInput);
    }

}

//Called when the movement action is triggered 
void UMyInputSystem::HandleMoveInput(const FInputActionValue& Value)
{
    if (CachedMovementComponent)
    {
        CachedMovementComponent->MovementInput = Value.Get<FVector2D>();
    }
}

//Stops the movement input
void UMyInputSystem::StopMoveInput()
{
    if (CachedMovementComponent)
    {
        CachedMovementComponent->MovementInput = FVector2D::ZeroVector;
    }
}

//Called when the look action is triggered. 
void UMyInputSystem::HandleLookInput(const FInputActionValue& Value)
{
    if (CachedLookComponent)
    {
        CachedLookComponent->LookInput = Value.Get<FVector2D>();
    }
}

//Stops the look input 
void UMyInputSystem::StopLookInput()
{
    if (CachedLookComponent)
    {
        CachedLookComponent->LookInput = FVector2D::ZeroVector;
    }
}