// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "OrbitalPathComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),ShowCategories = (Lighting, Rendering))
class ORBIT_SIMULATION_API UOrbitalPathComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UOrbitalPathComponent();
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void InitializeSpline(const double SemiMajorAxis, const double SemiMinorAxis);
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void InitializeSplineMesh(const double SemiMajorAxis, const double SemiMinorAxis);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void DrawPath(const double SemiMajorAxis, const double SemiMinorAxis);
	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetNumberOfPoints(const int Number) {NumberOfPoints = Number;}
	
	UFUNCTION(BlueprintCallable, Category = "Color")
	void SetColor(const FLinearColor Color);
	UFUNCTION(BlueprintCallable, Category = "Material")
	void SetNewMaterial(UMaterialInterface* Material);
	UFUNCTION(BlueprintCallable, Category="Material")
	virtual void CreateMaterialInstance();
	
	UFUNCTION(BlueprintCallable, Category="Render")
	void ShowMesh();
	UFUNCTION(BlueprintCallable, Category="Render")
	void HideMesh();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	int NumberOfPoints = 36;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	bool bMeshVisibility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineMesh")
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineMesh")
	FVector2D MeshScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SplineMesh")
	TArray<USplineMeshComponent*> SplineMeshes; // Array of Spline Meshes
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineMesh")
	UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineMesh")
	UMaterialInstanceDynamic* MaterialInstance;

	/*UFUNCTION(BlueprintCallable)
	void ClearSplineMeshes();*/
};
