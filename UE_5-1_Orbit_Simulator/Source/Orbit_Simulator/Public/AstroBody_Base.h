// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AstroBody_Base.generated.h"

UCLASS()
class ORBIT_SIMULATOR_API AAstroBody_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAstroBody_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(editAnywhere) double mass;
	UPROPERTY(editAnywhere) double radius;
	UPROPERTY(editAnywhere) FVector initialVelocity;
	FVector velocity;
};
