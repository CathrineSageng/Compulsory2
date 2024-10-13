// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AMyItemActor();

protected:

	// The item’s mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	// Collision capsule for detecting overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyItemComponent* ItemComponent;


};

