// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroBody.generated.h"

class USpringArmComponent;
class USpotLightComponent;
class AOrbit;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;

UCLASS()
class ORBIT_SIMULATION_API AAstroBody : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAstroBody();

	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void CalculateAcceleration(AAstroBody* OtherBody);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void UpdateVelocity(const double DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void UpdatePosition(const double DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	virtual void UpdateVelocityArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	virtual void UpdateAccelerationArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	virtual void AimAccelerationArrow(AActor* Target);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	void UpdateSpotLight(AActor* Source);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FVector& GetVelocityVector() const {return VelocityVector;};
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetMassOfBody() const {return mass;};
	
	// Setters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void InitializeVelocity(FVector& Velocity) {this->VelocityVector = Velocity; this->OrbitalSpeed = Velocity.Length();}

	
protected:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	double mass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	FVector VelocityVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	FVector AccelerationVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double OrbitalSpeed; // Scalar magnitude of Velocity Vector

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double AccelerationMagnitude; // Scalar

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double Size;

public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticSphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> SpotLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpotLightBoom;
	
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

	// Reference to Orbit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	AOrbit* Orbit;
};
