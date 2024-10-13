// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyItemSystem.generated.h"

class UMyItemComponent;
class AMyCharacter;

UCLASS()
class COMPULSORY2_API UMyItemSystem : public UObject
{
	GENERATED_BODY()

public:
	void CheckForPickup(ACharacter* Character, UMyItemComponent* ItemComponent);

};
