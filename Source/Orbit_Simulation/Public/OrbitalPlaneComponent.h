// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "ProceduralMeshComponent.h"
#include "OrbitalPlaneComponent.generated.h"

UCLASS()
class ORBIT_SIMULATION_API UOrbitalPlaneComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UOrbitalPlaneComponent();

	//virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void PostActorCreated() override;
	virtual void PostLoad() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void CreateEllipse();
	
public:	
	virtual void Initialize();
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void SetAxes(double a, double b);

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;*/
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> PlaneMesh;*/
	
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* OrbitMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* GridMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* GridMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSides;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SemiMajorAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SemiMinorAxis;

public:
	UFUNCTION(BlueprintCallable)
	void SetGridSize(const double Size);
	
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
