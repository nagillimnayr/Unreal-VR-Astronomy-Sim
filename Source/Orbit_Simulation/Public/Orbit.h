// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Orbit.generated.h"

class AAstroBody;
class ATrajectory;

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
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbit(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitalDistance();

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeOrbitingBody();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* OrbitingBody;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double InitialOrbitalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Astro")
	double PeriapsisRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Astro")
	double ApoapsisRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Astro")
	double OrbitalRadius;

	
	// Keplerian Elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keplerian Elements")
	double Eccentricity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keplerian Elements")
	double SemimajorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keplerian Elements")
	double Inclination = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keplerian Elements")
	double LongitudeOfAscendingNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keplerian Elements")
	double ArgumentOfPeriapsis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double TrueAnomaly;

	// Other Orbital Parameters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiminorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector PeriapsisVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector ApoapsisVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector SpecificAngularMomentum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector AscendingNodeVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EccentricityVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemilatusRectum;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SpecificOrbitalEnergy;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double LinearEccentricity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double OrbitalPeriod;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EllipticalCenter; // Center of Ellipse, for positioning the Trajectory object
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	ATrajectory* Trajectory;
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateOrbit();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DrawTrajectory();
	
	// Compute Keplerian Elements from Orbital State Vectors
	/*UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateTrajectory(); // Orbital State Vectors are needed to compute Keplerian Elements*/
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeAscendingNodeVector();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeSemiLatusRectum( double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeInclination();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeLongitudeOfAscendingNode(const double AscendingNodeMagnitude);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeArgumentOfPeriapsis(const double AscendingNodeMagnitude);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeTrueAnomaly(const FVector& Position, const FVector&  Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void ComputeArgumentOfLatitude(const double AscendingNodeMagnitude, const FVector& Position);

	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateSpecifcOrbitalEnergy();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateSemiMajorAxisFromSpecifcOrbitalEnergy();

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateApoapsisRadius();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculatePeriodFromSemiMajorAxis(); // Calculate the Orbital Period based on the Semi-Major Axis
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateSemiMajorAxisFromPeriod(double Period); // Calculate the Semi-Major Axis necessary for a particular Orbital Period

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateEccentricityFromSpecificAngularMomentum();

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateLinearEccentricity();

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateEccentricityFromApsides();

};
