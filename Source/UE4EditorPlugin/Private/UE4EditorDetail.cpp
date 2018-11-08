#include "UE4EditorDetail.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "STextBlock.h"
#include "DeclarativeSyntaxSupport.h"
#include "SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Components/SceneComponent.h"
#include "KismetEditorUtilities.h"
#include "BlueprintEditor.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Kismet2/ComponentEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"

#define LOCTEXT_NAMESPACE "UE4EditorDetail"

TSharedRef<IDetailCustomization> FUE4EditorDetail::MakeInstance() {
	return MakeShareable(new FUE4EditorDetail);
}

void FUE4EditorDetail::CustomizeDetails(IDetailLayoutBuilder & DetailBuilder) {
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory(TEXT("Rename Components Commands"));
	
	FDetailWidgetRow& Row = Category.AddCustomRow(FText::FromString("Rename Component"));

	Row.NameContent()[
			SAssignNew(RenameTemplateTextBox, SEditableTextBox)
				.ToolTipText(LOCTEXT("RenameComponentLabel_Tooltip", "Template of the Name of the components [NAME]_[index]"))
		]
		.ValueContent()[
			SNew(SButton)
				.Text(FText::FromString("Rename Component"))
				.OnClicked(FOnClicked::CreateLambda([this, &DetailBuilder]() {					
					FString TemplateName(RenameTemplateTextBox->GetText().ToString());
					return RenameComponent(&DetailBuilder, TemplateName);
				}))
		];
}

FReply FUE4EditorDetail::RenameComponent(IDetailLayoutBuilder * DetailBuilder, FString TemplateName) {
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder->GetObjectsBeingCustomized(ObjectsBeingCustomized);

	TArray<USceneComponent*> ComponentsBeingRenamed;
	for (auto& Element : ObjectsBeingCustomized) {
		if (Element.Get()->IsA(USceneComponent::StaticClass())) {
			ComponentsBeingRenamed.Add(Cast<USceneComponent>(Element.Get()));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Rename Clicked RenameTo=%s Count=%d"), *TemplateName, ObjectsBeingCustomized.Num());

	if (ComponentsBeingRenamed.Num() <= 0)
		return FReply::Unhandled();

	USceneComponent* FirstComponent = ComponentsBeingRenamed[0];

	TSharedPtr<IBlueprintEditor> BPEditor = FKismetEditorUtilities::GetIBlueprintEditorForObject(FirstComponent, false);
	if (!BPEditor->IsBlueprintEditor())
		return FReply::Unhandled();

	FBlueprintEditor* FBPEditor = (FBlueprintEditor*)BPEditor.Get();
	if (!FBPEditor)
		return FReply::Unhandled();

	TSharedPtr<SSCSEditor> SSCSEditor = FBPEditor->GetSCSEditor();
	UBlueprint* Blueprint = FBPEditor->GetBlueprintObj();

	Blueprint->Modify();
	SSCSEditor->SaveSCSCurrentState(Blueprint->SimpleConstructionScript);

	int Index = 0;
	for (auto& Component : ComponentsBeingRenamed) {
		auto Node = SSCSEditor->GetNodeFromActorComponent(Component, false);
		FString NewName = FString::Printf(TEXT("%s_%d"), *TemplateName, Index);
		
		if (!Node->CanRename())
			break;

		if (!CheckNewName(Blueprint, Node, NewName))
			break;

		FName OldName = Node->GetSCSNode()->GetVariableName();

		UE_LOG(LogTemp, Log, TEXT("Renaming %s to %s"), *OldName.ToString(), *NewName);
		Node->GetSCSNode()->SetVariableName(FName(*NewName));

		Index++;
	}


	return FReply::Handled();
}

bool FUE4EditorDetail::CheckNewName(UBlueprint* Blueprint, FSCSEditorTreeNodePtrType NodePtr, FString NewName) {
	AActor* ExistingNameSearchScope = NodePtr->GetComponentTemplate()->GetOwner();
	if ((ExistingNameSearchScope == nullptr) && (Blueprint != nullptr)) {
		ExistingNameSearchScope = Cast<AActor>(Blueprint->GeneratedClass->GetDefaultObject());
	}

	if (!FComponentEditorUtils::IsValidVariableNameString(NodePtr->GetComponentTemplate(), NewName)) {
		UE_LOG(LogTemp, Error, TEXT("This name is reserved for engine use. Name=%s"), *NewName);
		return false;
	}
	else if (NewName.Len() > NAME_SIZE) {
		UE_LOG(LogTemp, Error, TEXT("Component name must be less than %s characters long. Name=%s"), NAME_SIZE, *NewName);
		return false;
	}
	else if (!FComponentEditorUtils::IsComponentNameAvailable(NewName, ExistingNameSearchScope, NodePtr->GetComponentTemplate())) {
		UE_LOG(LogTemp, Error, TEXT("Another component already has the same name. Name=%s"), *NewName);
		return false;
	}

	TSharedPtr<INameValidatorInterface> NameValidator;
	if (Blueprint != nullptr) {
		NameValidator = MakeShareable(new FKismetNameValidator(Blueprint, NodePtr->GetVariableName()));
	}
	else {
		NameValidator = MakeShareable(new FStringSetNameValidator(NodePtr->GetComponentTemplate()->GetName()));
	}

	EValidatorResult ValidatorResult = NameValidator->IsValid(NewName);
	if (ValidatorResult == EValidatorResult::AlreadyInUse) {
		UE_LOG(LogTemp, Error, TEXT("%s is in use by another variable or function!"), *NewName);
		return false;
	}
	else if (ValidatorResult == EValidatorResult::EmptyName) {
		UE_LOG(LogTemp, Error, TEXT("Names cannot be left blank!"), *NewName);
		return false;
	}
	else if (ValidatorResult == EValidatorResult::TooLong) {
		UE_LOG(LogTemp, Error, TEXT("Names must have fewer than 100 characters!"), *NewName);
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE