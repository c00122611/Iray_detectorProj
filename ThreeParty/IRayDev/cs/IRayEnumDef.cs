/**
* File: IRayEnumDef.cs
*
* Purpose: IRay enum definition
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
	public enum Enm_EventLevel
	{
		Enm_EventLevel_Info = 0,
		Enm_EventLevel_Warn = 1,
		Enm_EventLevel_Error = 2,
		Enm_EventLevel_Notify = 3,
	};

	public enum Enm_LogLevel
	{
		Enm_LogLevel_Debug = 0,
		Enm_LogLevel_Info = 1,
		Enm_LogLevel_Warn = 2,
		Enm_LogLevel_Error = 3,
		Enm_LogLevel_Always = 4,
	};

	public enum Enm_Switch
	{
		Enm_Off = 0,
		Enm_On = 1,
	};

	public enum Enm_DetectorState
	{
		Enm_State_Unknown = 0,
		Enm_State_Ready = 1,
		Enm_State_Busy = 2,
		Enm_State_Sleeping = 3,
	};

	public enum Enm_Transaction
	{
		Enm_Transaction_Null = 0,
		Enm_Transaction_GainGen = 1, 	 // Generating Gain calibration templage
		Enm_Transaction_DefectGen = 2, 	 // Generating Defect calibration templage
		Enm_Transaction_LagGen = 3,
		Enm_Transaction_GridGen = 4,
		Enm_Transaction_Stitching = 5,
		Enm_Transaction_DualEnergyAcq = 6,
		Enm_Transaction_DualEnergyOffsetGen = 7,
		Enm_Transaction_DualEnergyGainGen = 8,
		Enm_Transaction_DualEnergyDefectGen = 9,
		Enm_Transaction_DualEnergyGridGen = 10,
		Enm_Transaction_FactoryGainGen = 11,
		Enm_Transaction_FactoryDefectGen = 12,
		Enm_Transaction_GainDefectOptimizeGen = 13,
	};

	public enum Enm_ScannedState
	{
		Enm_ScannedState_NotFound = 0,
		Enm_ScannedState_Occupy = 1,
		Enm_ScannedState_Standby = 2,
	};

	public enum Enm_ConnectionState
	{
		Enm_ConnState_Unknown = 0, 	 // not initialized
		Enm_ConnState_HardwareBreak = 1, 	 // specified communication hardware can not find, or been plugged out
		Enm_ConnState_NotConnected = 2, 	 // hardware exist but not ready for communication
		Enm_ConnState_LowRate = 3, 	 // connected but in bad situation
		Enm_ConnState_OK = 4, 	 // normal connected
	};

	public enum Enm_ImageTag
	{
		Enm_ImageTag_Width = 0x0100, 	 // Tiff tag: image width in pixels
		Enm_ImageTag_Height = 0x0101, 	 // Tiff tag: image height in pixels
		Enm_ImageTag_BitsPerSample = 0x0102, 	 // Tiff tag: bits per channel (sample)
		Enm_ImageTag_Compression = 0x0103, 	 // Tiff tag: data compression technique
		Enm_ImageTag_PhotoMetric = 0x0106, 	 // Tiff tag: photometric interpretation
		Enm_ImageTag_Description = 0x010E, 	 // Tiff tag: info about image
		Enm_ImageTag_Maker = 0x010F, 	 // Tiff tag: scanner manufacturer name
		Enm_ImageTag_Model = 0x0110, 	 // Tiff tag: scanner model name/number
		Enm_ImageTag_StripOffsets = 0x0111, 	 // Tiff tag: offsets to data strips
		Enm_ImageTag_Orientation = 0x0112, 	 // Tiff tag: image orientation
		Enm_ImageTag_SamplePerPixel = 0x0115, 	 // Tiff tag: samples per pixel
		Enm_ImageTag_RowsPerStrip = 0x0116, 	 // Tiff tag: rows per strip of data
		Enm_ImageTag_StripByteCounts = 0x0117, 	 // Tiff tag: bytes counts for strips
		Enm_ImageTag_XResolution = 0x011A, 	 // Tiff tag: pixels/resolution in x
		Enm_ImageTag_YResolution = 0x011B, 	 // Tiff tag: pixels/resolution in y
		Enm_ImageTag_ResolutionUnit = 0x0128, 	 // Tiff tag: units of resolutions
		Enm_ImageTag_Software = 0x0131, 	 // Tiff tag: name & release
		Enm_ImageTag_DateTime = 0x0132, 	 // Tiff tag: creation date and time
		Enm_ImageTag_FrameNo = 0x8001, 	 // Extern tag:
		Enm_ImageTag_Temperature = 0x8002, 	 // Extern tag:
		Enm_ImageTag_RealFrequency = 0x8003, 	 // Extern tag:
		Enm_ImageTag_ExposureDelay = 0x8004, 	 // Extern tag:
		Enm_ImageTag_AcquireTime = 0x8005, 	 // Extern tag:
		Enm_ImageTag_KV = 0x8006, 	 // Extern tag:
		Enm_ImageTag_MA = 0x8007, 	 // Extern tag:
		Enm_ImageTag_MS = 0x8008, 	 // Extern tag:
		Enm_ImageTag_SID = 0x8009, 	 // Extern tag:
		Enm_ImageTag_Dose = 0x800A, 	 // Extern tag:
		Enm_ImageTag_ExposureLine = 0x800B, 	 // Extern tag:
		Enm_ImageTag_CorrectFlag = 0x800C, 	 // Extern tag:
		Enm_ImageTag_BinningMode = 0x800D, 	 // Extern tag:
		Enm_ImageTag_XRayWindow = 0x800E, 	 // Extern tag:
		Enm_ImageTag_FrameRate = 0x800F, 	 // Extern tag:
		Enm_ImageTag_BodyPartSection = 0x8010, 	 // Extern tag:
		Enm_ImageTag_BodyPart = 0x8011, 	 // Extern tag:
		Enm_ImageTag_DoseRate = 0x8012, 	 // Extern tag:
		Enm_ImageTag_AvgValue = 0x8013, 	 // Extern tag:
		Enm_ImageTag_CenterValue = 0x8014, 	 // Extern tag:
		Enm_ImageTag_ImageQualityNG = 0x8015, 	 // Extern tag: Image quality not good, caused by Long-delay-time, vibration, or invalid correction, etc.
		Enm_ImageTag_LagPositionNG = 0x8016, 	 // Extern tag: Exposure time not good for lag calibration template image
		Enm_ImageTag_iRayProductNO = 0x8017, 	 // Extern tag: iRay detector product number
		Enm_ImageTag_ExposureStatus = 0x8018, 	 // Extern tag: Exposure status, -1:default freesync mode, 0: obsolete, 2: new freesync mode, other: reserved
		Enm_ImageTag_ImageID = 0x8019, 	 // Extern tag:
		Enm_ImageTag_EnergyIndex = 0x801A, 	 // Extern tag: the smaller the Energy index, the greater the energy
		Enm_ImageTag_iAECLines = 0x801B, 	 // Extern tag: iAEC lines position as string, splited by ','
		Enm_ImageTag_ImageEncrypt = 0x801C, 	 // Extern tag: Image encrypted or not
		Enm_ImageTag_IdcActivePosAvgValue = 0x801D, 	 // Extern tag: iAEC active position total pixel average value, this tag used in iAEC calibration
		Enm_ImageTage_ExternalInputLoData = 0x801E, 	 // Extern tag:low int32 of external input data
		Enm_ImageTage_ExternalInputHiData = 0x801F, 	 // Extern tag:high int32 of external input data
		Enm_ImageTag_SlipRing_CoderPoition = 0x8020, 	 // Extern tag:Add by 1 after acquire a image
		Enm_ImageTag_SlipRing_HomePulse = 0x8021, 	 // Extern tag:Add by 1 after receive a home pulse signal
		Enm_ImageTag_SlipRing_InputPulseCnt = 0x8022, 	 // Extern tag:Add by 1 after receive a trigger signal
		Enm_ImageTag_FirmwareVer = 0xA002, 	 // Extern tag:
		Enm_ImageTag_SoftwareVer = 0xA003, 	 // Extern tag:
		Enm_ImageTag_FpdSerialNo = 0xA004, 	 // Extern tag:
		Enm_ImageTag_Filter = 0xA005, 	 // Extern tag:
		Enm_ImageTag_CheckID = 0xA006, 	 // Extern tag:
		Enm_ImageTag_PatientID = 0xA007, 	 // Extern tag:
		Enm_ImageTag_PatientName = 0xA008, 	 // Extern tag:
		Enm_ImageTag_PatientGender = 0xA009, 	 // Extern tag:
		Enm_ImageTag_PatientAge = 0xA00A, 	 // Extern tag:
		Enm_ImageTag_PatientWeight = 0xA00B, 	 // Extern tag:
		Enm_ImageTag_PatientHeight = 0xA00C, 	 // Extern tag:
		Enm_ImageTag_PatientSize = 0xA00D, 	 // Extern tag:
	};

	public enum Enm_TriggerMode
	{
		Enm_TriggerMode_Outer = 0,
		Enm_TriggerMode_Inner = 1,
		Enm_TriggerMode_Soft = 2,
		Enm_TriggerMode_Prep = 3,
		Enm_TriggerMode_Service = 4,
		Enm_TriggerMode_FreeSync = 5,
		Enm_TriggerMode_AED_DC = 6,
		Enm_TriggerMode_AED_AC = 7,
	};

	public enum Enm_Binning
	{
		Enm_Binning_Null = 0, 	 // No binning
		Enm_Binning_2x2 = 1, 	 // 2x2 binning
		Enm_Binning_3x3 = 2, 	 // 3x3 binning
		Enm_Binning_4x4 = 3, 	 // 4x4 binning
		Enm_Binning_6x6 = 5, 	 // 6x6 binning
		Enm_Binning_8x8 = 7, 	 // 8x8 binning
		Enm_Binning_1x2 = 129, 	 // 1x2 binning
		Enm_Binning_1_33x1_33 = 201, 	 // 1.33x1.33 binning,convert 4 pixels to 3 pixels
	};

	public enum Enm_Zoom
	{
		Enm_Zoom_Null = 0, 	 // No zoom
		Enm_Zoom_1024x640 = 1,
		Enm_Zoom_1024x320 = 2,
		Enm_Zoom_1024x4 = 3,
		Enm_Zoom_1024x2 = 4,
		Enm_Zoom_1024x1 = 5,
		Enm_Zoom_512x512 = 6,
		Enm_Zoom_256x256 = 7,
		Enm_Zoom_3072x3072 = 8,
		Enm_Zoom_2048x2048 = 9,
		Enm_Zoom_1536x1536 = 10,
		Enm_Zoom_1024x1024 = 11,
		Enm_Zoom_3072x16 = 12,
		Enm_Zoom_3072x32 = 13,
		Enm_Zoom_3072x48 = 14,
		Enm_Zoom_3072x72 = 15,
		Enm_Zoom_3072x144 = 16,
	};

	public enum Enm_DynaFlag
	{
		Enm_DynaFlag_Static = 0,
		Enm_DynaFlag_Dynamic = 1,
	};

	public enum Enm_ReXferMode
	{
		Enm_ReXferMode_Null = 0,
		Enm_ReXferMode_Packet = 1,
		Enm_ReXferMode_Frame = 2,
	};

	public enum Enm_CorrectOption
	{
		Enm_CorrectOp_Null = 0x0000,
		Enm_CorrectOp_HW_PreOffset = 0x0001,
		Enm_CorrectOp_HW_PostOffset = 0x0002,
		Enm_CorrectOp_HW_Gain = 0x0004,
		Enm_CorrectOp_HW_Defect = 0x0010,
		Enm_CorrectOp_HW_Ghost = 0x0020,
		Enm_CorrectOp_HW_Lag = 0x0040,
		Enm_CorrectOp_HW_MicroPhony = 0x0080,
		Enm_CorrectOp_HW_GridRemoval = 0x0100,
		Enm_CorrectOp_HW_VirtualGrid = 0x0200,
		Enm_CorrectOp_SW_PreOffset = 0x00010000,
		Enm_CorrectOp_SW_PostOffset = 0x00020000,
		Enm_CorrectOp_SW_Gain = 0x00040000,
		Enm_CorrectOp_SW_Defect = 0x00100000,
		Enm_CorrectOp_SW_Ghost = 0x00200000,
		Enm_CorrectOp_SW_Lag = 0x00400000,
		Enm_CorrectOp_SW_MicroPhony = 0x00800000,
		Enm_CorrectOp_SW_GridRemoval = 0x01000000,
		Enm_CorrectOp_SW_VirtualGrid = 0x02000000,
	};

	public enum Enm_SubProductNo
	{
		Enm_SubProductNo_GoS = 0,
		Enm_SubProductNo_CsI = 1,
		Enm_SubProductNo_CsI400 = 2,
		Enm_SubProductNo_CsI550 = 3,
		Enm_SubProductNo_CsI500 = 4,
		Enm_SubProductNo_CsI600 = 5,
		Enm_SubProductNo_CsICMOS1 = 6,
		Enm_SubProductNo_Reserved7 = 7,
		Enm_SubProductNo_Reserved8 = 8,
		Enm_SubProductNo_Reserved9 = 9,
		Enm_SubProductNo_Reserved10 = 10,
	};

	public enum Enm_SignalLevel
	{
		Enm_SignalLevel_Low = 0,
		Enm_SignalLevel_High = 1,
	};

	public enum Enm_FluroSync
	{
		Enm_FluroSync_FreeRun = 0,
		Enm_FluroSync_SyncIn = 1,
		Enm_FluroSync_SyncOut = 2,
		Enm_FluroSync_SyncIn_Clk = 3,
	};

	public enum Enm_PGA
	{
		Enm_PGA_0 = 0,
		Enm_PGA_1 = 1,
		Enm_PGA_2 = 2,
		Enm_PGA_3 = 3,
		Enm_PGA_4 = 4,
		Enm_PGA_5 = 5,
		Enm_PGA_6 = 6,
		Enm_PGA_7 = 7,
		Enm_PGA_8 = 8,
		Enm_PGA_9 = 9,
	};

	public enum Enm_HWOffsetType
	{
		Enm_OffsetType_Null = 0,
		Enm_OffsetType_Post = 1,
		Enm_OffsetType_Pre = 2,
	};

	public enum Enm_ExpMode
	{
		Enm_ExpMode_Null = 0x00,
		Enm_ExpMode_Manual = 0x01,
		Enm_ExpMode_AEC = 0x02,
		Enm_ExpMode_Manual2 = 0x04,
		Enm_ExpMode_Pulse = 0x80,
		Enm_ExpMode_Continous = 0x81,
		Enm_ExpMode_Linewise = 0x82,
		Enm_ExpMode_Flush = 0x83,
		Enm_ExpMode_Swap = 0x84,
	};

	public enum Enm_PrepCapMode
	{
		Enm_PrepCapMode_ClearAcq = 0,
		Enm_PrepCapMode_Acq2 = 1,
	};

	public enum Enm_OutModeCapTrig
	{
		Enm_OutModeCapTrig_X_ON = 0,
		Enm_OutModeCapTrig_Prep = 1,
	};

	public enum Enm_ImgChType
	{
		Enm_ImgCh_Ethernet = 0,
		Enm_ImgCh_Cameralink = 1,
		Enm_ImgCh_GigeVision = 2,
	};

	public enum Enm_EthernetProtocol
	{
		Enm_Ethernet_UDP = 0,
		Enm_Ethernet_TCP = 1,
	};

	public enum Enm_FW_DeviceType
	{
		Enm_FW_DeviceType_MainFPGA = 0x01,
		Enm_FW_DeviceType_ReadFPGA1 = 0x02,
		Enm_FW_DeviceType_ReadFPGA2 = 0x03,
		Enm_FW_DeviceType_ControlBox = 0x04,
		Enm_FW_DeviceType_MCU1 = 0x10,
		Enm_FW_DeviceType_ProdCtrlBox = 0x12,
		Enm_FW_DeviceType_iAECBox = 0x13,
		Enm_FW_DeviceType_Dongle = 0x14,
		Enm_FW_ConfigType_FactoryROM = 0xC0,
		Enm_FW_ConfigType_ShockSensorROM = 0xC1,
		Enm_FW_DeviceType_AllInOne = 0xF0,
		Enm_FW_DeviceType_WebApp = 0xF1,
		Enm_FW_DeviceType_None = 0xFF,
	};

	public enum Enm_CaliDataState
	{
		Enm_CaliDataState_NoData = 0,
		Enm_CaliDataState_Valid = 1,
		Enm_CaliDataState_ValidWarn = 2,
		Enm_CaliDataState_OutOfDate = 3,
		Enm_CaliDataState_ParamMisMatch = 4,
	};

	public enum Enm_NetworkInterface
	{
		Enm_NetworkInterface_Unknown = 0,
		Enm_NetworkInterface_Cable = 1,
		Enm_NetworkInterface_Wifi = 2,
	};

	public enum Enm_FileTypes
	{
		Enm_File_Offset = 0x01,
		Enm_File_Gain = 0x02,
		Enm_File_Defect = 0x04,
		Enm_File_Lag = 0x05,
		Enm_File_GridRemoval = 0x06,
		Enm_File_CertiFile = 0x07,
		Enm_File_MultiEnergyOffset = 0x11,
		Enm_File_FactoryGain = 0x0A,
		Enm_File_FactoryDefect = 0x0B,
		Enm_File_DT = 0x0C,
		Enm_File_AT = 0x0D,
		Enm_File_ArmLog = 0xA0,
		Enm_File_Firmware = 0xA1,
		Enm_File_CustomFile = 0xA2,
		Enm_File_iAECRegionFile = 0xA3,
		Enm_File_TFT = 0xA4,
		Enm_File_WifiCountryCodeDB = 0xA5,
		Enm_File_CalibCfg = 0xA6,
		Enm_File_FactoryConfigFile = 0xA8,
		Enm_File_License = 0xB0,
		Enm_File_BinningOffset = 0xF1, 	 // Detector internal use, for post dual-energy
		Enm_File_BinningGain = 0xF2, 	 // Detector internal use, for post dual-energy
		Enm_File_BinningDefect = 0xF4, 	 // Detector internal use, for post dual-energy
	};

	public enum Enm_Wifi_CountryCode
	{
		Enm_Wifi_Country_CN = 0,
		Enm_Wifi_Country_DE = 1,
		Enm_Wifi_Country_FR = 2,
		Enm_Wifi_Country_GB = 3,
		Enm_Wifi_Country_HK = 4,
		Enm_Wifi_Country_IT = 5,
		Enm_Wifi_Country_KR = 6,
		Enm_Wifi_Country_NL = 7,
		Enm_Wifi_Country_RU = 8,
		Enm_Wifi_Country_US = 9,
		Enm_Wifi_Country_WW = 10,
		Enm_Wifi_Country_JP = 11,
		Enm_Wifi_Country_EU = 12,
		Enm_Wifi_Country_CA = 13,
		Enm_Wifi_Country_BR = 14,
		Enm_Wifi_Country_MX = 15,
		Enm_Wifi_Country_IN = 16,
		Enm_Wifi_Country_ID = 17,
		Enm_Wifi_Country_TW = 18,
		Enm_Wifi_Country_NP = 19,
		Enm_Wifi_Country_BD = 20,
		Enm_Wifi_Country_VN = 21,
		Enm_Wifi_Country_MY = 22,
		Enm_Wifi_Country_PK = 23,
		Enm_Wifi_Country_TR = 24,
		Enm_Wifi_Country_EG = 25,
		Enm_Wifi_Country_AR = 26,
		Enm_Wifi_Country_SG = 27,
		Enm_Wifi_Country_TH = 28,
		Enm_Wifi_Country_PH = 29,
		Enm_Wifi_Country_AU = 30,
		Enm_Wifi_Country_CO = 31,
		Enm_Wifi_Country_PR = 32,
		Enm_Wifi_Country_UA = 33,
		Enm_Wifi_Country_LK = 34,
		Enm_Wifi_Country_IR = 35,
		Enm_Wifi_Country_SA = 36,
		Enm_Wifi_Country_ZA = 37,
		Enm_Wifi_Country_KE = 38,
		Enm_Wifi_Country_TZ = 39,
		Enm_Wifi_Country_GH = 40,
		Enm_Wifi_Country_RS = 41,
		Enm_Wifi_Country_DZ = 42,
		Enm_Wifi_Country_IL = 43,
		Enm_Wifi_Country_MA = 44,
		Enm_Wifi_Country_AE = 45,
		Enm_Wifi_Country_ByLetter = 65535,
	};

	public enum Enm_Wifi_Frequency
	{
		Enm_Wifi_Freq_2GHz = 0,
		Enm_Wifi_Freq_5GHz = 1,
	};

	public enum Enm_Wifi_BandWidth
	{
		Enm_Wifi_Band_HT20 = 0,
		Enm_Wifi_Band_HT40_Plus = 1,
		Enm_Wifi_Band_HT40_Minus = 2,
		Enm_Wifi_Band_HT80 = 3,
		Enm_Wifi_Band_HE80 = 4,
	};

	public enum Enm_Wifi_Channel
	{
		Enm_Wifi_Chnl_1 = 1,
		Enm_Wifi_Chnl_2 = 2,
		Enm_Wifi_Chnl_3 = 3,
		Enm_Wifi_Chnl_4 = 4,
		Enm_Wifi_Chnl_5 = 5,
		Enm_Wifi_Chnl_6 = 6,
		Enm_Wifi_Chnl_7 = 7,
		Enm_Wifi_Chnl_8 = 8,
		Enm_Wifi_Chnl_9 = 9,
		Enm_Wifi_Chnl_10 = 10,
		Enm_Wifi_Chnl_11 = 11,
		Enm_Wifi_Chnl_12 = 12,
		Enm_Wifi_Chnl_13 = 13,
		Enm_Wifi_Chnl_36 = 36,
		Enm_Wifi_Chnl_40 = 40,
		Enm_Wifi_Chnl_44 = 44,
		Enm_Wifi_Chnl_48 = 48,
		Enm_Wifi_Chnl_149 = 149,
		Enm_Wifi_Chnl_153 = 153,
		Enm_Wifi_Chnl_157 = 157,
		Enm_Wifi_Chnl_161 = 161,
		Enm_Wifi_Chnl_165 = 165,
	};

	public enum Enm_Wifi_Security
	{
		Enm_Wifi_WPA_WPA2_PSK = 0,
		Enm_Wifi_WPA_PSK = 1,
		Enm_Wifi_WPA2_PSK = 2,
	};

	public enum Enm_Wifi_TransmitPower
	{
		Enm_Wifi_TransmitPower_8 = 8,
		Enm_Wifi_TransmitPower_9 = 9,
		Enm_Wifi_TransmitPower_10 = 10,
		Enm_Wifi_TransmitPower_11 = 11,
		Enm_Wifi_TransmitPower_12 = 12,
		Enm_Wifi_TransmitPower_13 = 13,
		Enm_Wifi_TransmitPower_14 = 14,
		Enm_Wifi_TransmitPower_15 = 15,
		Enm_Wifi_TransmitPower_16 = 16,
		Enm_Wifi_TransmitPower_17 = 17,
		Enm_Wifi_TransmitPower_18 = 18,
	};

	public enum Enm_Battery_Warn
	{
		Enm_Battery_Normal = 0,
		Enm_Battery_LowPower = 1,
		Enm_Battery_PowerOff = 2,
	};

	public enum Enm_Temperature_State
	{
		Enm_Temperature_Normal = 0,
		Enm_Temperature_Warning = 1,
		Enm_Temperature_PowerOff = 2,
	};

	public enum Enm_FreesyncSubFlow
	{
		Enm_FreesyncSubFlow_Reserved = 0,
		Enm_FreesyncSubFlow_NFNR = 1, 	 // ISync plus: Single ExpLine
		Enm_FreesyncSubFlow_FFNR = 2,
		Enm_FreesyncSubFlow_3 = 3, 	 // FSync Dual Gate: Multi ExpLine (distinguish odd/even column)
		Enm_FreesyncSubFlow_4 = 4, 	 // FSync Single Gate: Multi ExpLine (whole line)
		Enm_FreesyncSubFlow_5 = 5, 	 // ISync plus: Single ExpLine for PreOffset
	};

	public enum Enm_SyncWorkState
	{
		Enm_SyncWorkState_Unknown = 0,
		Enm_SyncWorkState_Ready = 1,
		Enm_SyncWorkState_Busy = 2,
	};

	public enum Enm_FullWell
	{
		Enm_FullWell_0 = 0,
		Enm_FullWell_1 = 1,
		Enm_FullWell_2 = 2,
		Enm_FullWell_3 = 3,
	};

	public enum Enm_InnerSubFlow
	{
		Enm_InnerSubFlow_ClearAcq = 0, 	 // Normal mode
		Enm_InnerSubFlow_CycleAcq = 1, 	 // Cycle self acquisition as freesync implementation
	};

	public enum Enm_SoftwareSubFlow
	{
		Enm_SoftwareSubFlow_Normal = 0, 	 // Normal mode
		Enm_SoftwareSubFlow_OverlieAcq = 1, 	 // Overlie the pixel value of cycle acquisition for long time exposure
	};

	public enum Enm_DetectorSleepMode
	{
		Enm_DetectorSleepMode_Normal = 0, 	 // Normal mode
		Enm_DetectorSleepMode_Deep = 1, 	 // Futher more suspend detector components
	};

	public enum Enm_COF_PGA
	{
		Enm_COF_PGA_Reserved = 0,
		Enm_COF_PGA_0_6_pC = 1,
		Enm_COF_PGA_1_2_pC = 2,
		Enm_COF_PGA_1_8_pC = 3,
		Enm_COF_PGA_2_4_pC = 4,
		Enm_COF_PGA_3_0_pC = 5,
		Enm_COF_PGA_3_6_pC = 6,
		Enm_COF_PGA_4_2_pC = 7,
		Enm_COF_PGA_4_8_pC = 8,
		Enm_COF_PGA_5_4_pC = 9,
		Enm_COF_PGA_6_0_pC = 10,
		Enm_COF_PGA_6_6_pC = 11,
		Enm_COF_PGA_7_2_pC = 12,
		Enm_COF_PGA_7_8_pC = 13,
		Enm_COF_PGA_8_4_pC = 14,
		Enm_COF_PGA_9_0_pC = 15,
		Enm_COF_PGA_9_6_pC = 31,
	};

	public enum Enm_NetworkBand
	{
		Enm_NetworkBand_1G = 1, 	 // 1Gbps network band
		Enm_NetworkBand_2_5G = 2, 	 // 2.5Gbps network band
		Enm_NetworkBand_5G = 3, 	 // 5Gbps network band
		Enm_NetworkBand_10G = 4, 	 // 10Gbps network band
		Enm_NetworkBand_100M = 5, 	 // 100Mbps network band
	};

	public enum Enm_ConnectType
	{
		Enm_ConnType_Off = 0x00,
		Enm_ConnType_Cable = 0x01,
		Enm_ConnType_Ethernet = 0x02,
		Enm_ConnType_RadioFrequency = 0x03,
		Enm_ConnType_MixedMode = 0x0A, 	 // RadioFrequency + Ethernet
		Enm_ConnType_Dynamic_Cable = 0x11,
		Enm_ConnType_Dynamic_MixedMode = 0x1A,
	};

	public enum Enm_TriggerThreshold
	{
		Enm_TriggerThreshold_5uGy = 1, 	 // 5uGy/s
		Enm_TriggerThreshold_20uGy = 2, 	 // 20uGy/s
		Enm_TriggerThreshold_50uGy = 3, 	 // 50uGy/s
		Enm_TriggerThreshold_100uGy = 4, 	 // 100uGy/s
		Enm_TriggerThreshold_150uGy = 5, 	 // 150uGy/s
		Enm_TriggerThreshold_200uGy = 6, 	 // 200uGy/s
		Enm_TriggerThreshold_300uGy = 7, 	 // 300uGy/s
		Enm_TriggerThreshold_500uGy = 8, 	 // 500uGy/s
	};

	public enum Enm_ImageEnergyType
	{
		Enm_ImageEnergy_Normal = 0x00,
		Enm_ImageEnergy_Energy1 = 0x01, 	 // each bit represent an energy type
		Enm_ImageEnergy_Energy2 = 0x02,
	};

	public enum Enm_AcqMode
	{
		Enm_AcqMode_ClearAcq = 0,
		Enm_AcqMode_Acq2 = 1,
		Enm_AcqMode_AedWndAcq = 2,
		Enm_AcqMode_Aed = 3,
		Enm_AcqMode_Continous = 4,
		Enm_AcqMode_Flush = 5,
		Enm_AcqMode_Pulse = 6,
		Enm_AcqMode_DualEnergy = 7,
	};

	public enum Enm_AedMethod
	{
		Enm_AedMethod_1 = 0,
		Enm_AedMethod_2 = 1,
		Enm_AedMethod_3 = 2,
	};

	public enum Enm_FwUpgradeMethod
	{
		Enm_FwUpgradeMethod_Recover = 0,
		Enm_FwUpgradeMethod_Differences = 1,
	};

	public enum Enm_AntiInterferenceLevel
	{
		Enm_AntiInterference_Default = 0,
		Enm_AntiInterference_High = 1,
		Enm_AntiInterference_Middle = 2,
		Enm_AntiInterference_Low = 3,
	};

	public enum Enm_SyncInClearFlow
	{
		Enm_SyncInClearFlow_Default = 0, 	 // no clear
		Enm_SyncInClearFlow_JudgePulseWidth = 1, 	 // clear if another pulse width is checked
		Enm_SyncInClearFlow_JudgePulseFrequency = 2, 	 // automatically clear if no pulse input
		Enm_SyncInClearFlow_JudgePulseWidthPushFirstDark = 3, 	 // Push the first image(dark) and then clear if another pulse width is checked
	};

	public enum Enm_DataPacketSize
	{
		Enm_DataPacketSize_1K_16bit = 1, 	 // 16 bits represents a pixel during transmission
		Enm_DataPacketSize_4K_16bit = 2,
		Enm_DataPacketSize_8K_16bit = 3,
		Enm_DataPacketSize_1K_12bit = 4, 	 // 12 bits represents a pixel during transmission
		Enm_DataPacketSize_4K_12bit = 5,
		Enm_DataPacketSize_8K_12bit = 6,
		Enm_DataPacketSize_1K_14bit = 7, 	 // 14 bits represents a pixel during transmission
		Enm_DataPacketSize_4K_14bit = 8,
		Enm_DataPacketSize_8K_14bit = 9,
		Enm_DataPacketSize_16K_16bit = 10, 	 // 16 bits represents a pixel during transmission
		Enm_DataPacketSize_32K_16bit = 11,
	};

	public enum Enm_MultiEnergy_Process
	{
		Enm_MultiEnergy_Process_None = 0,
		Enm_MultiEnergy_Process_Fusion = 1,
	};

	public enum Enm_IntegrationMethod
	{
		Enm_IntegrationMethod_Normal = 0,
		Enm_IntegrationMethod_MFS = 1, 	 // Multiframe Superposition within Pre-Offset workflow for NDT series
		Enm_IntegrationMethod_MFSA = 2, 	 // Multiframe Average within Pre-Offset workflow for NDT series
	};

	public enum Enm_IDCRegionType
	{
		Enm_IDCRegion_Undefined = 0,
		Enm_IDCRegion_Three = 3,
		Enm_IDCRegion_Five = 5,
		Enm_IDCRegion_Full = 9,
	};

	public enum Enm_FacMapMethod
	{
		Enm_FacMapMethod_Without = 0,
		Enm_FacMapMethod_Shared = 1,
		Enm_FacMapMethod_Exclusive = 2,
		Enm_FacMapMethod_Shared_II = 3,
		Enm_FacMapMethod_Exclusive_II = 4,
	};

	public enum Enm_IDCCorrMode
	{
		Enm_IDCCorrMode_Immutable = 0,
		Enm_IDCCorrMode_Alterable = 1,
	};

	public enum Enm_CorrParam_ExpFramePos
	{
		Enm_ExpFramePos_Default = 0,
		Enm_ExpFramePos_DarkAndLight = 1,
		Enm_ExpFramePos_LightAndDark = 2, 	 // mark the 1st frame as light frame and the 2nd frame as dark frame in a acquisition cycle
	};

	public enum Enm_SignalWarning
	{
		Enm_SignalWarning_Exception = -3, 	 // maybe there is something wrong with sensor
		Enm_SignalWarning_LowLow = -2,
		Enm_SignalWarning_Low = -1,
		Enm_SignalWarning_Normal = 0, 	 // ideal work temperature
		Enm_SignalWarning_High = 1,
		Enm_SignalWarning_HighHigh = 2,
	};

	public enum Enm_IDCState
	{
		Enm_IDCState_Deactived = 0,
		Enm_IDCState_DevOffline = 1,
		Enm_IDCState_UnRequested = 2,
		Enm_IDCState_UnInitialized = 3,
		Enm_IDCState_Busy = 4,
		Enm_IDCState_Ready = 5,
	};

	public enum Enm_ExpWindowPos
	{
		Enm_ExpWindowPos_AcquisitionAndExposure = 0x00,
		Enm_ExpWindowPos_ExposureAndAcquisition = 0x01,
	};

	public enum Enm_ProdType
	{
		Enm_Prd_Mammo1012F = 0x000B, 	 // Mammo1012F
		Enm_Prd_NDT0505F = 0x000C, 	 // NDT0505F
		Enm_Prd_Venu1417W = 0x0016, 	 // Venu1417W
		Enm_Prd_Senu1417P = 0x001C, 	 // Senu1417P
		Enm_Prd_Venu1717M2 = 0x001D, 	 // Venu1717M2
		Enm_Prd_Venu1417X = 0x0018, 	 // Venu1417X
		Enm_Prd_Penu1417P = 0x0019, 	 // Penu1417P
		Enm_Prd_Mars1417V = 0x0020, 	 // Mars1417V1
		Enm_Prd_Penu1417P_plus = 0x0021, 	 // Penu1417P_plus
		Enm_Prd_Mars1717V = 0x0025, 	 // Mars1717V1
		Enm_Prd_Venu1717MF = 0x0026, 	 // Venu1717MF
		Enm_Prd_Sars1417 = 0x0027, 	 // Sars1417
		Enm_Prd_Mars1417X = 0x002A, 	 // Mars1417X
		Enm_Prd_Mars1717XU = 0x002D, 	 // Mars1717XU
		Enm_Prd_Pluto0406X = 0x002E, 	 // Pluto0406X
		Enm_Prd_Mars1012V = 0x0029, 	 // Mars1012V
		Enm_Prd_Mercu1717V = 0x0030, 	 // Mercu1717V
		Enm_Prd_Mercu1616TE = 0x0031, 	 // Mercu1616TE
		Enm_Prd_Mercu1616VE = 0x0032, 	 // Mercu1616VE
		Enm_Prd_Mars1717XF = 0x0033, 	 // Mars1717XF
		Enm_Prd_Mars1417XF = 0x0034, 	 // Mars1417XF
		Enm_Prd_Mars1717X = 0x0035, 	 // Mars1717X
		Enm_Prd_Venu1717MN = 0x003A, 	 // Venu1717MN
		Enm_Prd_Mars1417V2 = 0x003B, 	 // Mars1417V2
		Enm_Prd_Venu1012V = 0x003C, 	 // Venu1012V
		Enm_Prd_NDT1012MA = 0x003D, 	 // NDT1012MA
		Enm_Prd_Mars1717V2 = 0x003E, 	 // Mars1717V2
		Enm_Prd_Jupi0810X = 0x003F, 	 // Jupi0810X
		Enm_Prd_NDT1012M = 0x0040, 	 // NDT1012M
		Enm_Prd_NDT1616HE = 0x0041, 	 // NDT1616HE
		Enm_Prd_Jupi1012X = 0x0042, 	 // Jupi1012X
		Enm_Prd_NDT0505J = 0x0044, 	 // NDT0505J
		Enm_Prd_Venu1717X = 0x0048, 	 // Venu1717X
		Enm_Prd_Jupi0505X = 0x0049, 	 // Jupi0505X
		Enm_Prd_Mercu1616TN = 0x004B, 	 // Mercu1616TN
		Enm_Prd_Venu1717MX = 0x0050, 	 // Venu1717MX
		Enm_Prd_Mars1417V3 = 0x0055, 	 // Mars1417V3
		Enm_Prd_Pluto0900X = 0x0057, 	 // Pluto0900X
		Enm_Prd_Venu1012VD = 0x0058, 	 // Venu1012VD
		Enm_Prd_Venu1717XV = 0x005B, 	 // Venu1717XV
		Enm_Prd_Mars1717V3 = 0x005C, 	 // Mars1717V3
		Enm_Prd_Luna1417XM = 0x005D, 	 // Luna1417XM
		Enm_Prd_Mercu1717V1 = 0x0060, 	 // Mercu1717V1
		Enm_Prd_Pluto0900X1 = 0x0061, 	 // Pluto0900X1
		Enm_Prd_NDT1012LA = 0x0062, 	 // NDT1012LA
		Enm_Prd_Pluto0909X = 0x0064, 	 // Pluto0909X
		Enm_Prd_Mars1417XM = 0x0065, 	 // Mars1417XM
		Enm_Prd_Jupi0606X1 = 0x0066, 	 // Jupi0606X1
		Enm_Prd_Pluto0600X = 0x0068, 	 // Pluto0600X
		Enm_Prd_NDT1717M = 0x006C, 	 // NDT1717M
		Enm_Prd_NDT1417MA = 0x006D, 	 // NDT1417MA
		Enm_Prd_Pluto0001X = 0x006F, 	 // Pluto0001X
		Enm_Prd_Pluto1212X = 0x0075, 	 // Pluto1212X
		Enm_Prd_Pluto1216X = 0x0076, 	 // Pluto1216X
		Enm_Prd_Mars1717VS = 0x0077, 	 // Mars1717VS
		Enm_Prd_Venu1717XS = 0x0079, 	 // Venu1717XS
		Enm_Prd_Luna1013XE = 0x007B, 	 // Luna1013XE
		Enm_Prd_NDT0900P = 0x007D, 	 // NDT0900P
		Enm_Prd_Mars1417VK = 0x0080, 	 // Mars1417VK
		Enm_Prd_NDT0505J1 = 0x0082, 	 // NDT0505J1
		Enm_Prd_NDT0406P = 0x0083, 	 // NDT0406P
		Enm_Prd_NDT0203P = 0x0084, 	 // NDT0203P
		Enm_Prd_Pluto0406X_CSM = 0x0085, 	 // Pluto0406X_CSM
		Enm_Prd_Jupi0506X = 0x0087, 	 // Jupi0506X
		Enm_Prd_NDT0202M = 0x0088, 	 // NDT0202M
		Enm_Prd_INDUSTREX3025D = 0x008E, 	 // INDUSTREX3025D
		Enm_Prd_Mercu1717V2 = 0x009C, 	 // Mercu1717V2
		Enm_Prd_Mercu1717V3 = 0x009F, 	 // Mercu1717V3
		Enm_Prd_Venu1717F = 0x0001, 	 // Venu1717F
		Enm_Prd_Mercu0909F = 0x0006, 	 // Mercu0909F
		Enm_Prd_NDT0909M = 0x0038, 	 // NDT0909M
		Enm_Prd_Jupi1717X = 0x002F, 	 // Jupi1717X
		Enm_Prd_Jupi0606X = 0x004A, 	 // Jupi0606X
		Enm_Prd_Jupi1212X = 0x005A, 	 // Jupi1212X
		Enm_Prd_Jupi1216X = 0x0063, 	 // Jupi1216X
		Enm_Prd_Jupi0606ZO = 0x0089, 	 // Jupi0606ZO
		Enm_Prd_Jupi1212ZO = 0x008A, 	 // Jupi1212ZO
		Enm_Prd_Jupi0808ZO = 0x008B, 	 // Jupi0808ZO
		Enm_Prd_Jupi0909X = 0x009B, 	 // Jupi0909X
		Enm_Prd_Pluto0002X = 0x0092, 	 // Pluto0002X
		Enm_Prd_NDT1417LA = 0x0096, 	 // NDT1417LA
		Enm_Prd_Mars1724V = 0x0099, 	 // Mars1724V
		Enm_Prd_Mercu1717XU = 0x00A0, 	 // Mercu1717XU
		Enm_Prd_NDT1717HE = 0x00A1, 	 // NDT1717HE
		Enm_Prd_Mercu1717HE = 0x00A4, 	 // Mercu1717HE
		Enm_Prd_Mercu1717HS = 0x00A5, 	 // Mercu1717HS
		Enm_Prd_Luna1012X = 0x00A6, 	 // Luna1012X
		Enm_Prd_NDT1013LA = 0x00A7, 	 // NDT1013LA
		Enm_Prd_NDT1616HE2 = 0x00A8, 	 // NDT1616HE2
		Enm_Prd_NDT0606HS = 0x00A9, 	 // NDT0606HS
		Enm_Prd_NDT0909HS = 0x00AA, 	 // NDT0909HS
		Enm_Prd_NDT1012HS = 0x00AB, 	 // NDT1012HS
		Enm_Prd_Mars1717X2 = 0x00AD, 	 // Mars1717X2
		Enm_Prd_Mars1417X2 = 0x00AE, 	 // Mars1417X2
		Enm_Prd_NDT1717HS = 0x00B1, 	 // NDT1717HS
		Enm_Prd_NDT1717M3 = 0x00B2, 	 // NDT1717M3
		Enm_Prd_Venu1748V = 0x0098, 	 // Venu1748V
		Enm_Prd_Jupi1012XL = 0x00AF, 	 // Jupi1012XL
		Enm_Prd_Pluto0002XW = 0x00B3, 	 // Pluto0002XW
		Enm_Prd_Mercu1717DE = 0x00B4, 	 // Mercu1717DE
		Enm_Prd_NDT1717DE = 0x00B5, 	 // NDT1717DE
		Enm_Prd_Mercu0909X = 0x00B6, 	 // Mercu0909X
		Enm_Prd_NDT1717IL = 0x00B8, 	 // NDT1717IL
		Enm_Prd_Mercu1717VE = 0x00EA, 	 // Mercu1717VE
		Enm_Prd_Mercu1717VN = 0x00BB, 	 // Mercu1717VN
		Enm_Prd_Mercu1717X = 0x00BC, 	 // Mercu1717X
		Enm_Prd_NDT1717M2 = 0x00BD, 	 // NDT1717M2
		Enm_Prd_NDT0303HS = 0x00B7, 	 // NDT0303HS
		Enm_Prd_NDT0506P = 0x00B9, 	 // NDT0506P
		Enm_Prd_Jupi1216XU = 0x00BF, 	 // Jupi1216XU
		Enm_Prd_Mercu1724V = 0x00C6, 	 // Mercu1724V
		Enm_Prd_NDT0208X = 0x00C3, 	 // NDT0208X
		Enm_Prd_Mercu1748V = 0x00C4, 	 // Mercu1748V
		Enm_Prd_NDT1003P = 0x00CE, 	 // NDT1003P
		Enm_Prd_NDT0606M = 0x00CA, 	 // NDT0606M
		Enm_Prd_NDT0506PX = 0x00D0, 	 // NDT0506PX
		Enm_Prd_NDT0503P = 0x00D1, 	 // NDT0503P
		Enm_Prd_Venu1717XN = 0x00D2, 	 // Venu1717XN
		Enm_Prd_DTDI128_115 = 0x00D4, 	 // DTDI128_115
		Enm_Prd_DTDI1024_230 = 0x00D5, 	 // DTDI1024_230
		Enm_Prd_NDT0002P = 0x00D6, 	 // NDT0002P
		Enm_Prd_NDT1503P = 0x00D7, 	 // NDT1503P
		Enm_Prd_Mercu0517X = 0x00DA, 	 // Mercu0517X
		Enm_Prd_Mercu0810X = 0x00DB, 	 // Mercu0810X
		Enm_Prd_Mercu0808X = 0x00DC, 	 // Mercu0808X
		Enm_Prd_Mercu0707X = 0x00DE, 	 // Mercu0707X
		Enm_Prd_Mercu1717V4 = 0x00E4, 	 // Mercu1717V4
		Enm_Prd_Pluto0000X = 0x00E5, 	 // Pluto0000X
		Enm_Prd_NDT1006P = 0x00E6, 	 // NDT1006P
		Enm_Prd_NDT0506PHS = 0x00E8, 	 // NDT0506PHS
		Enm_Prd_NDT1717X = 0x00EB, 	 // NDT1717X
		Enm_Prd_Pluto0909M = 0x00EF, 	 // Pluto0909M
		Enm_Prd_NDT1717X2 = 0x00F1, 	 // NDT1717X2
		Enm_Prd_DTDI512_230 = 0x00F5, 	 // DTDI512_230
		Enm_Prd_DTDI128_460 = 0x00F6, 	 // DTDI128_460
		Enm_Prd_NDT0303OX = 0x00F7, 	 // NDT0303OX
		Enm_Prd_NDT1724M = 0x00F8, 	 // NDT1724M
		Enm_Prd_NDT1748M = 0x00F9, 	 // NDT1748M
		Enm_Prd_Venu1717ZN = 0x00FA, 	 // Venu1717ZN
		Enm_Prd_Pluto1006X = 0x00FC, 	 // Pluto1006X
		Enm_Prd_Mars1717V5 = 0x00FF, 	 // Mars1717V5
		Enm_Prd_Mars1012P = 0x0100, 	 // Mars1012P
		Enm_Prd_DTDI256_230 = 0x0101, 	 // DTDI256_230
		Enm_Prd_Pluto0706X = 0x0102, 	 // Pluto0706X
		Enm_Prd_Pluto0015X = 0x0103, 	 // Pluto0015X
		Enm_Prd_Mercu0909X1 = 0x0104, 	 // Mercu0909X1
		Enm_Prd_NDT1717LA = 0x010E, 	 // NDT1717LA
		Enm_Prd_NDT1717BA = 0x0110, 	 // NDT1717BA
		Enm_Prd_Luna1717U = 0x0111, 	 // Luna1717U
		Enm_Prd_Jupi0808X = 0x0112, 	 // Jupi0808X
		Enm_Prd_NDT1212P = 0x011C, 	 // NDT1212P
		Enm_Prd_NDT0909P = 0x011D, 	 // NDT0909P
		Enm_Prd_Mercu0810DE = 0x011F, 	 // Mercu0810DE
		Enm_Prd_Mercu1417XM = 0x010C, 	 // Mercu1417XM
		Enm_Prd_NDT1717MA = 0x0120, 	 // NDT1717MA
		Enm_Prd_Mercu1724HE = 0x0124, 	 // Mercu1724HE
		Enm_Prd_Jupi0707X = 0x0125, 	 // Jupi0707X
		Enm_Prd_Mercu0606X1 = 0x0126, 	 // Mercu0606X1
		Enm_Prd_NDT1717V5 = 0x012B, 	 // NDT1717V5
		Enm_Prd_NDT1724HE = 0x012D, 	 // NDT1724HE
		Enm_Prd_NDT1748HE = 0x012E, 	 // NDT1748HE
		Enm_Prd_Mercu1724VE = 0x0133, 	 // Mercu1724VE
		Enm_Prd_NDT0506X = 0x0127, 	 // NDT0506X
		Enm_Prd_DTDI128_400 = 0x0356, 	 // DTDI128_400
		Enm_Prd_Mercu1717HX = 0x0132, 	 // Mercu1717HX
		Enm_Prd_NDT1616M2 = 0x00F2, 	 // NDT1616M2
		Enm_Prd_Mercu2524HE = 0x012C, 	 // Mercu2524HE
		Enm_Prd_NDT0203P_VSM = 0x0136, 	 // NDT0203P_VSM
		Enm_Prd_DTDI256_460 = 0x0135, 	 // DTDI256_460
		Enm_Prd_NDT0410BA = 0x013C, 	 // NDT0410BA
		Enm_Prd_Jupi0707X1 = 0x0139, 	 // Jupi0707X1
		Enm_Prd_Jupi0808X1 = 0x013A, 	 // Jupi0808X1
		Enm_Prd_Jupi0810X1 = 0x013B, 	 // Jupi0810X1
		Enm_Prd_NDT1012P = 0x0141, 	 // NDT1012P
	};
}
