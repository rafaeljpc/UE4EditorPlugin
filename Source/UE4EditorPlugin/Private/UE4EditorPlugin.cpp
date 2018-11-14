// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4EditorPlugin.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/Commands.h"
#include "PropertyEditorModule.h"
#include "UE4EditorDetail.h"
#include "UE4EditorPluginStyle.h"
#include "AddMeshDialog.h"

#define LOCTEXT_NAMESPACE "FUE4EditorPlugin"

class FUE4EditorPlugin : public IUE4EditorPlugin {
protected:
	static void OnGatherExtensions(TSharedPtr<FExtender> Extender, UBlueprint* Blueprint);

	static void OpenAddMeshDialog(UBlueprint* Blueprint);

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class FUE4PluginCommands : public TCommands<FUE4PluginCommands> {

public:
	TSharedPtr< FUICommandInfo > PluginAction;

public:

	FUE4PluginCommands()
		: TCommands<FUE4PluginCommands>(TEXT("UE4EditorPlugin"), NSLOCTEXT("Contexts", "UE4EditorPlugin", "UE4EditorPlugin Plugin"),
		NAME_None, FUE4EditorPluginStyle::GetStyleSetName()) {
	}

	// TCommands<> interface
	virtual void RegisterCommands() override {
		UI_COMMAND(PluginAction, "Add Mesh", "Add Mesh X times", EUserInterfaceActionType::Button, FInputGesture());
	}
};

void FUE4EditorPlugin::StartupModule() {
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)

	FUE4EditorPluginStyle::Initialize();
	FUE4EditorPluginStyle::ReloadTextures();

	FUE4PluginCommands::Register();

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

	FUE4PluginCommands::Unregister();

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
		FUE4PluginCommands::Get().PluginAction,
		FExecuteAction::CreateStatic(&FUE4EditorPlugin::OpenAddMeshDialog, Blueprint),
		FCanExecuteAction());

	Extender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands,
			FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& Builder) {
			Builder.AddToolBarButton(FUE4PluginCommands::Get().PluginAction);
		}));
}

void FUE4EditorPlugin::OpenAddMeshDialog(UBlueprint* Blueprint) {
	UE_LOG(LogTemp, Log, TEXT("FUE4EditorPlugin::OpenAddMeshDialog"));
	TSharedPtr<SWindow> Window;
	
	Window = SNew(SWindow)
		.Title(LOCTEXT("AddMeshDialog.Title", "Add Mesh X Times"))
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2D(500.0f, 300.0f))
		.IsTopmostWindow(true)
		.SupportsMaximize(false).SupportsMinimize(false)
		[
			SNew(SBorder)
			.Padding(FMargin(10))
		[
			SNew(SAddMeshDialog)
			.Blueprint(Blueprint)
		]
		];

	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
}

IMPLEMENT_MODULE(FUE4EditorPlugin, UE4EditorPlugin)

#undef LOCTEXT_NAMESPACE