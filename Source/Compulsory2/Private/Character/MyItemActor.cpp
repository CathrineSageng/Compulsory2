// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyItemActor.h"
#include "Components/MyItemComponent.h"
#include "Systems/MyItemSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyItemActor::AMyItemActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create the collision capsule and set it as the root component
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    CollisionCapsule->InitCapsuleSize(50.f, 50.f); 
    RootComponent = CollisionCapsule;

    // Create the static mesh component and attach it to the capsule
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(CollisionCapsule);


    // Create and attach the item component
    ItemComponent = CreateDefaultSubobject<UMyItemComponent>(TEXT("ItemComponent"));
}

