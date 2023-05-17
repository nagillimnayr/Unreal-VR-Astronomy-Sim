// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableInterface.h"
#include "GameFramework/Pawn.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
//#include "Components/MaterialBillboardComponent.h"
#include "AstroBody.generated.h"

//class UMaterialBillboardComponent;
//class UBillboardComponent;
class AObservationPoint;
//class UScalableSphereGizmo;
class USpringArmComponent;
//class USpotLightComponent;
class AOrbit;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;
class UCameraComponent;
class USphereComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS(Blueprintable, BlueprintType)
class ORBIT_SIMULATION_API AAstroBody : public APawn, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAstroBody();

	/*UFUNCTION(BlueprintCallable, Category = "Motion")
	virtual void CalculateAcceleration(const AAstroBody* OtherBody);*/
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void SetAcceleration(const FVector NewAcceleration);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void UpdateVelocity(const FVector NewVelocity);
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void UpdatePosition(const double DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void UpdateVelocityArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void UpdateAccelerationArrow();
	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void AimAccelerationArrow(const AActor* const Target);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;


public:
	UFUNCTION(BlueprintCallable, Category="Initialization")
	virtual void Initialize();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FVector& GetAccelerationVector() const {return AccelerationVector;}
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FVector& GetVelocityVector() const {return VelocityVector;}
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	double GetMassOfBody() const {return Mass;}
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetMeanRadius() const {return MeanRadius;}
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetMeanRadiusAdjusted() const {return MeanRadius / Unit::DISTANCE_MULT;}
	UFUNCTION(BlueprintCallable, Category = "Astro")
	double GetMeanRadiusScale() const {return SphereMesh->GetComponentScale().X;} // Returns the scale of the Sphere Mesh
	
	UFUNCTION(BlueprintCallable, Category = "Astro")
	FLinearColor GetColor() {return Color;}

	UFUNCTION(BlueprintCallable, Category = "Camera")
	USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	UFUNCTION(BlueprintCallable, Category = "Camera")
	UCameraComponent* GetCamera() { return Camera; }
	
	// Setters
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void InitializeVelocity(const FVector& InitVelocity) { VelocityVector = InitVelocity; OrbitalSpeed = InitVelocity.Length();}
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	void SetMass(const double NewMass) { Mass = NewMass; }
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	void SetMeanRadius(const double NewMeanRadius);
	UFUNCTION(BlueprintCallable, Category = "PhysicalParameters")
	void SetObliquity(const double NewObliquity);
	
	UFUNCTION(BlueprintCallable, Category = "Color")
	void SetColor(const FLinearColor NewColor);
	
	/*UFUNCTION(BlueprintCallable, Category = "Billboard")
	void UpdateBillboardSpriteScreenSpace();
	UFUNCTION(BlueprintCallable, Category = "Billboard")
	void UpdateBillboardSpriteAbsolute();*/

	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetOrbit(AOrbit* NewOrbit) { Orbit = NewOrbit;}
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	AOrbit* GetOrbit() {return Orbit;}

	UFUNCTION(BlueprintCallable, Category = "GeoCoord")
	void PositionGeoCoords(const double Latitude, const double Longitude);
	
	UFUNCTION(BlueprintCallable, Category = "Icon")
	UTextureRenderTarget2D* GetIconRenderTarget();
	UFUNCTION(BlueprintCallable, Category = "Icon")
	UMaterialInstanceDynamic* GetIconMaterial();

	UFUNCTION(BlueprintCallable, CallInEditor,Category = "Icon")
	void CreateIcon();

	UFUNCTION(BlueprintCallable)
	void SetObservationPoint(AObservationPoint* ObservationPoint);
	UFUNCTION(BlueprintCallable)
	void RemoveObservationPoint();

	UFUNCTION(BlueprintCallable)
	void HideOutline()
	{
		OutlineMesh->SetHiddenInGame(true);
		OutlineMesh->SetVisibility(false);
	}
	UFUNCTION(BlueprintCallable)
	void ShowOutline()
	{
		OutlineMesh->SetHiddenInGame(false);
		OutlineMesh->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	void ActivateParticleEmitter();
	UFUNCTION(BlueprintCallable)
	void DeactivateParticleEmitter();
	UFUNCTION(BlueprintCallable)
	void SetParticleTrailVisibility(bool bVisible);
	UFUNCTION(BlueprintCallable)
	void SetParticleTrailVisibile() {SetParticleTrailVisibility(true);}
	UFUNCTION(BlueprintCallable)
	void SetParticleTrailInvisibile() {SetParticleTrailVisibility(false);}
	
	/*UFUNCTION(BlueprintCallable)
	void ShowBillboard()
	{
		BillboardMaterialComponent->SetVisibility(true, true);
		BillboardMaterialComponent->SetHiddenInGame(false, true);
	}
	UFUNCTION(BlueprintCallable)
	void HideBillboard()
	{
		BillboardMaterialComponent->SetVisibility(false, true);
		BillboardMaterialComponent->SetHiddenInGame(true, true);
	}*/
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UFUNCTION(BlueprintCallable)
	void OnTransitionToSurfaceView();
	UFUNCTION(BlueprintCallable)
	void OnTransitionToSpaceView();
protected:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	double Mass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	FVector VelocityVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	FVector AccelerationVector;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double OrbitalSpeed; // Scalar magnitude of Velocity Vector
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Motion")
	double AccelerationMagnitude; // Scalar magnitude of Acceleration Vector

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double MeanRadius; // Mean Radius of the body. (1.0 = Mean Radius of Earth)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double Obliquity; // Axial Tilt (Degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astro")
	double SiderealRotationPeriod; // The period in days for the body to make one full 360 degree revolution about its polar axis

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> OutlineMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionSphere;
	
	// Arrows
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* AccelerationArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* VelocityArrow;

	// Trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TrailSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
	// Scene Capture Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;
	
	// Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Icon")
	TObjectPtr<UTextureRenderTarget2D> IconRenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon")
	UMaterialInterface* IconMaterialBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Icon")
	UMaterialInstanceDynamic* IconMaterialInstance;
	

	// Reference to Orbit
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AOrbit> Orbit;
	
	// Used to position Spherical Coordinates (Latitude and Longitude)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> GeoCoordinateArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> HorizonSphere;
	
	// Billboard
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	TObjectPtr<UBillboardComponent> BillboardComponent;*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	TObjectPtr<UMaterialBillboardComponent> BillboardMaterialComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UMaterialInterface* BillboardMaterialBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UMaterialInstanceDynamic* BillboardMaterialInstance;

	UFUNCTION(BlueprintCallable, Category="Billboard")
	void CreateBillboardMaterialInstance();*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BodyName;
public:
	UFUNCTION(BlueprintCallable)
	FString GetBodyName() const {return BodyName;};
	UFUNCTION(BlueprintCallable)
	void SetBodyName(const FString NewName) { BodyName = NewName;};
	
	// Implement SelectableInterface
	virtual void Select() override;
	virtual void Deselect() override;
};
