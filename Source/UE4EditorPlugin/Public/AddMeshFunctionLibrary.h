#pragma once

#include "Engine.h"
#include "AddMeshFunctionLibrary.generated.h"

UCLASS()
class UAddMeshFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	
	UFUNCTION()
		static void AddNMeshes(UBlueprint* Blueprint, FString ComponentTemplateName, UStaticMesh* StaticMesh, int32 Repetitions, FVector LocationStep, FRotator RotationStep);
};