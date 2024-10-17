// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHealthWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPULSORY2_API UCharacterHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void UpdateHealth(int32 CurrentHealth, int32 MaxHealth);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;
};
