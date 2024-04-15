#pragma once
#include "CoreMinimal.h"
#include "RenderGraphUtils.h"
#include "SceneViewExtension.h"
#include "PostProcess/PostProcessing.h"
#include "PostProcess/PostProcessMaterial.h"

class FPostFovViewExtension : public FSceneViewExtensionBase
{
public:
	FPostFovViewExtension(const FAutoRegister& AutoRegister);

public:
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {};
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {};

	// See SceneViewExtension.h for hooks to different stages of rendering
	// f.ex. PrePostProcessPass_RenderThread happens just when rendering is finished but PostProcessing hasn't started yet

	// This is the method to hook into PostProcessing pass
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass PassId, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled);

	// This is our actual processing function
	FScreenPassTexture CustomPostProcessing(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs);

};

