#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyLookComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMPULSORY2_API UMyLookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyLookComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look")
	FVector2D LookInput;
};
