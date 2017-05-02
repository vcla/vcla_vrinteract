// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class PluginPlayground : ModuleRules
{
	public PluginPlayground(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "KinectVCLA", "HeadMountedDisplay", "LeapMotion" });

        //PrivateDependencyModuleNames.AddRange(new string[] { "KinectVCLA" });

        PublicIncludePathModuleNames.AddRange(new string[] { "KinectVCLA", "LeapMotion" });
       // DynamicallyLoadedModuleNames.Add("KinectVCLA");


        PublicIncludePaths.AddRange(new string[] { "KinectVCLA/Classes", "LeapMotion/Public" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }

    }
}
