#include "AddMeshDialog.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/Input/SRotatorInputBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "PropertyCustomizationHelpers.h"



#define LOCTEXT_NAMESPACE "AddMeshDialog"

void SAddMeshDialog::Construct(const FArguments& InArgs) {
	Blueprint = InArgs._Blueprint.Get();

	ChildSlot[
		SNew(SGridPanel)
			.FillColumn(0, 0.4f)
			.FillColumn(1, 0.6f)
			
			// Component Name Template 

			+SGridPanel::Slot(0, 0)
				.Padding(0, 0, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.ComponentNameTemplate", "Component Name Template"))
				]
			+ SGridPanel::Slot(1, 0)
				.Padding(0, 0, 0, 0)
				[
					SNew(SEditableTextBox)
					.Text_Lambda([this] () -> FText { return FText::FromString(ComponentTemplateName); })
						.ToolTipText(LOCTEXT("AddMeshDialog.ComponentNameTemplate", "Component Name Template, <name>_<index>"))
						.OnTextCommitted_Lambda([this] (const FText& InText, ETextCommit::Type InCommitType) {
							ComponentTemplateName = InText.ToString();
						})
				]
			
			// Mesh
			+ SGridPanel::Slot(0, 1)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.Mesh", "Static Mesh"))
				]
			+ SGridPanel::Slot(1, 1)
				.Padding(0, 10, 0, 0)
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

			// Repetition
			+ SGridPanel::Slot(0, 2)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.Repetition", "Number of Repetition"))
				]
			+ SGridPanel::Slot(1, 2)
				.Padding(0, 10, 0, 0)
				[
					SNew(SNumericEntryBox<int32>)
						.AllowSpin(true)
						.MinValue(0)
						.MaxValue(TNumericLimits<int32>::Max())
						.Value_Lambda([this] () -> int32 {
							return Repetitions;
						})
						.OnValueCommitted_Lambda([this] (int32 NewValue, ETextCommit::Type CommitInfo) {
							Repetitions = NewValue;
						})
				]

			// Location Offset
			+ SGridPanel::Slot(0, 3)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.LocationOffset", "Location Offset"))
				]
			+ SGridPanel::Slot(1, 3)
				.Padding(0, 10, 0, 0)
				[
					SNew(SVectorInputBox)
						.bColorAxisLabels(true)
						.AllowResponsiveLayout(true)
						.AllowSpin(false)
						.X_Lambda([this] () { return LocationOffset.X; })
						.Y_Lambda([this] () { return LocationOffset.Y; })
						.Z_Lambda([this] () { return LocationOffset.Z; })
						.OnXCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { LocationOffset.X = NewValue; })
						.OnYCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { LocationOffset.Y = NewValue; })
						.OnZCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { LocationOffset.Z = NewValue; })
				]

			// Rotation Offset
			+ SGridPanel::Slot(0, 4)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("AddMeshDialog.LocationOffset", "Rotation Offset"))
				]
			+ SGridPanel::Slot(1, 4)
				.Padding(0, 10, 0, 0)
				[
					SNew(SRotatorInputBox)
						.bColorAxisLabels(true)
						.AllowResponsiveLayout(true)
						.AllowSpin(true)
						.Roll_Lambda([this] () { return RotationOffset.Roll; })
						.Pitch_Lambda([this] () { return RotationOffset.Pitch; })
						.Yaw_Lambda([this] () { return RotationOffset.Yaw; })						
						.OnRollCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { RotationOffset.Roll = NewValue; })
						.OnPitchCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { RotationOffset.Pitch = NewValue; })
						.OnYawCommitted_Lambda([this] (float NewValue, ETextCommit::Type CommitInfo) { RotationOffset.Yaw = NewValue; })
				]


			// Confirm Button
			+ SGridPanel::Slot(0, 5)
				.Padding(0, 10, 10, 0)
				.VAlign(VAlign_Center)
				.ColumnSpan(2)
				[
					SNew(SButton)
						.Text(FText::FromString("Build Body Region"))
						.OnClicked_Lambda([this]() -> FReply {
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.Button.Click: Button Clicked"));
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.ComponentTemplateName = %s"), *ComponentTemplateName);
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.StaticMesh = %s"), *StaticMesh->GetPathName());
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.Repetitions = %d"), Repetitions);
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.LocationOffset = %s"), *LocationOffset.ToString());
							UE_LOG(LogTemp, Log, TEXT("SAddMeshDialog.RotationOffset = %s"), *RotationOffset.ToString());

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