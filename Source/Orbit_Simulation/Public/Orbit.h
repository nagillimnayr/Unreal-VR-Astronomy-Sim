// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void Update(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitalDistance();

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeOrbitingBody();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* OrbitingBody;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Astro")
	double PerihelionDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Astro")
	double OrbitalDistance;

	
	// Keplerian Elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double Eccentricity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double SemiMajorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double Inclination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double LongitudeOfAscendingNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double ArgumentOfPeriapsis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Keplerian Elements")
	double TrueAnomaly;

	// Other Orbital Parameters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double Apoapsis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double Periapsis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiMinorAxis;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector SpecificAngularMomentum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector AscendingNodeVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EccentricityVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiLatusRectum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EllipticalCenter;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Astro")
	ATrajectory* Trajectory;
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DrawTrajectory();
	
	// Compute Keplerian Elements from Orbital State Vectors
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateTrajectory(); // Orbital State Vectors are needed to compute Keplerian Elements
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeAscendingNodeVector();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeSemiLatusRectum( double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeInclination();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeLongitudeOfAscendingNode(const double AscendingNodeMagnitude);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeArgumentOfPeriapsis(const double AscendingNodeMagnitude);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeTrueAnomaly(const FVector& Position, const FVector&  Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeArgumentOfLatitude(const double AscendingNodeMagnitude, const FVector& Position);
	
};
