#include "PostFovViewExtension.h"
#include "../NNE_FovCGI.h"

namespace
{
	TAutoConsoleVariable<int32> CVarPostFovExtensionOn(
		TEXT("r.SceneViewExtensionTemplate"),
		0,
		TEXT("Enable Custom SceneViewExtension \n")
		TEXT(" 0: OFF;")
		TEXT(" 1: ON."),
		ECVF_RenderThreadSafe);
}


FPostFovViewExtension::FPostFovViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
	UE_LOG(LogTemp, Error, TEXT("SceneViewExtensionTemplate: Custom SceneViewExtension registered"));
}

void FPostFovViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass PassId, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	// Define to what Post Processing stage to hook the SceneViewExtension into. See SceneViewExtension.h and PostProcessing.cpp for more info
	if (PassId == EPostProcessingPass::MotionBlur)
	{
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FPostFovViewExtension::CustomPostProcessing));
	}
}

FScreenPassTexture FPostFovViewExtension::CustomPostProcessing(FRDGBuilder& GraphBuilder, const FSceneView& SceneView, const FPostProcessMaterialInputs& Inputs)
{
	// SceneViewExtension gives SceneView, not ViewInfo so we need to setup some basics
	const FSceneViewFamily& ViewFamily = *SceneView.Family;
	const ERHIFeatureLevel::Type FeatureLevel = SceneView.GetFeatureLevel();

	const FScreenPassTexture& SceneColor = Inputs.Textures[(uint32)EPostProcessMaterialInput::SceneColor];

	if (!SceneColor.IsValid() || CVarPostFovExtensionOn.GetValueOnRenderThread() == 0)
	{
		return SceneColor;
	}
	return SceneColor;
}