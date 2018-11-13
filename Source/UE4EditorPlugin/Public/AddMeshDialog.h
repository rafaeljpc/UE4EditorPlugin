#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "DeclarativeSyntaxSupport.h"
#include "SCompoundWidget.h"
#include "UE4EditorPlugin.h"
#include "Engine/Blueprint.h"

DECLARE_DELEGATE(FOnStartAddMesh);

class SAddMeshDialog : public SCompoundWidget {

public:
	SLATE_BEGIN_ARGS(SAddMeshDialog) {}
		// Blueprint that will receive the meshes
		SLATE_ATTRIBUTE(UBlueprint*, Blueprint)

		/** Action to perform when create clicked */
		SLATE_EVENT(FOnStartAddMesh, OnStartAddMesh)

	SLATE_END_ARGS()

public:

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	class UBlueprint* GetBlueprint() const;

	void ClosingWindow(const TSharedRef<SWindow>& InWindow);

private:
	FOnStartAddMesh OnStartAddMesh;

	UBlueprint* Blueprint;

	class UStaticMesh* StaticMesh;
	int32 Repetitions;
	FVector LocationOffset;
	FRotator RotationOffset;
};