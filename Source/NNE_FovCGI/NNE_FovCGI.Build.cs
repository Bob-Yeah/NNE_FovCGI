// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NNE_FovCGI : ModuleRules
{
	public NNE_FovCGI(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.AddRange(
		   new string[] {
				Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Private"),
		   }
	   );

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"NNE", "Renderer","RenderCore", "RHI", "RHICore","D3D12RHI",
			"OpenCV","OpenCVHelper" });

		PrivateDependencyModuleNames.AddRange(
							new string[]
							{
										"Core",
										"CoreUObject",
										"Engine",
										"Renderer",
										"RenderCore",
										"RHI",
										"RHICore",
										"D3D12RHI",
										"OpenCV",
										"OpenCVHelper" 
								// ... add private dependencies that you statically link with here ...  
							}
						);
	}
}
