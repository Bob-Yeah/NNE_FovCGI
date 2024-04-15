// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetworkModel.h"
//#include "../FovViewExtension.h"

TArray<FString> UNeuralNetworkModel::GetRuntimeNames()
{
	using namespace UE::NNE;

	TArray<FString> Result;
	TArrayView<TWeakInterfacePtr<INNERuntime>> Runtimes = GetAllRuntimes();
	for (int32 i = 0; i < Runtimes.Num(); i++)
	{
		UE_LOG(LogTemp, Error, TEXT("runtime '%s' found"), *Runtimes[i]->GetRuntimeName());
		if (Runtimes[i].IsValid() && Cast<INNERuntimeRDG>(Runtimes[i].Get()))
		{
			Result.Add(Runtimes[i]->GetRuntimeName());
			UE_LOG(LogTemp, Error, TEXT("runtime '%s' added"), *Runtimes[i]->GetRuntimeName());
		}
	}
	return Result;
}



UNeuralNetworkModel* UNeuralNetworkModel::CreateModel(UObject* Parent, FString RuntimeName, UNNEModelData* ModelData)
{
	using namespace UE::NNE;

	if (!ModelData)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid model data"));
		return nullptr;
	}

	TWeakInterfacePtr<INNERuntimeRDG> Runtime = GetRuntime<INNERuntimeRDG>(RuntimeName);
	if (!Runtime.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("No RDG runtime '%s' found"), *RuntimeName);
		return nullptr;
	}

	TUniquePtr<IModelRDG> UniqueModel = nullptr;

	if (Runtime->CanCreateModelRDG(ModelData)) {
		UniqueModel = Runtime->CreateModel(ModelData);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Could not create the GPU model"));
		return nullptr;
	}


	UNeuralNetworkModel* Result = NewObject<UNeuralNetworkModel>(Parent);
	if (Result)
	{
		Result->NNEModel = UniqueModel->CreateModelInstance();
		//check(Result->NNEModel.IsValid())
		return Result;
	}

	return nullptr;
}

void UNeuralNetworkModel::DestroyModel()
{
	using namespace UE::NNE;
	NNEModel = nullptr;

	{
		FovViewExtension->IsActiveThisFrameFunctions.Empty();

		FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

		IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
		{
			return TOptional<bool>(false);
		};

		FovViewExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);
	}

	FovViewExtension.Reset();
	FovViewExtension = nullptr;
}

void UNeuralNetworkModel::EnableViewExtension()
{
	UE_LOG(LogTemp, Error, TEXT("EnableViewExtension"));
	FovViewExtension = FSceneViewExtensions::NewExtension<FPostFovViewExtension>();
}
//bool UNeuralNetworkModel::CreateTensor(TArray<int32> Shape, UPARAM(ref) FNeuralNetworkTensor& Tensor)
//{
//	if (Shape.Num() == 0)
//	{
//		return false;
//	}
//
//	int32 Volume = 1;
//	for (int32 i = 0; i < Shape.Num(); i++)
//	{
//		if (Shape[i] < 1)
//		{
//			return false;
//		}
//		Volume *= Shape[i];
//	}
//
//	Tensor.Shape = Shape;
//	Tensor.Data.SetNum(Volume);
//	return true;
//}

int32 UNeuralNetworkModel::NumInputs()
{
	check(NNEModel.IsValid())
	UE_LOG(LogNNE, Warning, TEXT("The current example supports only models with %d input tensor"), NNEModel->GetInputTensorDescs().Num());
		return NNEModel->GetInputTensorDescs().Num();
}

int32 UNeuralNetworkModel::NumOutputs()
{
	check(NNEModel.IsValid())
		return NNEModel->GetOutputTensorDescs().Num();
}

//TArray<int32> UNeuralNetworkModel::GetInputShape(int32 Index)
//{
//	check(NNEModel.IsValid())
//
//		using namespace UE::NNE;
//
//	TConstArrayView<FTensorDesc> Desc = NNEModel->GetInputTensorDescs();
//	if (Index < 0 || Index >= Desc.Num())
//	{
//		return TArray<int32>();
//	}
//	TArray<int32> result = TArray<int32>(Desc[Index].GetShape().GetData());
//	for (int32 i = 0; i < result.Num(); i++)
//	{
//		UE_LOG(LogNNE, Warning, TEXT("GetInputShape %d : %d"),i , result[i]);
//	}
//	return result;
//}

//TArray<int32> UNeuralNetworkModel::GetOutputShape(int32 Index)
//{
//	check(NNEModel.IsValid())
//
//		using namespace UE::NNE;
//
//	TConstArrayView<FTensorDesc> Desc = NNEModel->GetOutputTensorDescs();
//	if (Index < 0 || Index >= Desc.Num())
//	{
//		return TArray<int32>();
//	}
//
//	return TArray<int32>(Desc[Index].GetShape().GetData());
//} 

//bool UNeuralNetworkModel::SetInputs(const TArray<FNeuralNetworkTensor>& Inputs)
//{
//	check(NNEModel.IsValid())
//
//		using namespace UE::NNE;
//
//	InputBindings.Reset();
//	InputShapes.Reset();
//
//	TConstArrayView<FTensorDesc> InputDescs = NNEModel->GetInputTensorDescs();
//	if (InputDescs.Num() != Inputs.Num())
//	{
//		UE_LOG(LogTemp, Error, TEXT("Invalid number of input tensors provided"));
//		return false;
//	}
//
//	InputBindings.SetNum(Inputs.Num());
//	InputShapes.SetNum(Inputs.Num());
//	for (int32 i = 0; i < Inputs.Num(); i++)
//	{
//		InputBindings[i].Data = (void*)Inputs[i].Data.GetData();
//		InputBindings[i].SizeInBytes = Inputs[i].Data.Num() * sizeof(float);
//		InputShapes[i] = FTensorShape::MakeFromSymbolic(FSymbolicTensorShape::Make(Inputs[i].Shape));
//	}
//
//	if (NNEModel->SetInputTensorShapes(InputShapes) != 0)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to set the input shapes"));
//		return false;
//	}
//
//	return true;
//}

//bool UNeuralNetworkModel::RunSync(UPARAM(ref) TArray<FNeuralNetworkTensor>& Outputs)
//{
//	check(NNEModel.IsValid());
//
//	using namespace UE::NNE;
//
//	TConstArrayView<FTensorDesc> OutputDescs = NNEModel->GetOutputTensorDescs();
//	if (OutputDescs.Num() != Outputs.Num())
//	{
//		UE_LOG(LogTemp, Error, TEXT("Invalid number of output tensors provided"));
//		return false;
//	}
//
//	TArray<FTensorBindingRDG> OutputBindings;
//	OutputBindings.SetNum(Outputs.Num());
//	for (int32 i = 0; i < Outputs.Num(); i++)
//	{
//		OutputBindings[i].Data = (void*)Outputs[i].Data.GetData();
//		OutputBindings[i].SizeInBytes = Outputs[i].Data.Num() * sizeof(float);
//	}
//
//	return NNEModel->RunSync(InputBindings, OutputBindings) == 0;
//}

//void UNeuralNetworkModel::SetNetworkToViewExtension() {
//	FFovViewExtension::SetNeuralNetwork(this);
//}

