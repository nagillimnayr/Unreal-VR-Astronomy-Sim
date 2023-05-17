// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Orbit.generated.h"

class AAstroBody;
class ATrajectory;
class UArrowComponent;
class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UJsonParser;
//enum EOrbitPreset;
//class UEllipseMeshComponent;
class UOrbitalPathComponent;


UENUM(BlueprintType)
enum EOrbitPreset
{
	None,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune
};

const TMap<EOrbitPreset, FString> PresetNames = {
	{None, "None"},
	{Mercury, "Mercury"},
	{Venus, "Venus"},
	{Earth, "Earth"},
	{Mars, "Mars"},
	{Jupiter, "Jupiter"},
	{Saturn, "Saturn"},
	{Uranus, "Uranus"},
	{Neptune, "Neptune"},
};


UCLASS(Blueprintable, HideCategories=(Physics, Advanced, Activation, Collision))
class ORBIT_SIMULATION_API AOrbit : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void PostLoad() override;
	
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitalDistance();

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeOrbitingBody();
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void OrientOrbitingBodyTowardsCenter();
public:
	
	UFUNCTION(BlueprintCallable, Category = "Init")
	virtual void Initialize();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitingBody(float DeltaTime);

	// Getters
	AAstroBody* GetCentralBody() const { return CentralBody; } 
	AAstroBody* GetOrbitingBody() const { return OrbitingBody; }
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetSemiMajorAxis() const {return SemiMajorAxis;}

	double GetOrbitalSpeed() const { return OrbitalSpeed; }
	double GetOrbitalPeriod() const { return OrbitalPeriod; }
	double GetOrbitalDistance() const { return OrbitalRadius; }

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ShowOrbitalPath();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void HideOrbitalPath();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<AAstroBody> CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TObjectPtr<AAstroBody> OrbitingBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	// Camera
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double MaxOrbitalSpeed; // Orbital Speed at Periapsis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double MinOrbitalSpeed; // Orbital Speed at Apoapsis
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orbit")
	double PeriapsisRadius; // Closest point to the focus of attraction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double ApoapsisRadius; // Furthest point from the focus of attraction
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double OrbitalRadius; // Distance between the current position of the Orbiting Body and the Central Body

protected:
	// Orbital Elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiMajorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiMinorAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double OrbitalPeriod;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double Eccentricity;
	// Angular
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double Inclination;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double LongitudeOfPeriapsis;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double LongitudeOfAscendingNode;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double ArgumentOfPeriapsis;
	
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double OrbitalSpeed;

	// Anomalies
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Orbit")
	double TrueAnomaly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double EccentricAnomaly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double MeanAnomaly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double MeanAngularMotion; // Angle rotated per second
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SemiLatusRectum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	double SpecificOrbitalEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector SpecificAngularMomentum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Orbit")
	double LinearEccentricity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector EccentricityVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double AxialTilt; // Angle in degrees between the direction of the Orbiting Body's positive pole and the normal of the orbital plane, A.K.A Obliquity
	
	// Arrows
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	TObjectPtr<UArrowComponent> AscendingNodeArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	TObjectPtr<UArrowComponent> TrueAnomalyArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	TObjectPtr<UArrowComponent> MeanAnomalyArrow;

	// File reader
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJsonParser> JsonParser;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	TEnumAsByte<EOrbitPreset> Preset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UOrbitalPathComponent> OrbitalPath;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<UEllipseMeshComponent> OrbitalPlane;*/
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void LoadPreset();
	
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
	void SetTrueAnomalyArrow();
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetMeanAnomalyArrow();
	
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void DrawTrajectory();
	
	
	// Compute Keplerian Elements from Orbital State Vectors
	/*UFUNCTION(BlueprintCallable, Category = "Orbit")
	void CalculateTrajectory(); // Orbital State Vectors are needed to compute Keplerian Elements*/
	
	
public:

	bool operator<(const AOrbit& OtherOrbit) const
	{
		return PeriapsisRadius < OtherOrbit.PeriapsisRadius;
	}
	bool operator>(const AOrbit& OtherOrbit) const
	{
		return PeriapsisRadius > OtherOrbit.PeriapsisRadius;
	}
	
	/*
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;*/

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Astro")
	FLinearColor Color; // Color associated with the Orbit, for setting the color of the Trajectory, Trail effect, etc

protected:
	
/*#if WITH_EDITORONLY_DATA
	void OnObjectSelected(UObject* Object);
	bool SelectedInEditor = false;
#endif*/
};
