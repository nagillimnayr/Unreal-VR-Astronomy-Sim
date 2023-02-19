// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Trajectory.generated.h"

class AAstroBody;

UCLASS()
class ORBIT_SIMULATION_API ATrajectory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrajectory();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetSemimajorAxis(const double a) {
		SemimajorAxis = a; 
		SemimajorAxisArrow->ArrowLength = SemiminorAxis;
	}
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetSemiminorAxis(const double b) {
		SemiminorAxis = b; 
		SemiminorAxisArrow->ArrowLength = SemiminorAxis;
	}
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetClosedLoop(const bool Closed) { isClosedLoop = Closed; }

	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void Update();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void InitializeSpline();
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void InitializeSplineMesh();
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void UpdateSplineMesh();
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void DrawEllipse();
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	TArray<USplineMeshComponent*> SplineMeshes; // Array of Spline Meshes
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent* SplineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* DefaultMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* AlternateMaterial;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	int NumberOfPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	double SemimajorAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	double SemiminorAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	bool isClosedLoop = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	UArrowComponent* SemimajorAxisArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	UArrowComponent* SemiminorAxisArrow;
	
};
