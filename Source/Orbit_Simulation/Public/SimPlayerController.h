// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "SimPlayerController.generated.h"

// Forward declarations
class ISelectableInterface;
class ASystem;
//class ASimulation;
class AAstroBody;
class USimulationSubsystem;
class AObserver;
class AObservationPoint;
class ACelestialSphere;
class UInputMappingContext;
class USimInputConfigData;
struct FInputActionValue;
class UMaterialParameterCollectionInstance;

// Input Delegates
DECLARE_DELEGATE_OneParam(FInputSwitchPawnDelegate, const int32);
DECLARE_DELEGATE_OneParam(FInputZoomDelegate, const double);
DECLARE_DELEGATE_OneParam(FInputYawDelegate, double);
DECLARE_DELEGATE_OneParam(FInputPitchDelegate, double);
DECLARE_DELEGATE(FInputMouseClickDelegate);
DECLARE_DELEGATE_OneParam(FInputIncrementTimeScaleDelegate, const int32);

// Selection Change Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectDelegate, AActor*, NewSelection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnselectDelegate);

// Surface/Space Transition Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTransitionToSurfaceDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTransitionToSpaceDelegate);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API ASimPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//ASimPlayerController();

	// Selection Delegates
	UPROPERTY(BlueprintAssignable, Category="Selection")
	FOnSelectDelegate OnSelectDelegate;
	UPROPERTY(BlueprintAssignable, Category="Selection")
	FOnUnselectDelegate OnUnSelectDelegate;

	// Surface/ Space Transition Delegates
	UPROPERTY(BlueprintAssignable, Category="Transition")
	FOnTransitionToSurfaceDelegate OnTransitionToSurfaceDelegate;
	UPROPERTY(BlueprintAssignable, Category="Transition")
	FOnTransitionToSpaceDelegate OnTransitionToSpaceDelegate;
	
	UFUNCTION(BlueprintCallable, Category="Selection")
	void Select(AActor* NewSelectedActor);
	UFUNCTION(BlueprintCallable, Category="Selection")
	void Deselect();
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void SetupInputComponent() override;
	
	// Input Handlers
	// Space View
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

	// Surface View
	UFUNCTION(BlueprintCallable, Category="Input")
	void FreeLook(const FInputActionValue& AxisInput);
	
public:
	// Setters
	UFUNCTION(BlueprintCallable)
	void SetSimulation(USimulationSubsystem* Sim);
	UFUNCTION(BlueprintCallable)
	void SetSystem(ASystem* NewSystem);

	// Getters
	AActor* GetSelected() {return SelectedActor;}
	double GetLookSpeed() const {return LookSpeed;}


	UFUNCTION(BlueprintCallable)
	void IncreaseTimeScale(const int Amount);
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void TransitionToSurface();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void TransitionToSpace();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USimulationSubsystem> Simulation;
	//TObjectPtr<ASimulation> Simulation;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Observer")
	TSubclassOf<ACelestialSphere> CelestialSphereSubclass;
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Observer")
	TObjectPtr<ACelestialSphere> CelestialSphere;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	double LookSpeed = 50.0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* SpaceViewInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* SurfaceViewInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	USimInputConfigData* InputActions;

	UFUNCTION()
	void SetInputMappingContext(UInputMappingContext* IMC);

	// Material Parameter Collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollection* MaterialParameterCollection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollectionInstance* GlobalMaterialParameters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AAstroBody> SurfaceBody; // The Body that we are currently on
};
