// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlayerControl : ModuleRules
{
	public PlayerControl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","Sockets","Networking" });
	}
}
