// Copyright Epic Games, Inc. All Rights Reserved.

#include "NNE_FovCGIGameMode.h"
#include "NNE_FovCGICharacter.h"
#include "UObject/ConstructorHelpers.h"

ANNE_FovCGIGameMode::ANNE_FovCGIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//FovViewExtension = FSceneViewExtensions::NewExtension<FFovViewExtension>();
}
