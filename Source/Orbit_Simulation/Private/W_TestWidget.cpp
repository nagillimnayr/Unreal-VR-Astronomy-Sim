// Fill out your copyright notice in the Description page of Project Settings.


#include "W_TestWidget.h"

#include "AstroBody.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"


void UW_TestWidget::Init()
{
	if(TextName)
	{
		TextName->SetText(FText::FromString(FString("Hello, World!")));
	}
	if(IsValid(Body))
	{
		if(TextName)
		{
			TextName->SetText(FText::FromString(Body->GetActorNameOrLabel()));
		}
		if(ImageIcon)
		{
			UTextureRenderTarget2D* RenderTarget = Body->GetIconRenderTarget();
			if(!IsValid(RenderTarget))
			{
				GLog->Log("> Invalid Render Target!");
			}
			else
			{
				/*UTexture2D* Texture = RenderTarget->ConstructTexture2D(
					this,
					FString("Icon Texture"),
					EObjectFlags::RF_NoFlags
					);
				ImageIcon->SetBrushFromTexture(Texture);*/

				
				/*
				// Get Pixel data from Render Target
				//ETextureSourceFormat TextureSourceFormat = RenderTarget->GetTextureFormatForConversionToTexture2D();
				FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
				const uint32 SizeX = RenderTargetResource->GetSizeX();
				const uint32 SizeY = RenderTargetResource->GetSizeY();
				TArray<FColor> PixelData;
				PixelData.Reserve(SizeX*SizeY);
				RenderTargetResource->ReadPixels(PixelData);
				const EPixelFormat PixelFormat = RenderTarget->GetFormat();
				
				// Create dynamic texture
				/*UTexture2DDynamic* DynamicTexture = UTexture2DDynamic::Create(SizeX, SizeY,
					FTexture2DDynamicCreateInfo(PixelFormat));#1#

				// Create transient texture
				IconTexture = UTexture2D::CreateTransient(SizeX, SizeY, PixelFormat);
				IconTexture->SRGB = false;
				IconTexture->CompressionSettings  = TextureCompressionSettings::TC_Default;
				// Get first mipmap
				FTexture2DMipMap* MipMap = &IconTexture->GetPlatformData()->Mips[0];
				const int32 BufferSize = PixelData.Num() * 4; // 4 numbers per pixel, RGBA
				TArray<uint8> ColorBuffer;
				//ColorBuffer.Reserve(BufferSize);

				// Read pixel color data into Color Buffer
				/*for(int32 i = 0, Size = PixelData.Num(); i < Size; i++)
				{
					ColorBuffer.Append({PixelData[i].R, PixelData[i].G, PixelData[i].B, PixelData[i].A});
				}#1#
				
				// Memcpy
				//void* ColorBufferData = ColorBuffer.GetData();
				void* TextureData = MipMap->BulkData.Lock(LOCK_READ_WRITE);
				//MipMap.BulkData.Realloc(BufferSize);
				FMemory::Memcpy(TextureData, PixelData.GetData(), BufferSize);
				MipMap->BulkData.Unlock();

				//void* ColorBufferData = ColorBuffer.GetData();

				/#1#/ Get bulk data
				FByteBulkData* BulkData = &MipMap.BulkData;
				// Get raw data and lock bulk data
				uint8* RawImageData = static_cast<uint8*>(BulkData->Lock(LOCK_READ_WRITE));

				for(int32 i = 0; i < BufferSize; i++)
				{
					RawImageData[i] = ColorBuffer[i];
				}
				BulkData->Unlock();#1#
				IconTexture->UpdateResource();*/
				
				//ImageIcon->SetBrushFromTexture(IconTexture, true);
				ImageIcon->SetBrushFromMaterial(Body->GetIconMaterial());
				
			}
		}
	}
}

void UW_TestWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Init();
}

void UW_TestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}
