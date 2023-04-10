// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "EllipseMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ORBIT_SIMULATION_API UEllipseMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UEllipseMeshComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void CreateMesh(int NumOfSides, double SemiMajorAxis, double SemiMinorAxis, const FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "Color")
	void SetColor(const FLinearColor Color);
	
	UFUNCTION(BlueprintCallable, Category = "Material")
	virtual void SetNewMaterial(UMaterialInterface* Material);
	UFUNCTION(BlueprintCallable, Category = "Material")
	virtual void CreateMaterialInstance();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* MaterialInstance;
};
