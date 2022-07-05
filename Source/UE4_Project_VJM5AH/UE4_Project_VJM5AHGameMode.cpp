// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4_Project_VJM5AHGameMode.h"
#include "UE4_Project_VJM5AHHUD.h"
#include "UE4_Project_VJM5AHCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4_Project_VJM5AHGameMode::AUE4_Project_VJM5AHGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUE4_Project_VJM5AHHUD::StaticClass();
}
