
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyItemActor.generated.h"

class UMyItemComponent;
class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class COMPULSORY2_API AMyItemActor : public AActor
{
	GENERATED_BODY()

public:
	AMyItemActor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyItemComponent* ItemComponent;


};

