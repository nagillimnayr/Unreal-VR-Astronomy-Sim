// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "SimPlayerController.generated.h"

class ISelectableInterface;
DECLARE_DELEGATE_OneParam(FInputSwitchPawnDelegate, const int32)
DECLARE_DELEGATE_OneParam(FInputZoomDelegate, const double)
DECLARE_DELEGATE_OneParam(FInputYawDelegate, double)
DECLARE_DELEGATE_OneParam(FInputPitchDelegate, double)
DECLARE_DELEGATE(FInputMouseClickDelegate)

class ASystem;
class ASimulation;
class AObserver;
class AObservationPoint;
class UInputMappingContext;
class USimInputConfigData;
struct FInputActionValue;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API ASimPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//ASimPlayerController();
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void SetupInputComponent() override;

	
	UFUNCTION(BlueprintCallable, Category="Selection")
	void Select(AActor* NewSelectedActor);
	UFUNCTION(BlueprintCallable, Category="Selection")
	void Deselect();

	// Delegates
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnMouseClick();
	UFUNCTION(BlueprintCallable, Category="Input")
	void SwitchPawn(const int Index);
	UFUNCTION(BlueprintCallable, Category="Input")
	void CameraZoom(const FInputActionValue& AxisInput);
	UFUNCTION(BlueprintCallable, Category="Input")
	void CameraYaw(const FInputActionValue& AxisInput);
	UFUNCTION(BlueprintCallable, Category="Input")
	void CameraPitch(const FInputActionValue& AxisInput);
public:
	// Setters
	UFUNCTION(BlueprintCallable)
	void SetSimulation(ASimulation* Sim);
	UFUNCTION(BlueprintCallable)
	void SetSystem(ASystem* NewSystem);

	// Getters
	AActor* GetSelected() {return SelectedActor;}
	
	UFUNCTION(BlueprintCallable)
	void GoToSurface();
	UFUNCTION(BlueprintCallable)
	void GoToSpace();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASimulation> Simulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASystem> System;

	// Currently Selected Object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> SelectedActor;

	// Observer Blueprint Subclass
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Observer")
	TSubclassOf<AObserver> ObserverSubclass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Observer")
	TSubclassOf<AObservationPoint> ObservationPointSubclass;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	double LookSpeed = 25.0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	USimInputConfigData* InputActions;
};
