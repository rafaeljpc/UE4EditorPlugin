#include "AddMeshFunctionLibrary.h"
#include "KismetEditorUtilities.h"
#include "BlueprintEditor.h"
#include "SSCSEditor.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "UAddMeshFunctionLibrary"

void UAddMeshFunctionLibrary::AddNMeshes(UBlueprint* Blueprint, FString ComponentTemplateName, UStaticMesh* StaticMesh, 
	int32 Repetitions, FVector LocationStep, FRotator RotationStep) {
	UE_LOG(LogTemp, Log, TEXT("UAddMeshFunctionLibrary.AddNMeshes"));

	UE_LOG(LogTemp, Log, TEXT("Blueprint = %s"), *Blueprint->GetFullName());
	UE_LOG(LogTemp, Log, TEXT("ComponentTemplateName = %s"), *ComponentTemplateName);
	UE_LOG(LogTemp, Log, TEXT("StaticMesh = %s"), *StaticMesh->GetPathName());
	UE_LOG(LogTemp, Log, TEXT("Repetitions = %d"), Repetitions);
	UE_LOG(LogTemp, Log, TEXT("LocationStep = %s"), *LocationStep.ToString());
	UE_LOG(LogTemp, Log, TEXT("RotationStep = %s"), *RotationStep.ToString());

	TSharedPtr<IBlueprintEditor> BPEditor = FKismetEditorUtilities::GetIBlueprintEditorForObject(Blueprint, false);
	if (!BPEditor->IsBlueprintEditor())
		return;

	FBlueprintEditor* FBPEditor = (FBlueprintEditor*) BPEditor.Get();
	if (!FBPEditor)
		return;

	TSharedPtr<SSCSEditor> SSCSEditor = FBPEditor->GetSCSEditor();

	const FScopedTransaction Transaction(LOCTEXT("AddMeshFunctionLibrary", "AddNMeshes"));

	check(Blueprint != nullptr && Blueprint->SimpleConstructionScript != nullptr);

	Blueprint->Modify();
	SSCSEditor->SaveSCSCurrentState(Blueprint->SimpleConstructionScript);

	if (SSCSEditor->GetSelectedNodes().Num() == 0)
		SSCSEditor->SelectRoot();

	auto TreeNode = SSCSEditor->GetSelectedNodes()[0];

	USCS_Node* ImportNode = Blueprint->SimpleConstructionScript->CreateNode(USceneComponent::StaticClass(), TEXT("ImportNode"));

	for (int32 i = 0; i < Repetitions; i++) {
		FString CompName = FString::Printf(TEXT("%s_%d"), *ComponentTemplateName, i + 1);

		USCS_Node* MeshNode = Blueprint->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), *CompName);
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(MeshNode->ComponentTemplate);

		MeshComponent->SetStaticMesh(StaticMesh);

		ImportNode->AddChildNode(MeshNode);
	}

	UActorComponent* Component = SSCSEditor->AddNewNode(ImportNode, nullptr, true, false);
}

#undef LOCTEXT_NAMESPACE