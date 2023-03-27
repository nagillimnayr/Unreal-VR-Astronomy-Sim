// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "AstroBody.generated.h"

class USpringArmComponent;
class USpotLightComponent;
class AOrbit;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;
class UCameraComponent;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API AAstroBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAstroBody();

	/*UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void CalculateAcceleration(const AAstroBody* OtherBody);*/
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void SetAcceleration(const FVector NewAcceleration);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void UpdateVelocity(const FVector NewVelocity);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void UpdatePosition(const double DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void UpdateVelocityArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void UpdateAccelerationArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void AimAccelerationArrow(const AActor* const Target);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(BlueprintCallable, Category="Initialization")
	virtual void Initialize();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FVector& GetAccelerationVector() const {return AccelerationVector;}
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FVector& GetVelocityVector() const {return VelocityVector;}
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	double GetMassOfBody() const {return Mass;}
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetSize() const {return MeanRadius;}
	
	// Setters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void InitializeVelocity(const FVector& InitVelocity) { VelocityVector = InitVelocity; OrbitalSpeed = InitVelocity.Length();}
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	void SetMass(const double NewMass) { Mass = NewMass; }
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	void SetMeanRadius(const double NewMeanRadius)
	{
		MeanRadius = NewMeanRadius;
		//SphereMesh->SetRelativeScale3D(FVector(1.0));
		//SetActorScale3D(FVector(MeanRadius)); // Set size of Body
		//SetActorScale3D(FVector(MeanRadius / (Unit::DISTANCE_MULT * 100.0))); // Set size of Sphere
		SetActorRelativeScale3D(FVector(1.0));
		SphereMesh->SetWorldScale3D(FVector(MeanRadius));
	}
	UFUNCTION(BlueprintCallable, Category = "Color")
	void SetColor(const FLinearColor NewColor);

	
protected:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	double Mass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	FVector VelocityVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	FVector AccelerationVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double OrbitalSpeed; // Scalar magnitude of Velocity Vector
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double AccelerationMagnitude; // Scalar magnitude of Acceleration Vector

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double MeanRadius; // Mean Radius of the body. (1.0 = Mean Radius of Earth)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double SiderealRotationPeriod; // The period in days for the body to make one full 360 degree revolution about its polar axis
public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SphereMesh;
	
	// Arrows
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* AccelerationArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* VelocityArrow;

	// Trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TrailSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
};
