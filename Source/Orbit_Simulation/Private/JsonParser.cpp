// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParser.h"
#include "Dom/JsonObject.h"


// Sets default values for this component's properties
UJsonParser::UJsonParser()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	JsonObject = MakeShareable(new FJsonObject());
	JsonObjectField = MakeShareable(new FJsonObject());
}


// Called when the game starts
void UJsonParser::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJsonParser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UJsonParser::DeserializeFile()
{
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFile/" + FileName + ".json";
	FString JsonString;

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
	// Display file contents to output log
	GLog->Log(FileName + ".json:");
	GLog->Log(JsonString);

	//TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		if(JsonObject->HasField(ObjectName))
		{
			// Get Object Field from json file
			JsonObject = JsonObject->GetObjectField(ObjectName);
			return JsonObject.IsValid();
		}
		else
		{
			GLog->Log("Error: " + ObjectName + " field was not found");
		}
	}
	else
	{
		GLog->Log("Error: Could not deserialize " + FileName + ".json");
	}
	
	return false;
}

TSharedPtr<FJsonObject> UJsonParser::DeserializeFile(const FString TargetFileName, const FString TargetObjectName)
{
	FileName = TargetFileName;
	ObjectName = TargetObjectName;
	
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/" + FileName + ".json";
	FString JsonString;

	// Load file into FString
	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
	
	// Display file contents to output log
	UE_LOG(LogTemp, Log ,TEXT("%s.json:"), *FileName);
	//UE_LOG(LogTemp, Log ,TEXT("%s"), *JsonString);
	//GLog->Log(JsonString);

	//TSharedPtr<FJsonObject> JsonObjectField = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	
	if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		if(JsonObject->HasField(ObjectName))
		{
			// Get Object Field from json file
			JsonObjectField = JsonObject->GetObjectField(ObjectName);
		}
		else
		{
			//GLog->Log("Error: " + ObjectName + " field was not found");
			UE_LOG(LogTemp, Warning ,TEXT("Error: %s object was not found"), *ObjectName);
		}
	}
	else
	{
		//GLog->Log("Error: Could not deserialize " + FileName + ".json");
		UE_LOG(LogTemp, Warning ,TEXT("Error: Could not deserialize %s.json"), *FileName);
	}
	/*if (!JsonObjectField.IsValid())
	{
		JsonObjectField = MakeShareable(new FJsonObject());
	}*/
	return JsonObjectField;
}

