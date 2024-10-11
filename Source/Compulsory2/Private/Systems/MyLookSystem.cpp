#include "Systems/MyLookSystem.h"
#include "GameFramework/PlayerController.h"
#include "Components/MyLookComponent.h"

void UMyLookSystem::Execute(AController* Controller, UMyLookComponent* LookComponent)
{
    if (LookComponent)
    {
        // Cast Controller to APlayerController
        if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
        {
            PlayerController->AddPitchInput(LookComponent->LookInput.Y);
            PlayerController->AddYawInput(LookComponent->LookInput.X);
        }
    }
}
