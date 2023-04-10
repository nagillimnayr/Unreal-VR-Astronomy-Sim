// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SplineTrace.generated.h"

class ASimulation;

UCLASS()
class ORBIT_SIMULATION_API ASplineTrace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineTrace();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void InitializeFirstPoints(const FVector& Position);

	// Update spline
	UFUNCTION(BlueprintCallable)
	void UpdateLastPoint(const FVector& Position);
	UFUNCTION(BlueprintCallable)
	void AddNewPoint(const FVector& Position);
	
	// Update mesh
	void AddSplineMesh();
	void UpdateMesh();
	void UpdateLastMesh();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update(const FVector& Position);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sim")
	ASimulation* Sim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	double Duration; // How long the path will be traced for (in Days)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient,  Category = "Time")
	double Timer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	double Interval; // Time in Days between updates to the path
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Time")
	double TimeSinceLastUpdate;
	

	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	// Spline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> SplineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline") // This should maybe be a Queue?
	TArray<USplineMeshComponent*> SplineMeshes; // Array of Spline Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	FVector2D MeshScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* DefaultMaterial;

	// Counters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Count")
	int SplinePointCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Count")
	int SplineMeshCount;

};

