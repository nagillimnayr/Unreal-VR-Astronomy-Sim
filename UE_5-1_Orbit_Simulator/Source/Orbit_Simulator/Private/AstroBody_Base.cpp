// Fill out your copyright notice in the Description page of Project Settings.


#include "AstroBody_Base.h"

// Sets default values
AAstroBody_Base::AAstroBody_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAstroBody_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAstroBody_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAstroBody_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

