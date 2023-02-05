// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroBody.generated.h"

UCLASS()
class ORBIT_SIMULATOR_API AAstroBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAstroBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void CalculateAcceleration();
	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void UpdateVelocity();
	UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void UpdatePosition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion") double mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion") double initialVelocity; // Scalar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion") FVector velocity;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion") FVector acceleration;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion") double velocityMagnitude; // Scalar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion") double accelerationMagnitude; // Scalar
};
