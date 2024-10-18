#include "Components/MyInputComponent.h"


UMyInputComponent::UMyInputComponent()
{
	//No movement
	MovementInput = FVector2D::ZeroVector;
	LookInput = FVector2D::ZeroVector;
}


