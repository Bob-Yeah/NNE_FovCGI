// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "FovViewExtension.h"
#include "RenderGraphUtils.h"
#include <ViewExtensionManager.h>
#include "Public/PostFovViewExtension.h"

class NNE_FovCGIModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

//protected:
//	TSharedPtr<FPostFovViewExtension, ESPMode::ThreadSafe> FovViewExtension;
};

