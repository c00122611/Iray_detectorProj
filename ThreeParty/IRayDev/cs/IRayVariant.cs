/**
* File: IRayVariant.h
*
* Purpose: Definit interface data types
*
*
* @author Haitao.Ning
* @version 1.0 2015/02/02
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace iDetector
{
    public partial class SdkInterface
    {
        public const int IRAY_MAX_STR_LEN = 512;
        public const int IRAY_MAX_NAME_LEN = 128; 
        public const int IRAY_MAX_DESC_LEN = 256;         
    }

    public enum IRAY_VAR_TYPE
    {
	    IVT_INT = 0,
	    IVT_FLT = 1,
	    IVT_STR = 2,
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
	public struct IRayVariantVal
	{
		public int nVal;
        public float fVal;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_STR_LEN)]
        public string strVal;
	};

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct IRayVariant
    {
        public IRAY_VAR_TYPE vt;
        public IRayVariantVal val;
    };

    public enum IRAY_PARAM_TYPE
    {
	    IPT_VARIANT = 0,
	    IPT_BLOCK = 100
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct IRayDataBlock
    {
        public uint uBytes;
        public IntPtr pData; // void*
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct IRayCmdParam
    {
        public IRAY_PARAM_TYPE pt;
        public IRayVariant var;
        public IRayDataBlock blc;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct IRayVariantMapItem
    {
        public int nMapKey;
        public IRayVariant varMapVal;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct IRayVariantMap
    {
        public int nItemCount;
        public IntPtr pItems; // IRayVariantMapItem*
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DetectorProfile
    {
        public int nProdNo;
        public int nSubProdNo;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string strSN;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string strIP;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)]
        public string strReserved;

        public bool bDetState;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ProdInfo
    {
        public int nProdNo;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string strDescripion;
    };

    public enum PARAM_VALIDATOR
    {
        Validator_Null = 0,
        Validator_MinMax = 1,
        Validator_Enum = 2,
        Validator_FilePath = 3,
        Validator_IP = 4,
        Validator_MAC = 5,
        Validator_FpdSN = 6,
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AttrInfo
    {
        public int nAttrID;
        public IRAY_VAR_TYPE nDataType;
        public int bIsConfigItem;       // 0: not a config item, 1: is a config item
        public int bIsWritable;         // 0: not writable, 1: is writable
        public int bIsEnum;             // 0: not a enum, 1: is a enum
        public int nPrecision;          // 0: integer, 1: 0.1, 2: 0.01, 3: 0.001...
        public float fMinValue;
        public float fMaxValue;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strPath;        // Group information
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strDisplayName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strUnit;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string strDescription;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strEnumTypeName;
        public PARAM_VALIDATOR eValidator;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct EnumItem
    {
        public int nVal;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string strDescription;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct CmdInfo
    {
        public int nCmdID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strName;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct CmdParamInfo
    {
        public IRAY_VAR_TYPE nDataType; // DataType
        public int bIsDataBlock;        // 0: not a data block, 1: is a data block
        public int bIsEnum;             // 0: not a enum, 1: is a enum
        public float fMinValue;
        public float fMaxValue;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_NAME_LEN)]
        public string strName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string szEnumTypeName;
        public PARAM_VALIDATOR eValidator;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ErrorInfo
    {
        public int nErrorCode;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string strDescription;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = SdkInterface.IRAY_MAX_DESC_LEN)]
        public string strSolution;
    };

}