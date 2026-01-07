/**
* File: IRayEventDef.cs
*
* Purpose: IRay FPD callback event definition
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
		public const int Evt_GeneralInfo = 1; // General Info
		public const int Evt_GeneralWarn = 2; // General Warn
		public const int Evt_GeneralError = 3; // General Error
		public const int Evt_TaskResult_Succeed = 4; // Task succeed
		public const int Evt_TaskResult_Failed = 5; // Task failed
		public const int Evt_TaskResult_Canceled = 6; // Task been canceled
		public const int Evt_AutoTask_Started = 7; // Task is started which is not emitted by user command, outer or AED image acquiring will trigger this event.
		public const int Evt_ScanOnce_Request = 8; // Request scanner to start a broadcast scanning
		public const int Evt_ImageFileUpload_Result = 9; // Image file upload result notify
		public const int Evt_TemplateFileUpload_Result = 10; // Template file upload result notify
		public const int Evt_TemplateFileDownload_Result = 11; // Template file download result notify
		public const int Evt_HwCaliTemplateList = 12; // Return hardware calibration template list
		public const int Evt_HwWiFiScanList = 13; // Return hardware WiFi scan list
		public const int Evt_HistoricalImageList = 14; // Return hardware historical image list
		public const int Evt_TimeDiff = 15; // Return time difference between detector and PC in seconds
		public const int Evt_LivingTime = 16; // Return living time from detector in seconds
		public const int Evt_TransactionAborted = 17; // Notify that current transaction was aborted
		public const int Evt_HwAcquisitionSettingList = 18; // Return hardware acquisition setting list
		public const int Evt_Image = 1001; // Image received
		public const int Evt_Prev_Image = 1002; // Preview image received
		public const int Evt_Retransfer_Image = 1012; // Retransfered image received after break and re-connecting
		public const int Evt_WaitImage_Timeout = 1003; // Acquisition operation time out(equal: Possible Image Loss)
		public const int Evt_Exp_Prohibit = 1004; // Exposure prohibit
		public const int Evt_Exp_Enable = 1005; // Exposure enable
		public const int Evt_Exp_Timeout = 1006; // Exposure timeout
		public const int Evt_OffsetAmassingTime = 1008; // Pixel base amassing time,typically from previous clear operation to current acquisition
		public const int Evt_MistakenTrigger = 1009; // Mistaken trigger in FreeSync mode
		public const int Evt_Image_Trans_Slow = 1010; // Warn that image transfer speed low, usually at wireless connection.
		public const int Evt_Image_Abandon = 1011; // Warn that currently acquiring image abandoned because bad network situation
		public const int Evt_LastImageID = 1013; // Return the image ID while invoking Cmd_QueryLastImageID
		public const int Evt_CustomROM = 1014; // Return the customized data while invoking Cmd_ReadCustomROM
		public const int Evt_LicenseData = 1015; // Return the license data while invoking Cmd_ReadLicenseData
		public const int Evt_NextStitchingPositionEnable = 1016; // Inform the system that the acquisition of post-offset dark image is finished
		public const int Evt_StitchingFlowTimeout = 1017; // If two stitching exposures time out, the detector will exit the stitching workflow and back to normal workflow
		public const int Evt_FreshImageIDs = 1018; // Return remain images that haven't been transferred. If all the images have been transferred, then nFirstImageID and nFreshImageCount will be "0"
		public const int Evt_CustomData = 1019; // Return the customized data while invoking Cmd_ReadCustomData
		public const int Evt_AutoPoweroff = 1020; // Power off warn
		public const int Evt_ConnectProcess = 2001; // Report Process info while connecting
		public const int Evt_CommFailure = 2002; // Communication failed
		public const int Evt_TemperatureHigh = 2003; // Temperature too high
		public const int Evt_FpsOutOfRange = 2004; // Fps too high or too low
		public const int Evt_LowBattery = 2005; // Low battery warn
		public const int Evt_TemplateOverDue = 2006; // Calibration template file over due
		public const int Evt_SWHWDataNotMatch = 2007; // The correct data of software and hardware not match, for example:Software defect template create time not same as Software defect template create time
		public const int Evt_DefectPointsIncreased = 2008; // Notify user after new defect map generated if new defect points are found.
		public const int Evt_CtrlBoxOnlineState = 2009; // Notify user current controlbox connection state changed
		public const int Evt_IdcRegionConflict = 2010; // Notify user Region selection conflict
		public const int Evt_TemperatureWarning = 2011; // Temperature is not ideal
		public const int Evt_IdcCutOffThresholdParams = 2015; // Return IDC cut-off threshold parameters
		public const int Evt_IdcActiveRegionData = 2017; // Return IDC active region selection
		public const int Evt_Idc_ControlRequest = 2018; // Notify user need request IDC control again
		public const int Evt_Idc_RequestData = 2019; // Return IDC specified data while invoking Cmd_IDC_Request
		public const int Evt_FwMismatch = 2030; // Notify user current firmware version mismatch with main version
		public const int Evt_FpdSelfTestFailed = 2031; // Notify user current panel connected, but it working in error state.
		public const int Evt_IdcNetworkInstability = 2032; // Notify user current IDC function network instability
		public const int Evt_Idc_XRayCutOffRequest = 2033; // Notify user need cut off X-Ray immediately
		public const int Evt_LicenseExpireWarnning = 2034; // Each time a connection is established, the user is notified that the license is about to expire, starting from the last week before the expiration.
		public const int Evt_LicenseExpired = 2035; // After the License expires, this message is displayed indicating that the License has expired when connect to the detector or acquire an image
	}
}
