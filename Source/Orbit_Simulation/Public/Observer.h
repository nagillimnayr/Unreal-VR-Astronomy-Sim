// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Observer.generated.h"

class UCameraComponent;
class USimInputConfigData;
class UInputMappingContext;
class USpringArmComponent;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API AObserver : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double LookSpeed = 25.0;
	// Input Handlers
	UFUNCTION(BlueprintCallable, Category="Input")
	void CameraYaw(const FInputActionValue& AxisInput);
	UFUNCTION(BlueprintCallable, Category="Input")
	void CameraPitch(const FInputActionValue& AxisInput);
	UFUNCTION(BlueprintCallable, Category="Input")
	void FreeLook(const FInputActionValue& AxisInput);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRoot;
	
	// Camera
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	USimInputConfigData* InputActions;
};
