// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ObservationPoint.h"
#include "GameFramework/SpringArmComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
//#include "BaseGizmos/ScalableSphereGizmo.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
//#include "Observer.h"
#include "SimPlayerController.h"
//#include "Components/BillboardComponent.h"
//#include "Components/MaterialBillboardComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "TextureRenderTarget2D.h";

// Define collision channel for selection
#define COLLISION_SELECTION_CHANNEL ECC_GameTraceChannel1

// Sets default values
AAstroBody::AAstroBody() :
Mass(0.0),
VelocityVector(FVector::ZeroVector),
AccelerationVector(FVector::ZeroVector),
MeanRadius(1.0),
Color(FLinearColor::White)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Scene Root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	SetRootComponent(SceneRoot);

	// Initialize Sphere Mesh
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(SceneRoot);
	SphereMesh->CastShadow = false; // Disable shadows
	// Initialize Outline Mesh
	OutlineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutlineMesh"));
	OutlineMesh->SetupAttachment(SphereMesh);
	OutlineMesh->CastShadow = false; // Disable shadows

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Orbit_Sim/Meshes/Equirectangular_Sphere.Equirectangular_Sphere'"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		SphereMesh->SetRelativeLocation(FVector::ZeroVector);
		OutlineMesh->SetStaticMesh(SphereMeshAsset.Object);
		OutlineMesh->SetRelativeLocation(FVector::ZeroVector);
	}
	//StaticSphereMesh->SetWorldScale3D(FVector(Radius, Radius, Radius)); // Set size of Sphere
	// Set Materials
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineMat(TEXT("//Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/MI_Extrude_Outline.MI_Extrude_Outline'"));
	if(OutlineMat.Succeeded())
	{
		// Create Dynamic Material Instance for Outline material
		/*UMaterialInstanceDynamic* OutlineMatDyn = UMaterialInstanceDynamic::Create(OutlineMat.Object, nullptr, FName("Dynamic Outline Material"));
		OutlineMesh->SetMaterial(0, OutlineMatDyn);*/
		
		// Create Outline material
		OutlineMesh->SetMaterial(0, OutlineMat.Object);
	}
	OutlineMesh->SetVisibility(false);

	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMatAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Orbit_Sim/Materials/Misc_Materials/MI_Icon.MI_Icon'"));
	if(IconMatAsset.Succeeded())
	{
		IconMaterialBase = IconMatAsset.Object;
	}

	// Initialize Collision Sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetSphereRadius(102);
	CollisionSphere->SetupAttachment(SphereMesh);

	// Initialize arrows
	AccelerationArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Acceleration-Arrow"));
	if (AccelerationArrow)
	{
		AccelerationArrow->SetupAttachment(SceneRoot);
		AccelerationArrow->SetVisibility(true, true);
		AccelerationArrow->SetHiddenInGame(false, true);
		AccelerationArrow->SetArrowColor(FLinearColor::Red);
		AccelerationArrow->SetRelativeLocation(FVector::ZeroVector);
		AccelerationArrow->SetWorldLocation(GetActorLocation());
		AccelerationArrow->ArrowSize = 2.0;
		AccelerationArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		AccelerationArrow->ArrowLength = 75.0;
	}
	
	VelocityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Velocity-Arrow"));
	if (VelocityArrow)
	{
		VelocityArrow->SetupAttachment(SceneRoot);
		VelocityArrow->SetVisibility(true, true);
		VelocityArrow->SetHiddenInGame(false, true);
		VelocityArrow->SetArrowColor(FLinearColor::Green);
		VelocityArrow->SetRelativeLocation(FVector::ZeroVector);
		VelocityArrow->ArrowSize = 2.0;
		VelocityArrow->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
		VelocityArrow->ArrowLength = 75.0;
	}

	// Set collision
	SetActorEnableCollision(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//SphereMesh->SetCollisionObjectType(COLLISION_SELECTION_CHANNEL);
	//SphereMesh->SetCollisionResponseToChannel(COLLISION_SELECTION_CHANNEL, ECollisionResponse::ECR_Block);
	
	
	// Initialize Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(SceneRoot);
	Camera->SetupAttachment(CameraBoom);
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 0.0)));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = true;

	// Initialize SceneCaptureComponent
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
	// Use Show Only list
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent->FOVAngle = 30.0;
	/*SceneCaptureArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Scene Capture Arm"));
	SceneCaptureArm->SetupAttachment(SceneRoot);*/
	//SceneCaptureComponent->SetupAttachment(SceneCaptureArm);
	SceneCaptureComponent->SetupAttachment(CameraBoom);
	/*SceneCaptureArm->bDoCollisionTest = false;
	SceneCaptureArm->bUsePawnControlRotation = false;*/

	// Render Target
	//IconRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("Icon Render Target"));
	//SceneCaptureComponent->TextureTarget = IconRenderTarget;

	
	// Initialize GeoCoordinateArm
	GeoCoordinateArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Geo-Coordinate Arm"));
	GeoCoordinateArm->SetupAttachment(SceneRoot);
	GeoCoordinateArm->bDoCollisionTest = false;
	//GeoCoordinateArm->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, 0.0, 180.0)));
	
	HorizonSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Horizon Sphere"));
	HorizonSphere->SetupAttachment(GeoCoordinateArm);
	HorizonSphere->InitSphereRadius(10.0);

	// Billboard
	/*BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(SceneRoot);*/
	/*BillboardMaterialComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard Material Component"));
	BillboardMaterialComponent->SetupAttachment(SceneRoot);*/
	
}


// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	CreateIcon();
	SetActorEnableCollision(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	HorizonSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Initialize Niagara System/ Component
	if (TrailSystem)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, SphereMesh, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
			false);
		TrailComponent->SetVisibility(true);
		TrailComponent->SetHiddenInGame(false);
		TrailComponent->SetNiagaraVariableLinearColor(FString("User.DynamicColor"), Color);
		GLog->Log("> Setting Trail color...");

		/*// Set Dynamic Lifetime
		const double DynamicLifetime = 100.0 / (VelocityVector.Length() * Unit::KM_TO_M / Unit::DISTANCE_MULT);
		TrailComponent->SetNiagaraVariableFloat(FString("User.DynamicLifetime"), DynamicLifetime);*/

		// Set Dynamic Size
		const double DynamicSize = 30.0 * MeanRadius;
		TrailComponent->SetNiagaraVariableFloat(FString("User.DynamicSize"), DynamicSize);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error initializing Niagara System"));
		GLog->Log("> Error initializing Niagara System!");
	}
	
	SphereMesh->CastShadow = false; // Disable shadows
	AccelerationArrow->SetHiddenInGame(true);
	VelocityArrow->SetHiddenInGame(true);

	
}

void AAstroBody::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Initialize();

}

void AAstroBody::Initialize()
{
	// Set sphere scale
	SetMeanRadius(MeanRadius);

	
	// Camera
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 180.0)));
	CameraBoom->bUsePawnControlRotation = false;
	
	
	// Disable shadows
	SphereMesh->CastShadow = false;
	
	// Enable Collision
	SetActorEnableCollision(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	CollisionSphere->SetCollisionObjectType(COLLISION_SELECTION_CHANNEL);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(COLLISION_SELECTION_CHANNEL, ECollisionResponse::ECR_Block);
	OutlineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OutlineMesh->bHiddenInSceneCapture = true;
	HorizonSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HorizonSphere->bHiddenInSceneCapture = true;
	
	/*AccelerationArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AccelerationArrow->SetRelativeLocation(FVector::ZeroVector);
	AccelerationArrow->ArrowLength = 75 + (25 * Radius);
	
	VelocityArrow->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	VelocityArrow->SetRelativeLocation(FVector::ZeroVector);
	VelocityArrow->ArrowLength = 75 + (25 * Radius);*/
	
	// Acceleration Arrow
	AccelerationArrow->SetRelativeRotation(FRotator::ZeroRotator);
	AccelerationArrow->SetVisibility(false);
	AccelerationArrow->SetVisibility(true);
	AccelerationArrow->bHiddenInSceneCapture = true;

	// Velocity Arrow
	VelocityArrow->SetRelativeRotation(FRotator::MakeFromEuler(GetActorRightVector()));
	VelocityArrow->SetVisibility(false);
	VelocityArrow->SetVisibility(true);
	VelocityArrow->bHiddenInSceneCapture = true;

	
	// SceneCaptureComponent
	SceneCaptureComponent->ShowOnlyActors.Empty();
	SceneCaptureComponent->ClearShowOnlyComponents(); // Clear ShowOnlyComponents List
	SceneCaptureComponent->ShowOnlyComponent(SphereMesh); // Add SphereMesh to ShowOnlyComponents List
	SceneCaptureComponent->ShowOnlyActors.Add(this);
	SceneCaptureComponent->bCaptureEveryFrame = false; // Dont capture every frame
	SceneCaptureComponent->bCaptureOnMovement = false; // Dont capture on movement
	//SceneCaptureArm->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, 0.0, 180.0)));

	// Initialize Icon Render Target
	if(!IconRenderTarget)
	{
		IconRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	}
	IconRenderTarget->InitCustomFormat(256, 256, EPixelFormat::PF_FloatRGBA, false);
	IconRenderTarget->bAutoGenerateMips = true;
	IconRenderTarget->CompressionSettings = TextureCompressionSettings::TC_Default;
	SceneCaptureComponent->TextureTarget = IconRenderTarget;

	// Subscribe to delegates
	if(ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		// Hide Outline when in Surface View
		SimPlayerController->OnTransitionToSurfaceDelegate.AddUniqueDynamic(this, &AAstroBody::OnTransitionToSurfaceView);
		/*SimPlayerController->OnTransitionToSurfaceDelegate.AddUniqueDynamic(this, &AAstroBody::HideOutline);
		SimPlayerController->OnTransitionToSurfaceDelegate.AddUniqueDynamic(this, &AAstroBody::SetParticleTrailInvisibile);
		SimPlayerController->OnTransitionToSurfaceDelegate.AddUniqueDynamic(this, &AAstroBody::ShowBillboard);*/


		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &AAstroBody::OnTransitionToSpaceView);
		/*SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &AAstroBody::ShowOutline);
		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &AAstroBody::SetParticleTrailVisibile);
		SimPlayerController->OnTransitionToSpaceDelegate.AddUniqueDynamic(this, &AAstroBody::HideBillboard);*/

	}

	// Position GeoCoordArm
	GeoCoordinateArm->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -45.0, 0.0)));

	HorizonSphere->SetHiddenInGame(true);

	/*CreateBillboardMaterialInstance();

	BillboardMaterialComponent->MinDrawDistance = MeanRadius * 200.0;
	BillboardMaterialComponent->SetVisibility(false);*/
}


/*
void AAstroBody::CalculateAcceleration(const AAstroBody* const OtherBody)
{
	const FVector OtherPos = OtherBody->GetActorLocation();
	FVector ThisPos = GetActorLocation();
	FVector Difference = OtherPos - ThisPos;
	double DistanceSquared = (Difference * Unit::DISTANCE_MULT).SquaredLength(); // Multiply by DISTANCE_MULTIPLIER
	//FVector Direction = Difference.GetSafeNormal(1.0); // Normalized Direction Vector pointing from this Body to the other Body
	FVector Direction = Difference;
	Direction.Normalize(); // Normalized Direction Vector pointing from this Body to the other Body
	
	// Calculate gravitational force
	double Force = (Unit::GRAVITATIONAL_CONSTANT * OtherBody->GetMassOfBody() * Unit::SOLAR_MASS) / DistanceSquared;
	AccelerationVector = Direction * Force;

	AccelerationMagnitude = Force;
	
	UpdateAccelerationArrow();
	//AccelerationArrow->ArrowLength = FMath::Max(0.1, AccelerationMagnitude);
}*/

void AAstroBody::SetAcceleration(const FVector NewAcceleration)
{
	AccelerationVector = NewAcceleration;
	AccelerationMagnitude = AccelerationVector.Length();
	UpdateAccelerationArrow();
}

void AAstroBody::UpdateVelocity(const FVector NewVelocity)
{
	VelocityVector = NewVelocity;
	OrbitalSpeed = VelocityVector.Length();
	UpdateVelocityArrow();
	//VelocityArrow->ArrowLength = FMath::Max(0.1, OrbitalSpeed);
}

void AAstroBody::UpdatePosition(const double DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update Position based on Velocity
	// Divide by DISTANCE_MULTIPLIER so the result will be in in-editor units
	Position += VelocityVector * (DeltaTime * Unit::SECONDS_IN_DAY / Unit::DISTANCE_MULT);
	const bool bCheck = SetActorLocation(Position);
	
	if (!bCheck)
	{
		GLog->Log("> Error Updating Body!!!");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Error Updating Position"));
	}
}

void AAstroBody::UpdateVelocityArrow()
{
	if(!VelocityArrow) {return;}
	const FRotator Rotation = FRotationMatrix::MakeFromX(VelocityVector).Rotator();
	VelocityArrow->SetWorldRotation(Rotation);
	VelocityArrow->SetVisibility(false);
	VelocityArrow->SetVisibility(true);
}

void AAstroBody::UpdateAccelerationArrow()
{
	if(!AccelerationArrow) { return; }
	const FRotator Rotation = FRotationMatrix::MakeFromX(AccelerationVector).Rotator();
	AccelerationArrow->SetWorldRotation(Rotation);
	AccelerationArrow->SetVisibility(false);
	AccelerationArrow->SetVisibility(true);
}

void AAstroBody::AimAccelerationArrow(const AActor* const Target)
{
	const FVector Direction = Target->GetActorLocation() - GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	AccelerationArrow->SetWorldRotation(Rotation);
	AccelerationArrow->SetVisibility(false);
	AccelerationArrow->SetVisibility(true);

	//SceneCaptureArm->SetWorldRotation(Rotation); 
}


void AAstroBody::SetMeanRadius(const double NewMeanRadius)
{
	MeanRadius = NewMeanRadius;
	//SetActorRelativeScale3D(FVector(1.0));

	double Multiplier = 1.0;
	
	/*double AdjustedScale = MeanRadius / (Unit::DISTANCE_MULT * 100.0);
	SphereMesh->SetWorldScale3D(FVector(AdjustedScale)); // Set size of Sphere
	OutlineMesh->SetWorldScale3D(FVector(AdjustedScale)); // Set size of Sphere*/
	SphereMesh->SetWorldScale3D(FVector(MeanRadius * Multiplier)); // Set size of Sphere
	OutlineMesh->SetWorldScale3D(FVector(MeanRadius * Multiplier)); // Set size of Sphere

	//const double AdjustedRadius = (MeanRadius / Unit::DISTANCE_MULT); // Adjust value to be in editor units
	//CameraBoom->TargetArmLength = AdjustedRadius * 3;
	//SceneCaptureArm->TargetArmLength = AdjustedRadius * 3;
	
	//GeoCoordinateArm->TargetArmLength = AdjustedRadius;

	const double AdjustedRadius = MeanRadius * 100.0 * Multiplier;
	CameraBoom->TargetArmLength = AdjustedRadius * 3.0  * Multiplier;
	GeoCoordinateArm->TargetArmLength = AdjustedRadius  * Multiplier;
	
	// Update Billboard Sprite
	//UpdateBillboardSpriteScreenSpace();
	//UpdateBillboardSpriteAbsolute();
}

void AAstroBody::SetObliquity(const double NewObliquity)
{
	Obliquity = NewObliquity;
	const FRotator Rotation = FRotator::MakeFromEuler(FVector(Obliquity, 0.0, 0.0));
	SphereMesh->SetRelativeRotation(Rotation);
}

void AAstroBody::SetColor(const FLinearColor NewColor)
{
	Color = NewColor;
	if(IsValid(TrailComponent))
	{
		TrailComponent->SetNiagaraVariableLinearColor(FString("System.DynamicColor"), Color);
	}

	/*if(!BillboardMaterialInstance)
	{
		CreateBillboardMaterialInstance();
	}
	else
	{
		BillboardMaterialInstance->SetVectorParameterValue(TEXT("Color"), Color);
	}*/
	
	/*if(!IsValid(OutlineMesh))
	{
		GLog->Log("> Error!: OutlineMesh is invalid!");
		return;
	}
	UMaterialInterface* OutlineMat = OutlineMesh->GetMaterial(0);
	if(!IsValid(OutlineMat))
	{
		GLog->Log("> Error!: OutlineMat is invalid!");
		return;
	}
	UMaterialInstanceDynamic* OutlineMatDyn = Cast<UMaterialInstanceDynamic>(OutlineMat);
	if(IsValid(OutlineMatDyn))
	{
		// Set Outline Color
		OutlineMatDyn->SetVectorParameterValue(FName("Color"), Color);
	}*/
}

/*void AAstroBody::UpdateBillboardSpriteScreenSpace()
{
	// Billboard Material
	double AspectRatio = 1.7777;
	// Get Player Camera Manager
	if(APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		// Get Aspect Ratio
		AspectRatio = CameraManager->GetCameraCacheView().AspectRatio;
	}
	//BillboardMaterialComponent->Elements.Empty();
	// Adjust size of Sprite
	double BaseSizeX = 0.001 * MeanRadius / Unit::EARTH_RADIUS;
	//double BaseSizeX = 0.01 * AdjustedRadius / Unit::EARTH_RADIUS;
	//double BaseSizeX = 0.01 * AdjustedRadius / Unit::EARTH_RADIUS;
	double BaseSizeY = BaseSizeX * AspectRatio;
	
	
	
	//CreateBillboardMaterialInstance();
	
	// Set Material to Dynamic Instance
	BillboardMaterialComponent->SetMaterial(0, BillboardMaterialInstance);

	// Add sprite
	BillboardMaterialComponent->AddElement(BillboardMaterialInstance, nullptr, true, BaseSizeX, BaseSizeY, nullptr);
	//BillboardMaterialComponent->MinDrawDistance = MeanRadius * 5.0;
	
}
void AAstroBody::UpdateBillboardSpriteAbsolute()
{
	/#1#/ Billboard Material
	double AspectRatio = 1.7777;
	// Get Player Camera Manager
	if(APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		// Get Aspect Ratio
		AspectRatio = CameraManager->GetCameraCacheView().AspectRatio;
	}#1#
	BillboardMaterialComponent->Elements.Empty();
	// Adjust size of Sprite
	//double BaseSizeX = 10 * MeanRadius / Unit::EARTH_RADIUS;
	//double BaseSizeX = 0.01 * AdjustedRadius / Unit::EARTH_RADIUS;
	double BaseSizeX = 100 * MeanRadius;
	double BaseSizeY = BaseSizeX;
	
	
	CreateBillboardMaterialInstance();
	
	// Set Material to Dynamic Instance
	BillboardMaterialComponent->SetMaterial(0, BillboardMaterialInstance);

	// Add sprite
	BillboardMaterialComponent->AddElement(BillboardMaterialInstance, nullptr, false, BaseSizeX, BaseSizeY, nullptr);
	//BillboardMaterialComponent->MinDrawDistance = MeanRadius * 5.0;
	
}
*/

UTextureRenderTarget2D* AAstroBody::GetIconRenderTarget()
{
	// Return the RenderTarget
	return IconRenderTarget;
}

UMaterialInstanceDynamic* AAstroBody::GetIconMaterial()
{
	if(!IsValid(IconMaterialInstance))
	{
		CreateIcon();
	}
	return IconMaterialInstance;
}

void AAstroBody::CreateIcon()
{
	if(!IsValid(IconRenderTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT(">Error! Icon Render Target is invalid!"));
		GEngine->AddOnScreenDebugMessage(91, 5.0, FColor::Yellow, TEXT(">Error! Icon Render Target is invalid!"));
	}
	// Capture the scene
	SceneCaptureComponent->CaptureScene();
	
	// Create Material Instance Dynamic
	if(!IsValid(IconMaterialBase)) {return;}
	if(!IsValid(IconMaterialInstance))
	{
		IconMaterialInstance = UMaterialInstanceDynamic::Create(IconMaterialBase, nullptr);
	}
	IconMaterialInstance->SetTextureParameterValue(FName("Texture"), IconRenderTarget);
}

void AAstroBody::SetObservationPoint(AObservationPoint* ObservationPoint)
{
	ObservationPoint->AttachToComponent(HorizonSphere, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		true
	));

	// Orient
	const FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0, 90.0, 0.0));
	ObservationPoint->SetActorRelativeRotation(Rotation);
	ObservationPoint->GetRootComponent()->SetVisibility(true);
}

void AAstroBody::RemoveObservationPoint()
{
	TArray<TObjectPtr<USceneComponent>> AttachedObjects = GeoCoordinateArm->GetAttachChildren();
	
	for(TObjectPtr<USceneComponent> SceneComponent : AttachedObjects)
	{
		// Get Outer and Cast to Observation Point
		AObservationPoint* ObservationPoint = Cast<AObservationPoint>(SceneComponent->GetOuter());
		if(!ObservationPoint) continue; // If Cast failed, continue

		// Detach Scene Component From Arm
		SceneComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ObservationPoint->Destroy(); // Destroy the Observation Point
	}
}

void AAstroBody::ActivateParticleEmitter()
{
	if(TrailComponent)
	{
		TrailComponent->Activate(true);
	}
}

void AAstroBody::DeactivateParticleEmitter()
{
	if(TrailComponent)
	{
		TrailComponent->DeactivateImmediate();
	}
}

void AAstroBody::SetParticleTrailVisibility(bool bVisible)
{
	if(TrailComponent)
	{
		TrailComponent->SetVisibility(bVisible);
	}
}

void AAstroBody::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CameraBoom->bUsePawnControlRotation = true;
}

void AAstroBody::UnPossessed()
{
	Super::UnPossessed();

	CameraBoom->bUsePawnControlRotation = false;
}

void AAstroBody::OnTransitionToSurfaceView()
{
	SetParticleTrailVisibility(false);
	//BillboardMaterialComponent->SetVisibility(true, true);
	/*BillboardMaterialComponent->SetVisibility(false, true);
	BillboardMaterialComponent->SetHiddenInGame(false, true);*/
	SphereMesh->SetVisibility(false, false);
	SphereMesh->SetHiddenInGame(true, false);
	HideOutline();
}

void AAstroBody::OnTransitionToSpaceView()
{
	SetParticleTrailVisibility(true);
	/*BillboardMaterialComponent->SetVisibility(false, true);
	BillboardMaterialComponent->SetHiddenInGame(true, true);*/
	SphereMesh->SetVisibility(true, false);
	SphereMesh->SetHiddenInGame(false, false);
	ShowOutline();
}

void AAstroBody::PositionGeoCoords(const double Latitude, const double Longitude)
{
	
}

/*
void AAstroBody::CreateBillboardMaterialInstance()
{
	if(BillboardMaterialBase && !BillboardMaterialInstance)
	{
		// Create Dynamic Material Instance
		BillboardMaterialInstance = UMaterialInstanceDynamic::Create(BillboardMaterialBase, nullptr);
	}
	if(BillboardMaterialInstance)
	{
		// Set Color
		BillboardMaterialInstance->SetVectorParameterValue(TEXT("Color"), Color);
	}
}
*/


void AAstroBody::Select()
{
	OutlineMesh->SetVisibility(true);

}

void AAstroBody::Deselect()
{
	OutlineMesh->SetVisibility(false);

}

// Called every frame
void AAstroBody::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

