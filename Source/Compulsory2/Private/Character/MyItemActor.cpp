
#include "Character/MyItemActor.h"
#include "Components/MyItemComponent.h"
#include "Systems/MyItemSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"


AMyItemActor::AMyItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    CollisionCapsule->InitCapsuleSize(50.f, 50.f); 
    RootComponent = CollisionCapsule;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(CollisionCapsule);

    ItemComponent = CreateDefaultSubobject<UMyItemComponent>(TEXT("ItemComponent"));
}

