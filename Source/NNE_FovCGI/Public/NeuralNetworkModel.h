// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "NNE.h"
#include "NNERuntimeRDG.h"
#include "NNEModelData.h"
#include "PostFovViewExtension.h"
#include "NeuralNetworkModel.generated.h"


USTRUCT(BlueprintType, Category = "NNEFovCGI")
struct FNeuralNetworkTensor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "NNEFovCGI")
		TArray<int32> Shape = TArray<int32>();

	UPROPERTY(BlueprintReadWrite, Category = "NNEFovCGI")
		TArray<float> Data = TArray<float>();
};

UCLASS(BlueprintType, Category = "NNEFovCGI")
class NNE_FOVCGI_API UNeuralNetworkModel : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		static TArray<FString> GetRuntimeNames();

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		static UNeuralNetworkModel* CreateModel(UObject* Parent, FString RuntimeName, UNNEModelData* ModelData);

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	static bool CreateTensor(TArray<int32> Shape, UPARAM(ref) FNeuralNetworkTensor& Tensor);

public:

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		int32 NumInputs();

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		int32 NumOutputs();

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	TArray<int32> GetInputShape(int32 Index);

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	TArray<int32> GetOutputShape(int32 Index);

public:

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		void DestroyModel();

	UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
		void EnableViewExtension();

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	bool SetInputs(const TArray<FNeuralNetworkTensor>& Inputs);

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	bool RunSync(UPARAM(ref) TArray<FNeuralNetworkTensor>& Outputs);

	//UFUNCTION(BlueprintCallable, Category = "NNEFovCGI")
	//	void SetNetworkToViewExtension();

private:
	TSharedPtr<FPostFovViewExtension, ESPMode::ThreadSafe> FovViewExtension;
	TUniquePtr<UE::NNE::IModelInstanceRDG> NNEModel;
	TArray<UE::NNE::FTensorBindingRDG> InputBindings;
	TArray<UE::NNE::FTensorShape> InputShapes;

};
