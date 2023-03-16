// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Orbit.generated.h"

class AAstroBody;
class ATrajectory;
class USpringArmComponent;
class UArrowComponent;

UCLASS()
class ORBIT_SIMULATION_API AOrbit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitingBody(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitalDistance();

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeOrbitingBody();
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void OrientOrbitingBodyTowardsCenter();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<AAstroBody> CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<AAstroBody> OrbitingBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double InitialOrbitalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orbit")
	double PeriapsisRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double ApoapsisRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double OrbitalRadius;

protected:
	// Orbital Elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiMajorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiMinorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double OrbitalPeriod;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double Inclination;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double LongitudeOfAscendingNode;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double ArgumentOfPeriapsis;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double Eccentricity;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double TrueAnomaly;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiLatusRectum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SpecificOrbitalEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector SpecificAngularMomentum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double LinearEccentricity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector PeriapsisVector;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	//FVector ApoapsisVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector AscendingNodeVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EccentricityVector;
	

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UArrowComponent> SemiLatusRectumArrow;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UArrowComponent> AscendingNodeArrow;
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EllipticalCenter; // Center of Ellipse, for positioning the Trajectory object*/
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	ATrajectory* Trajectory;
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void OrientPeriapsisVector();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateOrbit();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateOrbitFromEccentricity();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void OrientOrbit();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void OrientAscendingNode();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void OrientArgumentOfPeriapsis();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void OrientInclination();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DrawTrajectory();
	
	// Compute Keplerian Elements from Orbital State Vectors
	/*UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateTrajectory(); // Orbital State Vectors are needed to compute Keplerian Elements*/
	
	
public:
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
