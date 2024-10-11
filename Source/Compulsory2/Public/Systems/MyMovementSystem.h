
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyMovementSystem.generated.h"

class UMyMovementComponent;
class AMyCharacter;

UCLASS()
class COMPULSORY2_API UMyMovementSystem : public UObject
{
	GENERATED_BODY()

public:
	void Execute(AMyCharacter* Character, UMyMovementComponent* MovementComponent);
	
};
