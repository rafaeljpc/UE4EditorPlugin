#pragma once
#include "IDetailCustomization.h"
#include "Reply.h"
#include "CoreMinimal.h"
#include "SSCSEditor.h"


class FUE4EditorDetail : public IDetailCustomization {
private:
	TSharedPtr<SEditableTextBox> RenameTemplateTextBox;

public:
	virtual ~FUE4EditorDetail() {}

	static TSharedRef<IDetailCustomization> MakeInstance();

	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override { CustomizeDetails(*DetailBuilder); }

	static FReply RenameComponent(IDetailLayoutBuilder* DetailBuilder, FString TemplateName);

private:

	static bool CheckNewName(UBlueprint* Blueprint, FSCSEditorTreeNodePtrType NodePtr, FString NewName);
};