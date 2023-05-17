// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "JsonParser.generated.h"

class FJsonObject;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_SIMULATION_API UJsonParser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJsonParser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool DeserializeFile();
	//virtual const FJsonObject& DeserializeFile(const FString TargetFileName, const FString TargetObjectName);
	virtual TSharedPtr<FJsonObject> DeserializeFile(const FString TargetFileName, const FString TargetObjectName);

	UFUNCTION(BlueprintCallable)
	FString GetObjectName() { return ObjectName; }
	

	const FJsonObject& GetJsonObject() { return *JsonObject; }

	
	UFUNCTION(BlueprintCallable)
	void SetObjectName(FString Name) { ObjectName = Name; }
	UFUNCTION(BlueprintCallable)
	void SetFileName(FString Name) { FileName = Name; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectName;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedPtr<FJsonObject> JsonObjectField;
};
