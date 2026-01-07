/**
* File: iRayAuthDef.h
*
* Purpose: Define function Authorities for iRay products.
*
* @author Haitao.Ning
* @version 1.0 2022/06/06
*
* Copyright (C) 2022, iRay Technology (Shanghai) Ltd.
*
*/
#pragma once


/**
* Enm_LicenseKeyMode: Enumerate the bind key mode in license file
*/
enum Enm_LicenseKeyMode
{
	Enm_LicenseKeyMode_Null = 0,     // No H/W bind
	Enm_LicenseKeyMode_PCID = 1,     // Bind Computer
	Enm_LicenseKeyMode_UserCode = 2, // Deprecated
	Enm_LicenseKeyMode_DetSN = 3,    // Bind DetectorSN
	Enm_LicenseKeyMode_BatchID = 4,  // Bind DetectorBatchID
};

#pragma pack(push, 1)

struct IRayAuthority
{
	int nSdkAuth;  // 32 bit for SDK functions 
	int nAlgAuth;  // 32 bit for Algorithm functions
	int nDetAuth;  // 32 bit for Detector functions
	int nTlsAuth;  // 32 bit for Tool Programs functions
	int nReserved[12];
};

struct IRayLicenseInfo
{
	int nVersion;

	char szPublisher[160];
	char szUserName[160];
	char szKey[256];

	long long tAuthorizingDay; // seconds since 1970.01.01£¬UTC time
	long long tStartDay;       // seconds since 1970.01.01£¬UTC time
	long long tExpireDay;      // seconds since 1970.01.01£¬UTC time

	Enm_LicenseKeyMode eKeyMode;        // Bind Computer/DetectorSN/DetectorBatchID 

	IRayAuthority  Authorities;         // enable/disable functions by bitwise

	unsigned char  btReserved[28];      // reserved
}; // totol 700 bytes

#pragma pack(pop)


enum Enm_Authority
{
	Enm_Authority_Basic = 0x00000000,
	Enm_Authority_RawImage_WithoutOffset = 0x00000001, // Bit_1
	Enm_Authority_UserDetConfig = 0x00000002, 	  // Bit_2
	Enm_Authority_Tomo = 0x00000004, 	          // Bit_3
	Enm_Authority_RawImage_WithoutGain = 0x0000008, 	  // Bit_4
	Enm_Authority_RawImage_WithoutDefect = 0x00000010, // Bit_5
	Enm_Authority_DiagnoseTFT = 0x00000020, // Bit_6
	Enm_Authority_Test = 0x00002000, 	          // Bit_14
	Enm_Authority_FactoryConfig = 0x00004000, 	  // Bit_15
	Enm_Authority_WriteSN = 0x00008000, 	      // Bit_16
	Enm_Authority_ATBT = 0x00010000, 	      // Bit_17
	// extern code here

	Enm_Authority_Full = 0x7FFFFFFF, 	          // Bit_1..31
};  

enum Enm_Authority_Sdk // inherit from old version (Enm_Authority)
{
	Enm_Authority_SDK_Basic = Enm_Authority_Basic,
	Enm_Authority_SDK_RawImage_WithoutOffset = Enm_Authority_RawImage_WithoutOffset, // Bit_1
	Enm_Authority_SDK_RawImage_WithoutGain = Enm_Authority_RawImage_WithoutGain,     // Bit_4
	Enm_Authority_SDK_RawImage_WithoutDefect = Enm_Authority_RawImage_WithoutDefect, // Bit_5
	Enm_Authority_SDK_DiagnoseTFT = Enm_Authority_DiagnoseTFT,                    // Bit_6
	Enm_Authority_SDK_UserDetConfig = Enm_Authority_UserDetConfig,         // Bit_2
	Enm_Authority_SDK_Tomo = Enm_Authority_Tomo,                           // Bit_3
	Enm_Authority_SDK_Test = Enm_Authority_Test,                           // Bit_14
	Enm_Authority_SDK_FactoryConfig = Enm_Authority_FactoryConfig,         // Bit_15
	Enm_Authority_SDK_WriteSN = Enm_Authority_WriteSN,                     // Bit_16
	Enm_Authority_SDK_ATBT = Enm_Authority_ATBT,                           // Bit_17
	Enm_Authority_SDK_Full = Enm_Authority_Full,                           // Bit_1..31
};

enum Enm_Authority_Alg
{
	Enm_Authority_Alg_Basic = 0x00000000,
	Enm_Authority_Alg_Grid_Med = 0x00000001, 	 // Bit_1
	Enm_Authority_Alg_Medical = 0x00000002, 	 // Bit_2
	Enm_Authority_Alg_Industry = 0x00000004, 	 // Bit_3
	Enm_Authority_Alg_Security = 0x00000008, 	 // Bit_4
	Enm_Authority_Alg_Grid_Vet = 0x00000010, 	 // Bit_5
	Enm_Authority_Alg_Grid_Mammo = 0x00000020, 	 // Bit_6
	Enm_Authority_Alg_Defect = 0x00000040, 	     // Bit_7
	// extern code here

	Enm_Authority_Alg_Full = 0x7FFFFFFF, 	     // Bit_1..31
};


enum Enm_Authority_Det
{
	Enm_Authority_Det_Basic = 0x00000000,
	//Enm_Authority_Det_RawImage = 0x00000001, 	   // Bit_1
	Enm_Authority_Det_iAEC = 0x00000002, 	     // Bit_2
	Enm_Authority_Det_Tomo = 0x00000004, 	     // Bit_3
	Enm_Authority_Det_Rent = 0x00000008, 	     // Bit_4

	// extern code here

	Enm_Authority_Det_Full = 0x7FFFFFFF, 	    // Bit_1..31
};


enum Enm_Authority_Tools
{
	Enm_Authority_Tls_Basic = 0x00000000,
	Enm_Authority_Tls_Demo = 0x00000001, 	         // Bit_1
	Enm_Authority_Tls_CustomSupport = 0x00000002, 	 // Bit_2
	Enm_Authority_Tls_Manufacture = 0x00000010, 	 // Bit_5
	Enm_Authority_Tls_Diagnose = 0x00000020, 	     // Bit_6
	Enm_Authority_Tls_DecryptImage = 0x00000040, 	 // Bit_7

	// extern code here

	Enm_Authority_Tlst_Full = 0x7FFFFFFF, 	         // Bit_1..31
};