// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimGameMode.generated.h"

class UW_MainHUD;
/**
 * 
 */
UCLASS()
class ORBIT_SIMULATION_API ASimGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	
	// Getters
	UFUNCTION(BlueprintCallable)
	UW_MainHUD* GetMainHUD() {return MainHUD;}
protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;
	
	/** Main Menu Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	/** Pause Menu Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	/** HUD Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UI")
	UW_MainHUD* MainHUD;
	
	
	/** The widget instance that we are currently using */
	UPROPERTY()
	UUserWidget* CurrentWidget;

	
};
