/**
* File: IRayCmdDef.cs
*
* Purpose: IRay command definition
*
*
* @author Haitao.Ning
* @version 1.0 2015/4/23
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
namespace iDetector
{
	public partial class SdkInterface
	{
		public const int Cmd_SetLogLevel = 1;
		public const int Cmd_Connect = 2;
		public const int Cmd_Disconnect = 3;
		public const int Cmd_Sleep = 4;
		public const int Cmd_Wakeup = 5;
		public const int Cmd_SetCorrectOption = 6;
		public const int Cmd_SetCaliSubset = 7;
		public const int Cmd_Reset = 8;
		public const int Cmd_DownloadLicenseFile = 90;
		public const int Cmd_Clear = 1001;
		public const int Cmd_ClearAcq = 1002;
		public const int Cmd_Acq2 = 1003;
		public const int Cmd_AecPreExp = 1016;
		public const int Cmd_StartAcq = 1004;
		public const int Cmd_StopAcq = 1005;
		public const int Cmd_ForceSingleAcq = 1006;
		public const int Cmd_ForceContinuousAcq = 1007;
		public const int Cmd_ForceDarkSingleAcq = 1008;
		public const int Cmd_ForceDarkClearAcq = 1009;
		public const int Cmd_ForceDarkContinuousAcq = 1010;
		public const int Cmd_ProhibOutExp = 1011;
		public const int Cmd_EnableOutExp = 1012;
		public const int Cmd_SyncStart = 1013;
		public const int Cmd_SyncCancel = 1014;
		public const int Cmd_SyncAcq = 1015;
		public const int Cmd_EnterStitchingFlow = 1017;
		public const int Cmd_StitchingAcqOnce = 1018;
		public const int Cmd_QuitStitchingFlow = 1019;
		public const int Cmd_RetrieveFreshStitchingImages = 1020;
		public const int Cmd_ClearStitchingImages = 1021;
		public const int Cmd_RetrieveFreshImages = 1022;
		public const int Cmd_AbortBackstageImageTransfer = 1023;
		public const int Cmd_EnterDualEnergyFlow = 1024;
		public const int Cmd_QuitDualEnergyFlow = 1025;
		public const int Cmd_StartDualEnergyAcq = 1026;
		public const int Cmd_StartCombAcquire = 1027;
		public const int Cmd_ReadUserROM = 2001;
		public const int Cmd_WriteUserROM = 2002;
		public const int Cmd_ReadUserRAM = 2030;
		public const int Cmd_WriteUserRAM = 2031;
		public const int Cmd_ReadFactoryROM = 2003;
		public const int Cmd_WriteFactoryROM = 2004;
		public const int Cmd_ReadVtMap = 2005;
		public const int Cmd_WriteVtMap = 2006;
		public const int Cmd_Recover = 2007;
		public const int Cmd_UpdateFirmware = 2008;
		public const int Cmd_SetImgChannel = 2009;
		public const int Cmd_ReadTemperature = 2010;
		public const int Cmd_ReadHumidity = 2011;
		public const int Cmd_UploadDetectorLog = 2012;
		public const int Cmd_UploadShockLog = 2013;
		public const int Cmd_ClearShockLog = 2014;
		public const int Cmd_WriteShockThreshold = 2015;
		public const int Cmd_ReadShockThreshold = 2016;
		public const int Cmd_ReadBatteryStatus = 2017;
		public const int Cmd_SetTimeByDiff = 2018;
		public const int Cmd_QueryTimeDiff = 2019;
		public const int Cmd_QueryLivingTime = 2020;
		public const int Cmd_ReadWifiStatus = 2021;
		public const int Cmd_QueryWifiScanList = 2022;
		public const int Cmd_WriteWifiSettings = 2023;
		public const int Cmd_ReadWifiSettings = 2024;
		public const int Cmd_DownloadWorkList = 2025;
		public const int Cmd_QueryHistoricalImageList = 2026;
		public const int Cmd_SelectWorkItem = 2027;
		public const int Cmd_UploadHistoricalImages = 2028;
		public const int Cmd_StopUploadingHistoricalImages = 2029;
		public const int Cmd_ChangeParamsOfCurrentAppMode = 2032;
		public const int Cmd_QueryLastImageID = 2033;
		public const int Cmd_GetImageByImageID = 2034;
		public const int Cmd_WriteCustomROM = 2035;
		public const int Cmd_ReadCustomROM = 2036;
		public const int Cmd_WriteLicenseData = 2037;
		public const int Cmd_ReadLicenseData = 2038;
		public const int Cmd_ReadHallSensor = 2039;
		public const int Cmd_ClearHistoricalImages = 2040;
		public const int Cmd_WriteSingleAcqSetting = 2041;
		public const int Cmd_ReadAllAcqSettings = 2042;
		public const int Cmd_SwitchAcqSetting = 2043;
		public const int Cmd_ChangeParamsOfCurrentAppModeForSpecificEnergy = 2044;
		public const int Cmd_TurnOnBatteryShipMode = 2045;
		public const int Cmd_TurnOffBatteryShipMode = 2046;
		public const int Cmd_OffsetGeneration = 3001;
		public const int Cmd_GainInit = 3002;
		public const int Cmd_GainSelectCurrent = 3004;
		public const int Cmd_GainSelectAll = 3005;
		public const int Cmd_GainGeneration = 3006;
		public const int Cmd_DefectInit = 3007;
		public const int Cmd_LoadTemporaryDefectFile = 3008;
		public const int Cmd_DefectSelectCurrent = 3009;
		public const int Cmd_DefectSelectAll = 3033;
		public const int Cmd_DefectGeneration = 3010;
		public const int Cmd_LoadTemporaryGainFile = 3011;
		public const int Cmd_LagPrepareDarkImages = 3037;
		public const int Cmd_LagInit = 3012;
		public const int Cmd_LagSelectCurrent = 3013;
		public const int Cmd_LagGeneration = 3014;
		public const int Cmd_GridInit = 3036;
		public const int Cmd_GridGeneration = 3038;
		public const int Cmd_DownloadCertificationFile = 3039;
		public const int Cmd_PanelCertificate = 3040;
		public const int Cmd_UpdateFreqCompositeCoeff = 3032;
		public const int Cmd_CalibrationInit = 3035;
		public const int Cmd_FinishGenerationProcess = 3015;
		public const int Cmd_DeleteGenerationTempFile = 3016;
		public const int Cmd_DownloadCaliFile = 3017;
		public const int Cmd_UploadCaliFile = 3018;
		public const int Cmd_SelectCaliFile = 3019;
		public const int Cmd_HwGeneratePreOffsetTemplate = 3020;
		public const int Cmd_QueryHwCaliTemplateList = 3021;
		public const int Cmd_ApplyDefectCorrection = 3022;
		public const int Cmd_RequestSyncboxControl = 3023;
		public const int Cmd_ReleaseSyncboxControl = 3024;
		public const int Cmd_ReadOutExpEnableState = 3025;
		public const int Cmd_EnableAutoSleep = 3026;
		public const int Cmd_DisableAutoSleep = 3027;
		public const int Cmd_ReadAutoSleepEnableState = 3028;
		public const int Cmd_PowerOff = 3029;
		public const int Cmd_StartAutoCleanup = 3030;
		public const int Cmd_StopAutoCleanup = 3031;
		public const int Cmd_PanelRecover = 3034;
		public const int Cmd_IdcRegionCalibration = 3041;
		public const int Cmd_IdcRegionChnlSelect = 3042;
		public const int Cmd_RequestIDCControl = 3043;
		public const int Cmd_ReleaseIDCControl = 3044;
		public const int Cmd_SetIDCCutOffThreshold = 3045;
		public const int Cmd_SetIDCActiveRegions = 3051;
		public const int Cmd_SetIDCRegionSelectionMode = 3052;
		public const int Cmd_MultiEnergy_RegistrationGeneration = 3054;
		public const int Cmd_IDC_Request = 3056;
		public const int Cmd_QueryIDCRegionSelectionMode = 3061;
		public const int Cmd_ActivePanelIDC = 3063;
		public const int Cmd_DeactivePanelIDC = 3064;
		public const int Cmd_QueryIDCCutOffThreshold = 3068;
		public const int Cmd_QueryIDCActiveRegions = 3069;
		public const int Cmd_UpdateOffsetOfAllModes = 3070;
		public const int Cmd_Debug = 10000;
		public const int Cmd_Debug_ReadCRC = 10001;
		public const int Cmd_ReadCustomFile = 4001;
		public const int Cmd_WriteCustomFile = 4002;
		public const int Cmd_ActivePanel = 4003;
		public const int Cmd_DeactivePanel = 4004;
		public const int Cmd_ReadCustomData = 4005;
		public const int Cmd_WriteCustomData = 4006;
		public const int Cmd_HwGenerateTFTTemplate = 4007;
		public const int Cmd_GainInit_FactoryMode = 4008;
		public const int Cmd_UploadWifiCountryCodeDB = 4009;
		public const int Cmd_DefectInit_FactoryMode = 4010;
		public const int Cmd_UploadChipsyCCLCTemplate = 4011;
		public const int Cmd_WriteRFSettings = 4012;
	}
}
