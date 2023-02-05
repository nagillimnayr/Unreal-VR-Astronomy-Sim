// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroBody.h"

#include "Orbit_Base.generated.h"

UCLASS()
class ORBIT_SIMULATOR_API AOrbit_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbit_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* CentralBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	AAstroBody* OrbitingBody;
	UPROPERTY(E)
};
