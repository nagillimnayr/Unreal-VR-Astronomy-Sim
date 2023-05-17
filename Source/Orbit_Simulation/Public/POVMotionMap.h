// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "POVMotionMap.generated.h"

class USphereComponent;
class AAstroBody;
class USplineTraceComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(Blueprintable, BlueprintType)
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

	/*void SetSphereRadius();
	void SetSphereRadius(const double Radius);*/

	UFUNCTION(BlueprintCallable)
	void TraceMotion();

	UFUNCTION(BlueprintCallable)
	void SetReferenceBody(AAstroBody* Body);
	UFUNCTION(BlueprintCallable)
	void SetOtherBody(AAstroBody* Body);
	
	/*UFUNCTION(BlueprintCallable)
	void UpdatePosition();*/

	UFUNCTION(BlueprintCallable)
	void DestroySelf();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> ReferenceBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> OtherBody;
	

	/*// Sphere from which to calculate the intersections of the line drawn between the two bodies
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionSphere; // Every point vector on the sphere will have a magnitude equal to the radius
	*/

	// Collision / Line Trace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineTraceComponent> SplineTrace;

	UFUNCTION(BlueprintCallable)
	void CreateMaterialInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* SplineMaterialBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInstanceDynamic* SplineMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* BodyMaterialBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstanceDynamic* BodyMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ProjectedBody;


	// Trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TrailSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TrailComponent;
	
	
};
