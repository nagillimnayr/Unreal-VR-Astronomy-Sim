// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroBody.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ORBIT_SIMULATION_API AAstroBody : public AActor
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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetInitialVelocity() const {return InitialVelocity;};
	
	// Setters
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeVelocity(FVector& Velocity) {this->OrbitalVelocity = Velocity;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticSphereMesh;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	double mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	double InitialVelocity; // Scalar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	FVector OrbitalVelocity;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	FVector Acceleration;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double VelocityMagnitude; // Scalar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double AccelerationMagnitude; // Scalar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double OrbitalDistance; // Scalar

public:
	// Arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* XArrow;

	// Trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TrailSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TrailComponent;

	// Sidereal rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double SiderealRotation; // Value of 1 = one full 360 degree revolution each day. (i.e. Earth's sidereal rotation)
	

};