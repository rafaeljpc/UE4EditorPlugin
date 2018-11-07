// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class UE4EditorPlugin : ModuleRules
    {
        public UE4EditorPlugin(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicIncludePaths.AddRange(
                new string[] {
                    "UE4EditorPlugin/Public"
                }
                );

            PrivateIncludePaths.AddRange(
                new string[] {
                    "UE4EditorPlugin/Private"
                }
                );

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "Slate",
                    "UnrealEd",
                    "UE4EditorPlugin"
                }
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "Projects",
                    "InputCore",
                    "UnrealEd",
                    "LevelEditor",
                    "GraphEditor",
                    "EditorStyle",
                    "EditorWidgets",
                    "Kismet",
                    "PropertyEditor",
                    "CoreUObject",
                    "Engine",
                    "Slate",
                    "SlateCore",
                    "BlueprintGraph",
                    "EditorStyle",
                }
                );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
					// ... add any modules that your module loads dynamically here ...
				}
                );
        }
    }
}
