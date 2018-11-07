#include "UE4EditorDetail.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "STextBlock.h"
#include "DeclarativeSyntaxSupport.h"
#include "SButton.h"
#include "Widgets/Input/SEditableTextBox.h"

#define LOCTEXT_NAMESPACE "UE4EditorDetail"

TSharedRef<IDetailCustomization> FUE4EditorDetail::MakeInstance() {
	return MakeShareable(new FUE4EditorDetail);
}

void FUE4EditorDetail::CustomizeDetails(IDetailLayoutBuilder & DetailBuilder) {
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	TSet<UClass*> Classes;
	for (auto& WeakObj : ObjectsBeingCustomized) {
		if (UObject* Instance = WeakObj.Get()) {
			Classes.Add(Instance->GetClass());
		}
	}

	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory(TEXT("Rename Components Commands"));
	
	FDetailWidgetRow& Row = Category.AddCustomRow(FText::FromString("Rename Component"));

	Row.NameContent()[
			SAssignNew(RenameTemplateTextBox, SEditableTextBox)
				.ToolTipText(LOCTEXT("RenameComponentLabel_Tooltip", "Template of the Name of the components [NAME]_[index]"))
		]
		.ValueContent()[
			SNew(SButton)
				.Text(FText::FromString("Rename Component"))
				.OnClicked(FOnClicked::CreateLambda([this, &DetailBuilder, ObjectsBeingCustomized]() {
					UE_LOG(LogTemp, Log, TEXT("Rename Clicked RenameTo=%s Count=%d"), 
						*RenameTemplateTextBox->GetText().ToString(), ObjectsBeingCustomized.Num());

					return FReply::Handled();
				}))
		];
}

FReply FUE4EditorDetail::RenameComponent(IDetailLayoutBuilder * DetailBuilder, FString NewName) {
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE