#include "Systems/MyLookSystem.h"
#include "GameFramework/PlayerController.h"
#include "Components/MyLookComponent.h"

// Process the look input to rotate the camera
void UMyLookSystem::Execute(AController* Controller, UMyLookComponent* LookComponent)
{
    if (LookComponent)
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
        {
            PlayerController->AddPitchInput(LookComponent->LookInput.Y);
            PlayerController->AddYawInput(LookComponent->LookInput.X);
        }
    }
}
