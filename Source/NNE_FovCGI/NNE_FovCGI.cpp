// Copyright Epic Games, Inc. All Rights Reserved.

#include "NNE_FovCGI.h"
#include "Modules/ModuleManager.h"


IMPLEMENT_PRIMARY_GAME_MODULE(NNE_FovCGIModule, NNE_FovCGI, "NNE_FovCGI");
void NNE_FovCGIModule::StartupModule()
{
	UE_LOG(LogTemp, Error, TEXT("NNE_FovCGI Startup"));
	//FovViewExtension = FSceneViewExtensions::NewExtension<FPostFovViewExtension>();
}