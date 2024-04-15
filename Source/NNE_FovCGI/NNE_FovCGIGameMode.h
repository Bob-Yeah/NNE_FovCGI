// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FovViewExtension.h"
#include "NNE_FovCGIGameMode.generated.h"

UCLASS(minimalapi)
class ANNE_FovCGIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANNE_FovCGIGameMode();

//protected:
//	TSharedPtr<FFovViewExtension, ESPMode::ThreadSafe> FovViewExtension;
};



