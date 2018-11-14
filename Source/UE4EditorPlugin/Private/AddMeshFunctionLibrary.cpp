#include "AddMeshFunctionLibrary.h"

void UAddMeshFunctionLibrary::AddNMeshes(UBlueprint* Blueprint, FString ComponentTemplateName, UStaticMesh* StaticMesh, 
	int32 Repetitions, FVector LocationStep, FRotator RotationStep) {
	UE_LOG(LogTemp, Log, TEXT("UAddMeshFunctionLibrary.AddNMeshes"));

	UE_LOG(LogTemp, Log, TEXT("Blueprint = %s"), *Blueprint->GetFullName());
	UE_LOG(LogTemp, Log, TEXT("ComponentTemplateName = %s"), *ComponentTemplateName);
	UE_LOG(LogTemp, Log, TEXT("StaticMesh = %s"), *StaticMesh->GetPathName());
	UE_LOG(LogTemp, Log, TEXT("Repetitions = %d"), Repetitions);
	UE_LOG(LogTemp, Log, TEXT("LocationStep = %s"), *LocationStep.ToString());
	UE_LOG(LogTemp, Log, TEXT("RotationStep = %s"), *RotationStep.ToString());


}