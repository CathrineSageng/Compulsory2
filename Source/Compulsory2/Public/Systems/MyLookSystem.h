
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyLookSystem.generated.h"

class UMyLookComponent;

UCLASS()
class COMPULSORY2_API UMyLookSystem : public UObject
{
	GENERATED_BODY()

public:
	void Execute(AController* Controller, UMyLookComponent* LookComponent);

};
