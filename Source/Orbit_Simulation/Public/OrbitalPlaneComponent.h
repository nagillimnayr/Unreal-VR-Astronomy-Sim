// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "ProceduralMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "OrbitalPlaneComponent.generated.h"

class UMaterialInstanceConstant;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_SIMULATION_API UOrbitalPlaneComponent : public USceneComponent
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
	UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSides;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SemiMajorAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SemiMinorAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;
public:
	UFUNCTION(BlueprintCallable, Category="Material")
	void SetColor(const FLinearColor NewColor);
	UFUNCTION(BlueprintCallable, Category="Material")
	void InitializeMaterial();
	

};
