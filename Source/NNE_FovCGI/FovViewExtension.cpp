#include "FovViewExtension.h"
#include "Modules/ModuleManager.h"
#include "PostProcess/PostProcessMaterialInputs.h"
#include "PostProcess/PostProcessMaterial.h"
#include "PostProcess/SceneRenderTargets.h"

#include "Public/NeuralNetworkModel.h"

#include "PreOpenCVHeaders.h"
#include "OpenCVHelper.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core.hpp"
#include "PostOpenCVHeaders.h"
#include <vector>

#include "Math/PackedVector.h"
//
//using namespace std;
//using namespace cv;
//
//
//DEFINE_LOG_CATEGORY_STATIC(LogNNEFov, Log, All);
//
//namespace NNE_FovCGI
//{
//	static int32 IsActive = 0;
//	static FAutoConsoleVariableRef CVarFovNNEIsActive(
//		TEXT("r.FovNNEIsActive"),
//		IsActive,
//		TEXT("Allows an additional rendering pass that will apply a nn to the frame.\n")
//		TEXT("=0:off (default), >0: on"),
//		ECVF_Cheat | ECVF_RenderThreadSafe);
//}
//
//FFovViewExtension::FFovViewExtension(const FAutoRegister& AutoRegister)
//	: FSceneViewExtensionBase(AutoRegister)
//{
//	ViewExtensionIsActive = GDynamicRHI->GetName() == FString(TEXT("D3D12"));
//}
//
//UNeuralNetworkModel* FFovViewExtension::myNetwork = nullptr;
//
////------------------------------------------------------------------------------
//void FFovViewExtension::SetNeuralNetwork(UNeuralNetworkModel* Model)
//{
//	myNetwork = Model;
//}
//
//
//bool FFovViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
//{
//	//UE_LOG(LogNNEFov, Warning, TEXT("IsActiveThisFrame_Internal"));
//	return ViewExtensionIsActive;
//}
//
//void FFovViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass PassId, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
//{
//	//UE_LOG(LogNNEFov, Warning, TEXT("SubscribeToPostProcessingPass"));
//	if (!NNE_FovCGI::IsActive)
//	{
//		return;
//	}
//
//	if (!bIsPassEnabled)
//	{
//		return;
//	}
//
//	if (PassId == EPostProcessingPass::Tonemap) {
//		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FFovViewExtension::AfterTonemap_RenderThread));
//	}
//	UE_LOG(LogNNEFov, Warning, TEXT("SubscribeToPostProcessingPass"));
//}
//
//FScreenPassTexture FFovViewExtension::AfterTonemap_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs)
//{
//	RDG_EVENT_SCOPE(GraphBuilder, "FovViewExt_AfterTonemap");
//	return ApplyNeuralNetwork(GraphBuilder, View, InOutInputs, FString::Printf(TEXT("After%02dTonemap"), EPostProcessingPass::Tonemap));
//}
//
//FScreenPassTexture FFovViewExtension::ApplyNeuralNetwork(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs, const FString& DDSFileName)
//{
//	FRDGTextureRef SaveTexture = nullptr;
//	FScreenPassTexture ReturnTexture;
//
//	if (InOutInputs.OverrideOutput.IsValid())
//	{
//		SaveTexture = InOutInputs.OverrideOutput.Texture;
//		ReturnTexture = InOutInputs.OverrideOutput;
//	}
//	else
//	{
//		SaveTexture = InOutInputs.Textures[(uint32)EPostProcessMaterialInput::SceneColor].Texture;
//		ReturnTexture = const_cast<FScreenPassTexture&>(InOutInputs.Textures[(uint32)EPostProcessMaterialInput::SceneColor]);
//	}
//
//	AddNNPass_RenderThread(GraphBuilder, SaveTexture);
//
//	return ReturnTexture;
//}
//
//BEGIN_SHADER_PARAMETER_STRUCT(FFovNNPassParameters, )
//RDG_TEXTURE_ACCESS(Source, ERHIAccess::CPURead)
//END_SHADER_PARAMETER_STRUCT()
//
//void FFovViewExtension::AddNNPass_RenderThread(
//	FRDGBuilder& GraphBuilder,
//	FRDGTextureRef SourceTexture)
//{
//	if (SourceTexture == nullptr)
//	{
//		UE_LOG(LogNNEFov, Warning, TEXT("Skipping null texture"));
//		return;
//	}
//
//	FFovNNPassParameters* Parameters = GraphBuilder.AllocParameters<FFovNNPassParameters>();
//	Parameters->Source = SourceTexture;
//
//	GraphBuilder.AddPass(
//		RDG_EVENT_NAME("FovRenderNN"),
//		Parameters,
//		ERDGPassFlags::Readback,
//		[this, SourceTexture](FRHICommandListImmediate& RHICmdList) {
//			if (myNetwork == nullptr)
//			{
//				return;
//			}
//			FRHITexture2D* Texture = SourceTexture->GetRHI()->GetTexture2D();
//			Width = Texture->GetSizeX();
//			Height = Texture->GetSizeY();
//			CopyTextureFromGPUToCPU(RHICmdList, Texture);
//			ResizeScreenImageToMatchModel();
//			ApplyNeuralNetwork();
//			ResizeModelImageToMatchScreen();
//			CopyTextureFromCPUToGPU(RHICmdList, Texture);
//		});
//
//	UE_LOG(LogNNEFov, Warning, TEXT("AddNNPass_RenderThread"));
//}
//
//void FFovViewExtension::CopyTextureFromGPUToCPU(FRHICommandListImmediate& RHICmdList, FRHITexture2D* Texture)
//{
//	const int PixelCount = Width * Height;
//
//	struct FReadSurfaceContext
//	{
//		FRHITexture* BackBuffer;
//		TArray<FColor>& OutData;
//		FIntRect Rect;
//		FReadSurfaceDataFlags Flags;
//	};
//
//	const FReadSurfaceContext readSurfaceContext = {
//		Texture,
//		RawImage,
//		FIntRect(0, 0, Width, Height),
//		FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX)
//	};
//
//	RHICmdList.ReadSurfaceData(
//		readSurfaceContext.BackBuffer,
//		readSurfaceContext.Rect,
//		readSurfaceContext.OutData,
//		readSurfaceContext.Flags);
//
//	InputImageCPU.Reset();
//	InputImageCPU.Reserve(PixelCount * 3);
//
//	for (int i = 0; i < RawImage.Num(); i++)
//	{
//		const FColor& Pixel = RawImage[i];
//		InputImageCPU.Add(Pixel.R);
//		InputImageCPU.Add(Pixel.G);
//		InputImageCPU.Add(Pixel.B);
//	}
//}
//
////------------------------------------------------------------------------------
//void FFovViewExtension::CopyTextureFromCPUToGPU(FRHICommandListImmediate& RHICmdList, FRHITexture2D* Texture)
//{
//	const FUpdateTextureRegion2D TextureRegion2D(0, 0, 0, 0, Width, Height);
//	RHICmdList.UpdateTexture2D(Texture, 0, TextureRegion2D, Width * 4, (const uint8*)EnhancedImageCPU.GetData());
//}
//
//
////------------------------------------------------------------------------------
//void FFovViewExtension::ResizeScreenImageToMatchModel()
//{
//	// Create image from StylizedImage object
//	cv::Mat inputImage(Height, Width, CV_8UC3, InputImageCPU.GetData());
//
//	// Create image to resize for inferencing
//	cv::Mat outputImage(224, 224, CV_8UC3);
//
//	// Resize to outputImage
//	cv::resize(inputImage, outputImage, cv::Size(224, 224));
//
//	// Reshape to 1D
//	outputImage = outputImage.reshape(1, 1);
//
//	// uint_8, [0, 255] -> float, [0, 1]
//	std::vector<float> vec;
//	outputImage.convertTo(vec, CV_32FC1, 1. / 255);
//
//	// Height, Width, Channel to Channel, Height, Width
//	const int inputSize = 224 * 224 * 3;
//	//ModelInputImage.Reset();
//	//ModelInputImage.Reserve(inputSize);
//	//for (size_t ch = 0; ch < 3; ++ch) {
//	//	for (size_t i = ch; i < inputSize; i += 3) {
//	//		ModelInputImage.Add(vec[i]);
//	//	}
//	//}
//	TArray<FNeuralNetworkTensor> Inputs_nnm;
//	Inputs_nnm.Reset();
//	Inputs_nnm.SetNum(myNetwork->NumOutputs());
//	
//	for (int32 k = 0; k < Inputs_nnm.Num(); k++) {
//		Inputs_nnm[k].Data.Reset();
//		Inputs_nnm[k].Data.Reserve(inputSize);
//		for (size_t ch = 0; ch < 3; ++ch) {
//			for (size_t i = ch; i < inputSize; i += 3) {
//				Inputs_nnm[k].Data.Add(vec[i]);
//			}
//		}
//		myNetwork->CreateTensor(myNetwork->GetInputShape(k), Inputs_nnm[k]);
//	}
//	myNetwork->SetInputs(Inputs_nnm);
//}
//
////------------------------------------------------------------------------------
//void FFovViewExtension::ResizeModelImageToMatchScreen()
//{
//	if (ModelOutputImage.Num() == 0)
//	{
//		return;
//	}
//
//	cv::Mat resultImage(224, 224, CV_8UC3, ModelOutputImage.GetData());
//	//cv::Mat resultoutputImage(Height, Width, CV_8UC3);
//	cv::resize(resultImage, resultImage, cv::Size(Width, Height));
//
//	//cv::imwrite("C:/code/ue5-onnxruntime/FPStyleTransfer/Content/resultImage.png", resultImage);
//
//	const uint8* RawPixel = resultImage.data;
//	const int PixelCount = Width * Height;
//	EnhancedImageCPU.Reset();
//	EnhancedImageCPU.Reserve(PixelCount);
//	int y = 0;
//	for (int i = 0; i < PixelCount; i++)
//	{
//		uint32 R, G, B;
//		R = RawPixel[y++];
//		G = RawPixel[y++];
//		B = RawPixel[y++];
//		uint32 color = (R << 22) | (G << 12) | (B << 2) | 3;
//		EnhancedImageCPU.Add(color);
//	}
//}
//
//uint8 FloatToColor(float value) {
//	return static_cast<uint8>(FMath::Clamp(value, 0, 255));
//}
//
////------------------------------------------------------------------------------
//void FFovViewExtension::ApplyNeuralNetwork() {
//
//	// create network and run model
//	//ModelOutputImage.Reset();
//	TArray<FNeuralNetworkTensor> Outputs;
//	Outputs.Reset();
//	Outputs.SetNum(myNetwork->NumOutputs());
//	for (int32 i = 0; i < Outputs.Num(); i++) {
//		myNetwork->CreateTensor(myNetwork->GetOutputShape(i),Outputs[i]);
//	}
//	myNetwork->RunSync(Outputs);
//	
//	// Create array of the correct pixel values from results
//	ModelOutputImage.Reset();
//	int channelStride = Outputs[0].Data.Num() / 3;
//
//	ModelOutputImage.Reserve(channelStride * 4);
//
//	for (size_t i = 0; i < channelStride; i++) {
//		ModelOutputImage.Add(FloatToColor(Outputs[0].Data[channelStride * 2 + i])); // B
//		ModelOutputImage.Add(FloatToColor(Outputs[0].Data[channelStride + i])); // G
//		ModelOutputImage.Add(FloatToColor(Outputs[0].Data[i]));					// R
//	}
//}
