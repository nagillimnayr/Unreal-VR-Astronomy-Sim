// Fill out your copyright notice in the Description page of Project Settings.

#include "AstroBody.h"

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ObservationPoint.h"
#include "GameFramework/SpringArmComponent.h"
#include "../CalculateOrbitalElements/OrbitalElements.h"
#include "BaseGizmos/ScalableSphereGizmo.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "Observer.h"
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
		AccelerationArrow->bUseInEditorScaling = false;
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
		VelocityArrow->bUseInEditorScaling = false;
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

	// Initialize SceneCaptureComponent
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
	SceneCaptureComponent->SetupAttachment(Camera);
	// Use Show Only list
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent->FOVAngle = 30.0;
	// Render Target
	IconRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("Icon Render Target"));
	

	
	// Initialize GeoCoordinateArm
	GeoCoordinateArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Geo-Coordinate Arm"));
	GeoCoordinateArm->SetupAttachment(SceneRoot);
	GeoCoordinateArm->bDoCollisionTest = false;
	GeoCoordinateArm->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, 0.0, 180.0)));
	
	HorizonSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Horizon Sphere"));
	HorizonSphere->SetupAttachment(GeoCoordinateArm);
	HorizonSphere->InitSphereRadius(10.0);
	//DrawSphere = CreateDefaultSubobject<UDrawSphereComponent>(TEXT("Draw Sphere"));
	//DrawSphere->SetupAttachment(GeoCoordinateArm);
	//SphereGizmo = CreateDefaultSubobject<UScalableSphereGizmo>(TEXT("Gizmo Sphere"));
	//SphereGizmo->SetupAttachment(GeoCoordinateArm);
}


// Called when the game starts or when spawned
void AAstroBody::BeginPlay()
{
	Super::BeginPlay();
	CreateIcon();
	SetActorEnableCollision(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	
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
		const double DynamicLifetime = 10.0 / (VelocityVector.Length() * Unit::KM_TO_M / Unit::DISTANCE_MULT);
		TrailComponent->SetNiagaraVariableFloat(FString("User.DynamicLifetime"), DynamicLifetime);
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
	/*SphereMesh->SetRelativeScale3D(FVector(1.0));
	//SetActorScale3D(FVector(MeanRadius / (Unit::DISTANCE_MULT * 100.0))); // Set size of Body
	SetActorScale3D(FVector(MeanRadius)); // Set size of Body*/
	SetActorRelativeScale3D(FVector(1.0));
	SphereMesh->SetWorldScale3D(FVector(MeanRadius)); // Set size of sphere
	OutlineMesh->SetWorldScale3D(FVector(MeanRadius)); // Set size of sphere

	// SceneCaptureComponent
	SceneCaptureComponent->ShowOnlyActors.Empty();
	SceneCaptureComponent->ClearShowOnlyComponents(); // Clear ShowOnlyComponents List
	SceneCaptureComponent->ShowOnlyComponent(SphereMesh); // Add SphereMesh to ShowOnlyComponents List
	SceneCaptureComponent->bCaptureEveryFrame = false; // Dont capture every frame
	SceneCaptureComponent->bCaptureOnMovement = false; // Dont capture on movement

	// Initialize Icon Render Target
	IconRenderTarget->InitCustomFormat(256, 256, EPixelFormat::PF_FloatRGBA, false);
	IconRenderTarget->bAutoGenerateMips = true;
	IconRenderTarget->CompressionSettings = TextureCompressionSettings::TC_Default;
	SceneCaptureComponent->TextureTarget = IconRenderTarget;

	CreateIcon();
	
	//SceneCaptureComponent->TextureTarget = RenderTarget;
	
	// Disable shadows
	SphereMesh->CastShadow = false;
	
	// Enable Collision
	SetActorEnableCollision(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	CollisionSphere->SetCollisionObjectType(COLLISION_SELECTION_CHANNEL);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(COLLISION_SELECTION_CHANNEL, ECollisionResponse::ECR_Block);
	
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

	// Velocity Arrow
	VelocityArrow->SetRelativeRotation(FRotator::MakeFromEuler(GetActorRightVector()));
	VelocityArrow->SetVisibility(false);
	VelocityArrow->SetVisibility(true);

	// Camera
	CameraBoom->TargetArmLength = 400.0 * MeanRadius;
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0, -30.0, 180.0)));

	// GeoCoordinateArm
	GeoCoordinateArm->TargetArmLength = MeanRadius * 100.0;
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
	
}

void AAstroBody::PostLoad()
{
	Super::PostLoad();


}

void AAstroBody::SetColor(const FLinearColor NewColor)
{
	Color = NewColor;
	if(IsValid(TrailComponent))
	{
		TrailComponent->SetNiagaraVariableLinearColor(FString("System.DynamicColor"), Color);
	}
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
		GLog->Log(">Error! Icon Render Target is invalid!");
		GEngine->AddOnScreenDebugMessage(91, 5.0, FColor::Yellow, TEXT(">Error! Icon Render Target is invalid!"));
	}
	// Capture the scene
	SceneCaptureComponent->CaptureScene();
	if(!IsValid(IconMaterialBase)) {return;}
	if(!IsValid(IconMaterialInstance))
	{
		IconMaterialInstance = UMaterialInstanceDynamic::Create(IconMaterialBase, nullptr);
		IconMaterialInstance->SetTextureParameterValue(FName("Texture"), IconRenderTarget);
	}
}

void AAstroBody::GoToSurface(AObservationPoint* ObservationPoint)
{
	ObservationPoint->AttachToComponent(GeoCoordinateArm, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		false
	));
}

void AAstroBody::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Initialize();
}

void AAstroBody::PositionGeoCoords(const double Latitude, const double Longitude)
{
	
}

void AAstroBody::Select()
{
	OutlineMesh->SetVisibility(true);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, FString("Selecting: " + GetActorLabel()));

}

void AAstroBody::Deselect()
{
	OutlineMesh->SetVisibility(false);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, FString("Deselecting: " + GetActorLabel()));

}

// Called every frame
void AAstroBody::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAstroBody::PostInitProperties()
{
	Super::PostInitProperties();

}

#if WITH_EDITOR
void AAstroBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
