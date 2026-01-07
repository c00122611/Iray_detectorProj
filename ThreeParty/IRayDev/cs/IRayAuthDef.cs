/**
* File: IRayAuthDef.cs
*
* Purpose: IRay Authority definition
*
*
* @author Haitao.Ning
* @version 1.0 2022/6/13
*
* Copyright (C) 2009, 2022, iRay Technology (Shanghai) Ltd.
*
*/
namespace iDetector
{
    /**
    * Enm_LicenseKeyMode: Enumerate the bind key mode in license file
    */
    public enum Enm_LicenseKeyMode
    {
	    Enm_LicenseKeyMode_Null = 0,     // No H/W bind
	    Enm_LicenseKeyMode_PCID = 1,     // Bind Computer
	    Enm_LicenseKeyMode_UserCode = 2, // Deprecated
	    Enm_LicenseKeyMode_DetSN = 3,    // Bind DetectorSN
	    Enm_LicenseKeyMode_BachID = 4,   // Bind DetectorBatchID
    };
  
    public enum Enm_Authority
    {
	    Enm_Authority_Basic = 0x00000000,
	    Enm_Authority_RawImage = 0x00000001, 	 // Bit_1
	    Enm_Authority_UserDetConfig = 0x00000002, 	 // Bit_2
        Enm_Authority_Tomo = 0x00000004, 	 // Bit_3
	    Enm_Authority_Test = 0x00002000, 	 // Bit_14
	    Enm_Authority_FactoryConfig = 0x00004000, 	 // Bit_15
	    Enm_Authority_WriteSN = 0x00008000, 	 // Bit_16
        Enm_Authority_ATBT = 0x00010000, 	      // Bit_17

        // extern code here

	    Enm_Authority_Full = 0x7FFFFFFF, 	 // Bit_1..32
    };  

    public enum Enm_Authority_Sdk // inherit from old version (Enm_Authority)
    {
	    Enm_Authority_SDK_Basic = Enm_Authority.Enm_Authority_Basic,
	    Enm_Authority_SDK_RawImage = Enm_Authority.Enm_Authority_RawImage,
	    Enm_Authority_SDK_UserDetConfig = Enm_Authority.Enm_Authority_UserDetConfig,
        Enm_Authority_SDK_Tomo = Enm_Authority.Enm_Authority_Tomo,
        Enm_Authority_SDK_Test = Enm_Authority.Enm_Authority_Test,
        Enm_Authority_SDK_FactoryConfig = Enm_Authority.Enm_Authority_FactoryConfig,
        Enm_Authority_SDK_WriteSN = Enm_Authority.Enm_Authority_WriteSN,
        Enm_Authority_SDK_Full = Enm_Authority.Enm_Authority_Full,                           // Bit_1..31
    };

    public enum Enm_Authority_Alg
    {
	    Enm_Authority_Alg_Basic = 0x00000000,
	    Enm_Authority_Alg_Grid = 0x00000001, 	 // Bit_1
        Enm_Authority_Alg_Medical = 0x00000002, 	 // Bit_2
        Enm_Authority_Alg_Industry = 0x00000004, 	 // Bit_3
        Enm_Authority_Alg_Security = 0x00000008, 	 // Bit_4

        // extern code here

	    Enm_Authority_Alg_Full = 0x7FFFFFFF, 	 // Bit_1..32
    };

    public enum Enm_Authority_Det
    {
	    Enm_Authority_Det_Basic = 0x00000000,
	    Enm_Authority_Det_RawImage = 0x00000001, 	 // Bit_1
        Enm_Authority_Det_iAEC = 0x00000002, 	 // Bit_2
        Enm_Authority_Det_Tomo = 0x00000004, 	 // Bit_3

	    // extern code here

	    Enm_Authority_Det_Full = 0x7FFFFFFF, 	 // Bit_1..32
    };

    public enum Enm_Authority_Tools
    {
	    Enm_Authority_Tls_Basic = 0x00000000,
	    Enm_Authority_Tls_Demo = 0x00000001, 	 // Bit_1
	    Enm_Authority_Tls_CustomSupport = 0x00000002, 	 // Bit_2
	    Enm_Authority_Tls_Manufacture = 0x00000010, 	 // Bit_5
	    Enm_Authority_Tls_Diagnose = 0x00000020, 	 // Bit_6

	    // extern code here

	    Enm_Authority_Tlst_Full = 0x7FFFFFFF, 	 // Bit_1..32
    };
}
