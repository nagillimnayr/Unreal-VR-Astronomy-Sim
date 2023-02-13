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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Astro")
	double OrbitalDistance;

	
	// Keplerian Elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double Eccentricity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double SemiMajorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double Inclination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double LongitudeOfAscendingNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double ArgumentOfPeriapsis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double TrueAnomaly;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Astro")
	ATrajectory* Trajectory;
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DrawTrajectory();
	
	// Compute Keplerian Elements from Orbital State Vectors
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void UpdateTrajectory(); // Orbital State Vectors are needed to compute Keplerian Elements
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeSpecificAngularMomentum(const FVector& Position, const FVector& Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeAscendingNodeVector(const FVector& SpecificAngularMomentum);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	FVector ComputeEccentricityVector(const FVector& Velocity, const FVector& Position, const FVector& SpecificAngularMomentum, double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeSemiLatusRectum(const FVector& SpecificAngularMomentum, double SGP);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeInclination(const FVector& SpecificAngularMomentum);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeLongitudeOfAscendingNode(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeArgumentOfPeriapsis(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude, const FVector& EccentricityVector);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeTrueAnomaly(const FVector& Position, const FVector&  Velocity);
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	double ComputeArgumentOfLatitude(const FVector& AscendingNodeVector, const double AscendingNodeMagnitude, const FVector& Position);
	
};
