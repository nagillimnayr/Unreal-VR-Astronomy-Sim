// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Orbit_Simulator/Public/AstroBody_Base.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAstroBody_Base() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AAstroBody_Base();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AAstroBody_Base_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Orbit_Simulator();
// End Cross Module References
	void AAstroBody_Base::StaticRegisterNativesAAstroBody_Base()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AAstroBody_Base);
	UClass* Z_Construct_UClass_AAstroBody_Base_NoRegister()
	{
		return AAstroBody_Base::StaticClass();
	}
	struct Z_Construct_UClass_AAstroBody_Base_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AAstroBody_Base_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_Orbit_Simulator,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AAstroBody_Base_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "AstroBody_Base.h" },
		{ "ModuleRelativePath", "Public/AstroBody_Base.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AAstroBody_Base_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AAstroBody_Base>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AAstroBody_Base_Statics::ClassParams = {
		&AAstroBody_Base::StaticClass,
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
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AAstroBody_Base_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AAstroBody_Base_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AAstroBody_Base()
	{
		if (!Z_Registration_Info_UClass_AAstroBody_Base.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AAstroBody_Base.OuterSingleton, Z_Construct_UClass_AAstroBody_Base_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AAstroBody_Base.OuterSingleton;
	}
	template<> ORBIT_SIMULATOR_API UClass* StaticClass<AAstroBody_Base>()
	{
		return AAstroBody_Base::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AAstroBody_Base);
	AAstroBody_Base::~AAstroBody_Base() {}
	struct Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Unreal_3D_Orbit_Sim_UE_5_1_Orbit_Simulator_Source_Orbit_Simulator_Public_AstroBody_Base_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Unreal_3D_Orbit_Sim_UE_5_1_Orbit_Simulator_Source_Orbit_Simulator_Public_AstroBody_Base_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AAstroBody_Base, AAstroBody_Base::StaticClass, TEXT("AAstroBody_Base"), &Z_Registration_Info_UClass_AAstroBody_Base, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AAstroBody_Base), 4290744759U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Unreal_3D_Orbit_Sim_UE_5_1_Orbit_Simulator_Source_Orbit_Simulator_Public_AstroBody_Base_h_3315267432(TEXT("/Script/Orbit_Simulator"),
		Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Unreal_3D_Orbit_Sim_UE_5_1_Orbit_Simulator_Source_Orbit_Simulator_Public_AstroBody_Base_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Unreal_3D_Orbit_Sim_UE_5_1_Orbit_Simulator_Source_Orbit_Simulator_Public_AstroBody_Base_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
