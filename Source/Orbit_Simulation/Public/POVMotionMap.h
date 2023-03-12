// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "POVMotionMap.generated.h"

class USphereComponent;
class AAstroBody;
class ASplineTrace;

UCLASS()
class ORBIT_SIMULATION_API APOVMotionMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APOVMotionMap();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSphereRadius();
	void SetSphereRadius(const double Radius);

	UFUNCTION(BlueprintCallable)
	void TraceMotion();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> ReferenceBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> OtherBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TArray<FVector> MappedPoints; // Array of points mapped onto a 2D plane

	// Sphere from which to calculate the intersections of the line drawn between the two bodies
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> Sphere; // Every point vector on the sphere will have a magnitude equal to the radius

	// Collision / Line Trace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_PhysicsBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASplineTrace> SplineTrace;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ProjectedBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> DefaultMaterial;
};
