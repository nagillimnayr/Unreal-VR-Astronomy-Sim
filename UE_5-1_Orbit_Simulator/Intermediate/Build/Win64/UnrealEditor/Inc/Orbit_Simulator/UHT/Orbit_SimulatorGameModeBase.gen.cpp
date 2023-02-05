// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Orbit_Simulator/Orbit_SimulatorGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOrbit_SimulatorGameModeBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AOrbit_SimulatorGameModeBase();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AOrbit_SimulatorGameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Orbit_Simulator();
// End Cross Module References
	void AOrbit_SimulatorGameModeBase::StaticRegisterNativesAOrbit_SimulatorGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AOrbit_SimulatorGameModeBase);
	UClass* Z_Construct_UClass_AOrbit_SimulatorGameModeBase_NoRegister()
	{
		return AOrbit_SimulatorGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Orbit_Simulator,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Orbit_SimulatorGameModeBase.h" },
		{ "ModuleRelativePath", "Orbit_SimulatorGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOrbit_SimulatorGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::ClassParams = {
		&AOrbit_SimulatorGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AOrbit_SimulatorGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AOrbit_SimulatorGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AOrbit_SimulatorGameModeBase.OuterSingleton, Z_Construct_UClass_AOrbit_SimulatorGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AOrbit_SimulatorGameModeBase.OuterSingleton;
	}
	template<> ORBIT_SIMULATOR_API UClass* StaticClass<AOrbit_SimulatorGameModeBase>()
	{
		return AOrbit_SimulatorGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AOrbit_SimulatorGameModeBase);
	AOrbit_SimulatorGameModeBase::~AOrbit_SimulatorGameModeBase() {}
	struct Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_Orbit_SimulatorGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_Orbit_SimulatorGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AOrbit_SimulatorGameModeBase, AOrbit_SimulatorGameModeBase::StaticClass, TEXT("AOrbit_SimulatorGameModeBase"), &Z_Registration_Info_UClass_AOrbit_SimulatorGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AOrbit_SimulatorGameModeBase), 3700343360U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_Orbit_SimulatorGameModeBase_h_563685391(TEXT("/Script/Orbit_Simulator"),
		Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_Orbit_SimulatorGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_Orbit_SimulatorGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
