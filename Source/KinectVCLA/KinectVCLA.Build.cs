// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class KinectVCLA : ModuleRules
{
    public KinectVCLA(TargetInfo Target)
    {
        //Type = ModuleType.External;

        PublicIncludePaths.AddRange(
                new string[] {
                    "KinectVCLA/Public",
					// ... add public include paths required here ...
				}
                );

        PrivateIncludePaths.AddRange(
            new string[] {
                    "KinectVCLA/Private",

                // ... add other private include paths required here ...
            }
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {

                    "CoreUObject",
                    "Core",
                    "Engine",
                    "InputDevice",
                    //"MSSpeech",
                    "InputCore"
                // ... add other public dependencies that you statically link with here ...
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                    "CoreUObject",
                    "Core",
                    "Engine",
                    "InputDevice"
                // ... add private dependencies that you statically link with here ...
            }
            );

        string SDKDIR = Utils.ResolveEnvironmentVariable("%KINECTSDK20_DIR%");

        SDKDIR = SDKDIR.Replace("\\", "/");

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));


            string ShortPlatform = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            PublicIncludePaths.Add(SDKDIR + "inc/");

            string PlatformPath = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64/" : "x86/";

            string LibPath = SDKDIR + "Lib/" + PlatformPath;

            PublicLibraryPaths.Add(LibPath);
            PublicAdditionalLibraries.AddRange(new string[] { "Kinect20.lib" });

            PublicDelayLoadDLLs.AddRange(new string[] {"Kinect20.dll"});

        }
    }
}
