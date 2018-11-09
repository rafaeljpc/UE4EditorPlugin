// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "IUE4EditorPlugin.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/Commands.h"
#include "PropertyEditorModule.h"
#include "UE4EditorDetail.h"
#include "UE4EditorPluginStyle.h"

#define LOCTEXT_NAMESPACE "FUE4EditorPlugin"

class FUE4EditorPlugin : public IUE4EditorPlugin {
protected:
	static void OnGatherExtensions(TSharedPtr<FExtender> Extender, UBlueprint* Blueprint);

	static void OpenBodyBuildDialog(UBlueprint* Blueprint);

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class FMRABodyPluginCommands : public TCommands<FMRABodyPluginCommands> {

public:
	TSharedPtr< FUICommandInfo > PluginAction;

public:

	FMRABodyPluginCommands()
		: TCommands<FMRABodyPluginCommands>(TEXT("UE4EditorPlugin"), NSLOCTEXT("Contexts", "UE4EditorPlugin", "UE4EditorPlugin Plugin"),
		NAME_None, FUE4EditorPluginStyle::GetStyleSetName()) {
	}

	// TCommands<> interface
	virtual void RegisterCommands() override {
		UI_COMMAND(PluginAction, "Plugin Action", "", EUserInterfaceActionType::Button, FInputGesture());
	}
};

IMPLEMENT_MODULE(FUE4EditorPlugin, UE4EditorPlugin)

void FUE4EditorPlugin::StartupModule() {
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)

	FUE4EditorPluginStyle::Initialize();
	FUE4EditorPluginStyle::ReloadTextures();

	FMRABodyPluginCommands::Register();

	FBlueprintEditorModule& BPEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BPEditorModule.OnGatherBlueprintMenuExtensions().AddStatic(&FUE4EditorPlugin::OnGatherExtensions);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
		PropertyModule.RegisterCustomClassLayout("SceneComponent", FOnGetDetailCustomizationInstance::CreateStatic(&FUE4EditorDetail::MakeInstance));
	}
}


void FUE4EditorPlugin::ShutdownModule() {
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FUE4EditorPluginStyle::Shutdown();

	FMRABodyPluginCommands::Unregister();

}

void FUE4EditorPlugin::OnGatherExtensions(TSharedPtr<FExtender> Extender, UBlueprint * Blueprint) {
	if (Blueprint == nullptr)
		return;
	if (Blueprint->ParentClass == nullptr)
		return;
	if (Blueprint->ParentClass->IsChildOf(AActor::StaticClass()) == false)
		return;

	TSharedPtr<FUICommandList> PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMRABodyPluginCommands::Get().PluginAction,
		FExecuteAction::CreateStatic(&FUE4EditorPlugin::OpenBodyBuildDialog, Blueprint),
		FCanExecuteAction());

	Extender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands,
			FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& Builder) {
			Builder.AddToolBarButton(FMRABodyPluginCommands::Get().PluginAction);
		}));
}

void FUE4EditorPlugin::OpenBodyBuildDialog(UBlueprint* Blueprint) {
	TSharedPtr<SWindow> Window;
	/*TSharedPtr<SMRABodyBuilderDialog> Dialog;

	Window = SNew(SWindow)
		.Title(LOCTEXT("MRBodyBuilder.Title", "MR BodyBuilder"))
		.SizingRule(ESizingRule::Autosized)
		.SupportsMaximize(false).SupportsMinimize(false)
		[
			SNew(SBorder)
			.Padding(FMargin(10))
		[
			SNew(SMRABodyBuilderDialog)
			.Blueprint(Blueprint)
		]
		];

	FSlateApplication::Get().AddWindow(Window.ToSharedRef());*/
}

#undef LOCTEXT_NAMESPACE