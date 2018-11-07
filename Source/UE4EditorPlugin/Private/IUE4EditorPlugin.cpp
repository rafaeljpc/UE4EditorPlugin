// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "IUE4EditorPlugin.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "UE4EditorDetail.h"



class FUE4EditorPlugin : public IUE4EditorPlugin {
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FUE4EditorPlugin, UE4EditorPlugin)



void FUE4EditorPlugin::StartupModule() {
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
		PropertyModule.RegisterCustomClassLayout("SceneComponent", FOnGetDetailCustomizationInstance::CreateStatic(&FUE4EditorDetail::MakeInstance));
	}
}


void FUE4EditorPlugin::ShutdownModule() {
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



