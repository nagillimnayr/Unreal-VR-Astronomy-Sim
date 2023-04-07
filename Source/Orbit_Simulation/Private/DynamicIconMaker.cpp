// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicIconMaker.h"

void UDynamicIconMaker::InitializeComponent()
{
	Super::InitializeComponent();
	
	
}

UTextureRenderTarget2D* UDynamicIconMaker::GetIconRenderTarget()
{
	return IconRenderTarget;
}

void UDynamicIconMaker::CreateIcon()
{
	
}
