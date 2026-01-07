/**
* File: IRayErrDef.cs
*
* Purpose: IRay FPD error code definition
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
		public const int Err_OK = 0;
		public const int Err_TaskPending = 1;
		public const int Err_Unknown = 2;
		public const int Err_DuplicatedCreation = 3;
		public const int Err_DetectorIdNotFound = 4;
		public const int Err_StateErr = 5;
		public const int Err_NotInitialized = 6;
		public const int Err_NotImplemented = 7;
		public const int Err_AccessDenied = 8;
		public const int Err_LoadDllFailed = 9;
		public const int Err_DllCreateObjFailed = 10;
		public const int Err_OpenFileFailed = 11;
		public const int Err_FileNotExist = 12;
		public const int Err_ConfigFileNotExist = 13;
		public const int Err_TemplateFileNotExist = 14;
		public const int Err_TemplateFileNotMatch = 15;
		public const int Err_InvalidFileFormat = 16;
		public const int Err_CreateLoggerFailed = 17;
		public const int Err_InvalidParamCount = 18;
		public const int Err_InvalidParamType = 19;
		public const int Err_InvalidParamValue = 20;
		public const int Err_PreCondition = 21;
		public const int Err_TaskTimeOut = 22;
		public const int Err_ProdInfoMismatch = 23;
		public const int Err_DetectorRespTimeout = 24;
		public const int Err_InvalidPacketNo = 25;
		public const int Err_InvalidPacketFormat = 26;
		public const int Err_PacketDataCheckFailed = 27;
		public const int Err_PacketLost_BufOverflow = 28;
		public const int Err_FrameLost_BufOverflow = 29;
		public const int Err_ImgChBreak = 30;
		public const int Err_BadImgQuality = 31;
		public const int Err_GeneralSocketErr = 32;
		public const int Err_DetectorSN_Mismatch = 33;
		public const int Err_CommDeviceNotFound = 34;
		public const int Err_CommDeviceOccupied = 35;
		public const int Err_CommParamNotMatch = 36;
		public const int Err_NotEnoughDiskSpace = 37;
		public const int Err_NotEnoughMemorySpace = 38;
		public const int Err_ApplyFirmwareFailed = 39;
		public const int Err_CallbackNotFinished = 40;
		public const int Err_FirmwareUpdated = 41;
		public const int Err_TooMuchDefectPoints = 42;
		public const int Err_TooLongFilePath = 43;
		public const int Err_ClientIPNotMatch = 44;
		public const int Err_AddressOccupied = 45;
		public const int Err_NotInSameNetworkSegment = 46;
		public const int Err_ImageBufferEmpty = 47;
		public const int Err_FactoryCalibFileNotExist = 48;
		public const int Err_Crypto_OpensslFailure = 80;
		public const int Err_Auth_DateTimeExpired = 90;
		public const int Err_Auth_CommSetupFailure = 91;
		public const int Err_Auth_LicenseInvalid = 92;
		public const int Err_FPD_General_Detector_Error = 1001;
		public const int Err_FPD_General_ControlBox_Error = 1002;
		public const int Err_FPD_General_FirmwareUpgrade_Error = 1003;
		public const int Err_FPD_General_GSensor_Error = 1004;
		public const int Err_FPD_NotImplemented = 1005;
		public const int Err_FPD_SeqNoOutOfSync = 1006;
		public const int Err_FPD_Busy = 1007;
		public const int Err_FPD_Busy_Initializing = 1018;
		public const int Err_FPD_Busy_Last_Command_Suspending = 1019;
		public const int Err_FPD_Busy_Mode_Not_Supported = 1020;
		public const int Err_FPD_Busy_MCU_Busy = 1021;
		public const int Err_FPD_Busy_FPGA_Busy = 1022;
		public const int Err_FPD_Busy_FPGA_Timeout = 1023;
		public const int Err_FPD_Busy_Doing_Dynamic_Ghost = 1024;
		public const int Err_FPD_Busy_Doing_Dynamic_Preoffset = 1025;
		public const int Err_FPD_Busy_FTP_Image_Uploading = 1026;
		public const int Err_FPD_Busy_Capture_State_Recover = 1027;
		public const int Err_FPD_Busy_System_Error = 1028;
		public const int Err_FPD_Busy_BatteryLow = 1029;
		public const int Err_FPD_Occupied = 1008;
		public const int Err_FPD_SleepWakeupFailed = 1009;
		public const int Err_FPD_SleepCaptureError = 1010;
		public const int Err_FPD_CmdExecuteTimeout = 1011;
		public const int Err_FPD_FirmwareFallback = 1012;
		public const int Err_FPD_NotSupportInCurrMode = 1013;
		public const int Err_FPD_NoEnoughStorageSpace = 1014;
		public const int Err_FPD_FileNotExist = 1015;
		public const int Err_FPD_FtpServerAccessError = 1016;
		public const int Err_FPD_HWCaliFileError = 1017;
		public const int Err_FPD_AcquisitionBlock = 1040;
		public const int Err_FPD_SelfTestFailed = 1041;
		public const int Err_TemperatureHigh = 1042;
		public const int Err_Ftrans_Not_Ready = 1043;
		public const int Err_Ftrans_LocalFileOpenFailed = 1044;
		public const int Err_Ftrans_NetError = 1045;
		public const int Err_Ftrans_StartTimeout = 1046;
		public const int Err_Ftrans_StartError = 1047;
		public const int Err_Ftrans_FinishedTimeout = 1048;
		public const int Err_Ftrans_Aborted = 1049;
		public const int Err_Ftrans_DownloadTimeout = 1050;
		public const int Err_Ftrans_DownloadNotComplete = 1051;
		public const int Err_Ftrans_UploadTimeout = 1052;
		public const int Err_Ftrans_TcpBreak = 1053;
		public const int Err_Ftrans_FileLen_Zero = 1054;
		public const int Err_Ftrans_ParamError = 1055;
		public const int Err_Ftrans_FinishError = 1056;
		public const int Err_Ftrans_FileCheckFailed = 1057;
		public const int Err_FPD_SelfTest_DiskReadOnly = 1100;
		public const int Err_FPD_SelfTest_SpaceNotEnough = 1101;
		public const int Err_FPD_SelfTest_CanNotGetFPDCfg = 1102;
		public const int Err_FPD_SelfTest_FPDCfgInvalid = 1103;
		public const int Err_FPD_SelfTest_TemplateFailed = 1104;
		public const int Err_FPD_SelfTest_SyncDftFailed = 1105;
		public const int Err_FPD_SelfTest_SyncGainFailed = 1106;
		public const int Err_FPD_SelfTest_SystemFileNotFound = 1107;
		public const int Err_FPD_SelfTest_Reserved = 1199;
		public const int Err_FTP_OpenLoginFailed = 2001;
		public const int Err_FTP_MkdirCdFailed = 2002;
		public const int Err_FTP_LocalFileOpenFailed = 2003;
		public const int Err_FTP_UploadFailed = 2004;
		public const int Err_FTP_DownloadFailed = 2005;
		public const int Err_FTP_FileVerifyFailed = 2006;
		public const int Err_FTP_TypeError = 2007;
		public const int Err_Cali_GeneralError = 3001;
		public const int Err_Cali_UnexpectImage_DoseHighHigh = 3002;
		public const int Err_Cali_UnexpectImage_ExpLineNotSatisfy = 3003;
		public const int Err_Cali_UnexpectImage_MistakeTrigger = 3004;
		public const int Err_Cali_DataNotReadyForGen = 3005;
		public const int Err_Cali_NotEnoughIntervalTime_OffsetTmpl = 3006;
		public const int Err_Cali_UnexpectImage_DoseOver = 3007;
		public const int Err_Cali_UnexpectImage_DoseUnder = 3008;
		public const int Err_Cali_UnexpectImage_ObjectDetected = 3009;
		public const int Err_Cali_UnexpectImage_PartialExposure = 3010;
		public const int Err_FPD_CertificationFailed = 4000;
		public const int Err_FPD_AccessDenied_Unauthorized = 4001;
		public const int Err_Lib_SplicingCorrect_Error = 5000;
	}
}
