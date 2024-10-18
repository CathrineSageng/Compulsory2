
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/MyCharacter.h"
#include "MyInteractionSystem.generated.h"

UCLASS()
class COMPULSORY2_API UMyInteractionSystem : public UObject
{
	GENERATED_BODY()
	
public:
	void ProcessInteractions(AMyCharacter* PlayerCharacter);
};
