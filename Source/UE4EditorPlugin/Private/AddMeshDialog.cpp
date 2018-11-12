#include "AddMeshDialog.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "PropertyCustomizationHelpers.h"


#define LOCTEXT_NAMESPACE "AddMeshDialog"

void SAddMeshDialog::Construct(const FArguments& InArgs) {
	Blueprint = InArgs._Blueprint.Get();

	ChildSlot[
		SNew(SGridPanel)
			.FillColumn(0, 0.2f)
			.FillColumn(1, 0.8f)
			
			// Mesh
			+ SGridPanel::Slot(0, 0)
				.Padding(0, 0, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.Mesh", "Static Mesh"))
				]
			+ SGridPanel::Slot(1, 0)
				.Padding(0, 0, 0, 0)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(UStaticMesh::StaticClass())
						.AllowClear(true)
						.ObjectPath_Lambda([this]() -> FString {
							return StaticMesh ? StaticMesh->GetPathName() : FString("");
						})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData) {
							StaticMesh = Cast<UStaticMesh>(AssetData.GetAsset());
						})
						
				]

			// Confirm Button
			+ SGridPanel::Slot(0, 3)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				.ColumnSpan(2)
				[
					SNew(SButton)
						.Text(FText::FromString("Build Body Region"))
						.OnClicked_Lambda([this]() -> FReply {
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.Button.Click: Button Clicked"));
							return FReply::Handled();
						})
				]
		];
}

UBlueprint* SAddMeshDialog::GetBlueprint() const {
	return Blueprint;
}

void SAddMeshDialog::ClosingWindow(const TSharedRef<SWindow>& InWindow) {
	Blueprint = nullptr;
}

#undef LOCTEXT_NAMESPACE