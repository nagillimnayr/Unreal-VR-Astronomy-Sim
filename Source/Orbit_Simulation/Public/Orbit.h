// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Orbit.generated.h"

class AAstroBody;

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
	
	UFUNCTION(BlueprintCallable, Category = "Astro")
	void UpdateOrbitingBody(float DeltaTime);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Astro")
	void InitializeOrbitingBody();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* OrbitingBody;
};
