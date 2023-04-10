// Fill out your copyright notice in the Description page of Project Settings.


#include "Observer.h"

#include "Camera/CameraComponent.h"


// Sets default values
AObserver::AObserver()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

}

// Called when the game starts or when spawned
void AObserver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AObserver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}
