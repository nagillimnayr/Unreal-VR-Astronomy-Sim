// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialSphere.generated.h"

class AAstroBody;
class USphereComponent;
class UMaterialInstanceDynamic;
class APOVMotionMap;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API ACelestialSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
	UFUNCTION(BlueprintCallable)
	void SetParentBody(AAstroBody* NewParent)
	{
		ParentBody = NewParent;
		Initialize();
	}
	
	UFUNCTION(BlueprintCallable)
	void DestroySelf();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> SphereStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* MaterialInstance;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APOVMotionMap*> MotionMaps;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Subclass")
	TSubclassOf<APOVMotionMap> MotionMapSubclass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AAstroBody*> TrackedBodies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, APOVMotionMap*> MotionMaps;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAstroBody> ParentBody;

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	UFUNCTION(BlueprintCallable)
	void GetTrackedBodies();
};
