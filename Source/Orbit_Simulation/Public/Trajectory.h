// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Trajectory.generated.h"

class UOrbitalPlaneComponent;
class AAstroBody;
class USpringArmComponent;
class UEllipseMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API ATrajectory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrajectory();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Initialize();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetAxes(const double a, const double b);
	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetClosedLoop(const bool Closed) { isClosedLoop = Closed; }
	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void Draw();
	
	UFUNCTION(BlueprintCallable, Category = "Color")
	void SetColor(const FLinearColor NewColor);

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
	void UpdateEllipse();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	FVector GetCenter();
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void UpdateArrows();
	UFUNCTION(BlueprintCallable, Category = "Spline")
	FVector GetPeriapsisVector();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetSemiMajorAxis(const double a) {
		SemiMajorAxis = a; 
		SemiMajorAxisArrow->ArrowLength = SemiMajorAxis;
	}
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetSemiMinorAxis(const double b) {
		SemiMinorAxis = b; 
		SemiMinorAxisArrow->ArrowLength = SemiMinorAxis;
	}
protected:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	TArray<USplineMeshComponent*> SplineMeshes; // Array of Spline Meshes
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> SplineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInstanceDynamic* MaterialInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	int NumberOfPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	double SemiMajorAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	double SemiMinorAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	bool isClosedLoop = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UArrowComponent> SemiMajorAxisArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UArrowComponent> SemiMinorAxisArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UArrowComponent> SemiLatusRectumArrow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UOrbitalPlaneComponent> OrbitalPlane;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	FVector2D MeshScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

public:
	void InitializeMaterial();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
