// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimGameMode.h"

#include "SimPlayerController.h"
#include "W_MainHUD.h"
#include "Kismet/GameplayStatics.h"
//#include "OrbitalPlane.h"

void ASimGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	
}

void ASimGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(IsValid(HUDWidgetClass));

	ASimPlayerController* SimPlayerController = Cast<ASimPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(!IsValid(SimPlayerController)) return;

	// Create HUD and add to viewport (use the subclass set in the editor)
	MainHUD = Cast<UW_MainHUD>(CreateWidget(SimPlayerController, HUDWidgetClass, FName("Main HUD")));
	MainHUD->AddToViewport();

	
}
