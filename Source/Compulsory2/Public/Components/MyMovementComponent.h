
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMovementComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector2D MovementInput;
};