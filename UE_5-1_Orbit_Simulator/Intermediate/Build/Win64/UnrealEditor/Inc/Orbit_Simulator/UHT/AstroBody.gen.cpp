// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Orbit_Simulator/AstroBody.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAstroBody() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AAstroBody();
	ORBIT_SIMULATOR_API UClass* Z_Construct_UClass_AAstroBody_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Orbit_Simulator();
// End Cross Module References
	void AAstroBody::StaticRegisterNativesAAstroBody()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AAstroBody);
	UClass* Z_Construct_UClass_AAstroBody_NoRegister()
	{
		return AAstroBody::StaticClass();
	}
	struct Z_Construct_UClass_AAstroBody_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AAstroBody_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Orbit_Simulator,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AAstroBody_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "AstroBody.h" },
		{ "ModuleRelativePath", "AstroBody.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AAstroBody_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AAstroBody>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AAstroBody_Statics::ClassParams = {
		&AAstroBody::StaticClass,
		"Engine",
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
		METADATA_PARAMS(Z_Construct_UClass_AAstroBody_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AAstroBody_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AAstroBody()
	{
		if (!Z_Registration_Info_UClass_AAstroBody.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AAstroBody.OuterSingleton, Z_Construct_UClass_AAstroBody_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AAstroBody.OuterSingleton;
	}
	template<> ORBIT_SIMULATOR_API UClass* StaticClass<AAstroBody>()
	{
		return AAstroBody::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AAstroBody);
	AAstroBody::~AAstroBody() {}
	struct Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_AstroBody_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_AstroBody_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AAstroBody, AAstroBody::StaticClass, TEXT("AAstroBody"), &Z_Registration_Info_UClass_AAstroBody, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AAstroBody), 4134381802U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_AstroBody_h_4205298265(TEXT("/Script/Orbit_Simulator"),
		Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_AstroBody_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Ryan_Desktop_Applied_Project_UE_5_1_Orbit_Simulator_Orbit_Simulator_Source_Orbit_Simulator_AstroBody_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
