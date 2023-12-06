// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AZ_MHW : ModuleRules
{
	public AZ_MHW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"EnhancedInput",
                "HeadMountedDisplay",
                "HTTP",
                "UMG", 
				"Sockets",
                "MoviePlayer",
                "Networking", 
				"AIModule", 
				"NavigationSystem", 
				"SlateCore", 
				"Slate" ,
                "RenderCore",
                "Projects",
                "GameplayTasks",
                "Niagara",

                "MovieScene",
                "LevelSequence",
                "ActorSequence",
                "Paper2D",
                "CinematicCamera",
                "MovieSceneTracks",
                "MediaAssets",

                "BuildPatchServices",
                "OnlineSubsystem",
                "Json",
                "JsonUtilities",
                "PakFile",
                "Landscape",

                "WebBrowserWidget",
                "AssetRegistry"
            });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "UMG"
        });

        PrivateIncludePaths.AddRange(new string[] {
            "AZ_MHW",
        });

        //PrivatePCHHeaderFile = ".h";
        
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
