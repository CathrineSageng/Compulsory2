
#include "Systems/MyInputSystem.h"
#include "Components/MyInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Components/MyLookComponent.h"
#include "Components/MyMovementComponent.h"


void UMyInputSystem::InitializeInput(APlayerController* PlayerController, UMyInputComponent* InputComponent, UMyMovementComponent* MovementComponent, UMyLookComponent* LookComponent)
{

    // Cache the input component for use in other functions
    CachedMovementComponent = MovementComponent;
    CachedLookComponent = LookComponent;

    // Get the Enhanced Input Subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputComponent->SlashContext, 0);
    }

    // Set up the Enhanced Input Component for action binding
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
    {

        // Bind the movement actions
        EnhancedInputComponent->BindAction(InputComponent->MovementAction, ETriggerEvent::Triggered, this, &UMyInputSystem::HandleMoveInput);
        EnhancedInputComponent->BindAction(InputComponent->MovementAction, ETriggerEvent::Completed, this, &UMyInputSystem::StopMoveInput);

        // Bind the look actions
        EnhancedInputComponent->BindAction(InputComponent->LookAction, ETriggerEvent::Triggered, this, &UMyInputSystem::HandleLookInput);
        EnhancedInputComponent->BindAction(InputComponent->LookAction, ETriggerEvent::Completed, this, &UMyInputSystem::StopLookInput);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast InputComponent to UEnhancedInputComponent. Ensure Enhanced Input is enabled and configured correctly."));
    }
}

void UMyInputSystem::HandleMoveInput(const FInputActionValue& Value)
{
    if (CachedMovementComponent)
    {
        CachedMovementComponent->MovementInput = Value.Get<FVector2D>();
        /* UE_LOG(LogTemp, Warning, TEXT("Movement Input: X=%f, Y=%f"), CachedMovementComponent->MovementInput.X, CachedMovementComponent->MovementInput.Y);*/
    }
}

void UMyInputSystem::StopMoveInput()
{
    if (CachedMovementComponent)
    {
        CachedMovementComponent->MovementInput = FVector2D::ZeroVector;
    }
}

void UMyInputSystem::HandleLookInput(const FInputActionValue& Value)
{
    if (CachedLookComponent)
    {
        CachedLookComponent->LookInput = Value.Get<FVector2D>();
        /*UE_LOG(LogTemp, Warning, TEXT("Look Input: X=%f, Y=%f"), CachedLookComponent->LookInput.X, CachedLookComponent->LookInput.Y);*/
    }
}

void UMyInputSystem::StopLookInput()
{
    if (CachedLookComponent)
    {
        CachedLookComponent->LookInput = FVector2D::ZeroVector;
    }
}