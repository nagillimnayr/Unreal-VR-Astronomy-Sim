// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "SimulationSubsystem.generated.h"


class ASystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatePositionsDelegate);

/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API USimulationSubsystem : public UTickableWorldSubsystem/*, public FTickableGameObject*/
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return Super::GetStatID();}

	/*virtual bool IsTickableWhenPaused() const
	{
		return false;
	}*/

	// Mutators
	UFUNCTION(BlueprintCallable)
	void IncreaseTimeScale(const int Amount);
	void DecreaseTimeScale(const int Amount);
	
	// Getters
	UFUNCTION(BlueprintCallable)
	TArray<ASystem*> GetSystems() {return Systems;}
	UFUNCTION(BlueprintCallable)
	void AddSystem(ASystem* NewSystem);
	
	// Time Getters
	UFUNCTION(BlueprintCallable)
	double GetTimeScale() { return TimeScale; }
	UFUNCTION(BlueprintCallable)
	double GetTimer() { return Timer; }
	UFUNCTION(BlueprintCallable)
	double GetDays() { return Days; }
	UFUNCTION(BlueprintCallable)
	double GetYears() { return Years; }

	// Delegates
	UPROPERTY(BlueprintAssignable, Category="Delegate")
	FOnUpdatePositionsDelegate OnUpdatePositionsDelegate;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASystem*> Systems; //Array of references to all of the systems in the simulation
	
	// Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeScale;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Timer = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Days = 0.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Years = 0;
	
	double Remainder;

private:
	friend class FSubsystemCollectionBase;
	FSubsystemCollectionBase* InternalOwningSubsystem;

	UFUNCTION(BlueprintCallable)
	void FindSystems(); // Populates the Systems array

	// To prevent ticking multiple times per frame
	uint32 LastFrameNumberTicked = INDEX_NONE;
public:
	static const int STEPS_PER_SECOND;
	static const double FIXED_TIMESTEP;
};
