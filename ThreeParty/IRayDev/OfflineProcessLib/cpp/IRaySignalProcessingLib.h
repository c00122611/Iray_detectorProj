/**
* File: IRaySignalProcessingLib.h
*This Library is but all kinds of Post-Processing algorithms for X-ray image, which are developed independently by IRay software.
*
* @author Haitao.Ning, Zhu.Zhe
* @version 1 2017/03/21
*
* Copyright (C) 2017, iRay Technology (Shanghai) Ltd.
*
*/

#ifndef _ISIGNAL_PROCESSING_LIB_H
#define _ISIGNAL_PROCESSING_LIB_H

#define IRAY_SP_SET_USERCODE        "SetUserCode"
#define IRAY_SP_EMICORRECTION       "EMICorrection"
#define IRAY_SP_DYNPROC_INIT        "InitializeDynamicProcessingByPreviousFrames"
#define IRAY_SP_DYNPROC_DO          "DoDynamicProcessingByPreviousFrames"
#define IRAY_SP_DYNPROC_UNINIT      "UninitializeDynamicProcessingByPreviousFrames"
#define IRAY_SP_OFFLINECORR_INIT    "InitializeOfflineCorrection"
#define IRAY_SP_OFFLINECORR_APPLY   "ApplyOfflineCorrection"
#define IRAY_SP_OFFLINECORR_UNINIT  "UninitializeOfflineCorrection"
#define IRAY_SP_CORRECTION_DEFECT   "ApplyDefectCorrection"
#define IRAY_SP_ENLARGE_IMAGE       "EnlargeImage"
#define IRAY_SP_GENERATE_DEFECT     "GenerateDefect"
#define IRAY_SP_CORRECTION_EXPLINE  "ApplyExpLineCorrection"
#define IRAY_SP_CORRECTION_BADLINES "ApplyBadLinesCorrection"

//Define the error code type
typedef int LAB_ERRCODE;

//Error code description
const LAB_ERRCODE LabErr_OK = 0;     // Pass
const LAB_ERRCODE LabErr_Config_Read_Failed = 4001;   // Config file parameter read error
const LAB_ERRCODE LabErr_UnKnown_Process_Intensity = 4002;   // Processing level selection is wrong
const LAB_ERRCODE LabErr_No_Config_File = 4003;   // Can not find config file 
const LAB_ERRCODE LabErr_AccessDenied = 4004;   // No enough authority to execute corresponding module
const LAB_ERRCODE LabErr_GeneralErr = 4005;   // No enough authority to execute corresponding module

//struct OfflineCorrectionInfo: used as offline correction interface parameter
#pragma pack(1)
struct OfflineCorrectionInfo
{
	int nFpdProdNo;					   // could read from the attribute called "Attr_UROM_ProductNo" by SDK
	int nFpdTriggerMode;               // could read from the attribute called "Attr_UROM_TriggerMode" by SDK
	int nFpdPrepCapMode;               // could read from the attribute called "Attr_UROM_PrepCapMode" by SDK
	int nFpdSeqInterval;               // could read from the attribute called "Attr_UROM_SequenceIntervalTime" by SDK
    int nFpdExpMode;                   // could read from the attribute called "Attr_UROM_ExpMode" by SDK
	int nImgWidth;                     // width of current image
	int nImgHeight;                    // height of current image
	int nBinningWidth;                 // reserved
	int nBinningHeight;                // reserved
	wchar_t* pszFpdWorkDir;			   // work directory of current FPD in current mode
	wchar_t* pszFpdCaliSubset;	       // calibration subset of current FPD in current mode
	int nDoPreOffset;                  // whether do the Pre-Offset correction, 0 means no, other values mean yes
	int nDoGain;                       // whether do the Gain correction, 0 means no, other values mean yes
	int nDoDefect;                     // whether do the Defect correction, 0 means no, other values mean yes
	int nPreOffsetDone;                // whether current image has already done Pre-Offset correction, 0 means no, other values mean yes
	unsigned char byReserved[88];
};// total 148 bytes
#pragma pack()

//struct DefectInfo: used as defect correction interface parameter
#pragma pack(1)
struct DefectInfo
{
	unsigned short* pImgData;		   // data buffer of current image
	int nImgWidth;                     // width of current image
	int nImgHeight;                    // height of current image
	int nBinningWidth;                 // reserved
	int nBinningHeight;                // reserved
	int nFpdProdNo;					   // product number of current FPD(e.g. 0x0030 is the product number of Mercu1717V)
	const wchar_t* pszFpdWorkDir;	   // work directory of current FPD in current mode
    const wchar_t* pszFpdCaliSubset;   // calibration subset of current FPD in current mode
	int nTryDoubleBadLineDefect;       // reserved
	int nSoftwarePreOffset;            // 1 means current image has been applied Software Pre-Offset correction, just for products of Mars- series in FreeSync mode, otherwise please set this parameter to -1
	int nFreeSyncExpLine;              // exposure line, just for products of Mars- series in FreeSync mode, otherwise please set this parameter to -1
	int nFreeSyncExpStatus;            // exposure status, just for products of Mars- series in FreeSync mode, otherwise please set this parameter to -1
    const char* pszAecLines;           // iAEC lines
    int nFpdSubProdNo;				   // could read from the attribute called "Attr_UROM_SubProductNo" by SDK
	unsigned char byReserved[92];
};// total 148 bytes
#pragma pack()

//struct GenerateDefectInfo: used as interface parameter of GenerateDefect
#pragma pack(1)
struct GenerateDefectInfo
{
	int nFpdProdNo;					   // could read from the attribute called "Attr_UROM_ProductNo" by SDK
	int nImgWidth;                     // width of current image
	int nImgHeight;                    // height of current image
	wchar_t* pszConfigIniFilePath;	   // path of configuration file
	wchar_t* pszImageFolderPath;	   // path of image folder, please end with '\'
	wchar_t* pszOutputFolderPath;	   // path of output result folder, please end with '\'
	unsigned char byReserved[124];
};// total 148 bytes
#pragma pack()

/**
* FnSetUserCode: Define a function type for DLL export function "SetUserCode"
*
* SetUserCode: Set user code to obtain the authority specified by the license which is published by the mode of "UserCode".
*
* @param pszUserCode [in] User code to match the license
* 
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnSetUserCode)(const char* pszUserCode);

//EMI Processing Method
typedef int EMI_Processing_Method;

enum Enm_EMI_Processing_Level
{
	Enm_EMI_Processing_Level_L = 0,
	Enm_EMI_Processing_Level_M = 1,
	Enm_EMI_Processing_Level_H = 2,
};

/**
* FnEMICorrection: Define a function type for DLL export function "EMICorrection"
*
* EMICorrection: Apply EMI correction to an image captured by FPD.
*
* @param psImagein [in,out] image buffer
* @param nWidth [in] width of the image
* @param nHeight [in] height of the image
* @param bDummy [in] border dummy line options: 0 for no, 1 for yes
* @param nMethod [in] processing method options
* @param eLevel [in] processing intensity options: Enm_EMI_Processing_Level_L, Enm_EMI_Processing_Level_M, Enm_EMI_Processing_Level_H.
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnEMICorrection)(unsigned short * psImagein, int nWidth, int nHeight, bool bDummy, EMI_Processing_Method nMethod, Enm_EMI_Processing_Level eLevel);

/**
* FnInitializeDynamicProcessingByPreviousFrames: Define a function type for DLL export function "InitializeDynamicProcessingByPreviousFrames"
*
* InitializeDynamicProcessingByPreviousFrames: Make some preparation work before DoDynamicProcessingByPreviousFrames, such as allocate memories.
*                                              This function should be invoked before capture.
*
* @param nPixelCnt [in] count of pixels in each image frame
* @param fRecursionFactor [in] a factor for adjust the effect of recursion, should between 0.0 and 1.0, other values disable recursion.
* @param nCompositionCnt [in] max count of frames for composition, should be greater than 1, other values disable composition.
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnInitializeDynamicProcessingByPreviousFrames)(
	unsigned int nPixelCnt,
	float fRecursionFactor,
	unsigned short nCompositionCnt);


/**
* FnDoDynamicProcessingByPreviousFrames: Define a function type for DLL export function "DoDynamicProcessingByPreviousFrames"
*
* DoDynamicProcessingByPreviousFrames: Processing dynamic image frames by methods of recursion and composition.
*                                      This function should be invoked each time when a new frame comes.
*
* @param pOutImg [out] result image
* @param pInImg [in] source image
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnDoDynamicProcessingByPreviousFrames)(
	unsigned short* pOutImg,
	const unsigned short* pInImg);


/**
* FnUninitializeDynamicProcessingByPreviousFrames: Define a function type for DLL export function "UninitializeDynamicProcessingByPreviousFrames"
*
* UninitializeDynamicProcessingByPreviousFrames: Make some cleaning work after DoDynamicProcessingByPreviousFrames, such as free memories.
*                                                This function should be invoked after capture.
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnUninitializeDynamicProcessingByPreviousFrames)();


/**
* FnInitializeOfflineCorrection: Define a function type for DLL export function "InitializeOfflineCorrection"
*
* InitializeOfflineCorrection: Make some preparation work before FnApplyOfflineCorrection, such as allocate memories.
*
* @param pOfflineCorrInfo [in] all data and parameters about offline correction
* @param ppHandle [in/out] input address of a pointer type of void*, and then use this pointer to invoke other functions
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnInitializeOfflineCorrection)(const OfflineCorrectionInfo* pOfflineCorrInfo, void** ppHandle);


/**
* FnApplyOfflineCorrection: Define a function type for DLL export function "ApplyOfflineCorrection"
*
* ApplyOfflineCorrection: Apply offline correction to an image captured by iRay FPD.
*                         This function should be invoked each time when a new frame comes.
*
* @param pHandle [in] handle acquired by InitializeOfflineCorrection in a same group
* @param pImgData [in/out] source and result image
* @param pPostImg [in] post offset image if exist
* @param nFreeSyncExpLine [in] exposure line, just for products of Mars- series in FreeSync mode, otherwise please set this parameter to -1
* @param nFreeSyncExpStatus [in] exposure status, just for products of Mars- series in FreeSync mode, otherwise please set this parameter to -1
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnApplyOfflineCorrection)(
	void* pHandle, 
	unsigned short* pImgData, 
	const unsigned short* pPostImg,
	int nFreeSyncExpLine, 
	int nFreeSyncExpStatus);


/**
* FnUninitializeOfflineCorrection: Define a function type for DLL export function "UninitializeOfflineCorrection"
*
* UninitializeOfflineCorrection: Make some cleaning work after ApplyOfflineCorrection, such as free memories.
*
* @param pHandle [in] handle acquired by InitializeOfflineCorrection in a same group
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnUninitializeOfflineCorrection)(void* pHandle);


/**
* FnApplyDefectCorrection: Define a function type for DLL export function "ApplyDefectCorrection"
*
* ApplyDefectCorrection: Apply offline defect correction to an image captured by iRay FPD.
*
* @param pDftInfo [in] all data and parameters about defect
* 
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnApplyDefectCorrection)(const DefectInfo* pDftInfo);


/**
* FnEnlargeImage: Define a function type for DLL export function "EnlargeImage"
*
* EnlargeImage: Enlarge an image by pixel interpolation
*
* @param pImgDst [out] data buffer of destination image, the caller of function should allocate memory
* @param nDstWidth [in] width of destination image, should be greater than nSrcWidth
* @param nDstHeight [in] height of destination image, should be greater than nSrcHeight
* @param pImgSrc [in] data buffer of source image
* @param nSrcWidth [in] width of source image, should be less than or equal to 20480
* @param nSrcHeight [in] height of source image, should be less than or equal to 20480
* @param nMethod [in] 0/1/10/11 for different results, other values are reserved
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnEnlargeImage)(unsigned short* pImgDst, unsigned short nDstWidth, unsigned short nDstHeight,
	const unsigned short* pImgSrc, unsigned short nSrcWidth, unsigned short nSrcHeight, unsigned char nMethod);


/**
* FnGenerateDefect: Define a function type for DLL export function "GenerateDefect"
*
* GenerateDefect: Generate defect template and statistical result
*
* @param pInfo [in] all data and parameters about defect generation
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnGenerateDefect)(const GenerateDefectInfo* pInfo);


/**
* FnApplyDefectCorrection: Define a function type for DLL export function "ApplyExpLineCorrection"
*
* ApplyDefectCorrection: Apply offline exposure line correction to an image captured by iRay FPD.
*
* @param pDftInfo [in] all data and parameters about defect
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnApplyExpLineCorrection)(const DefectInfo* pDftInfo);


/**
* FnApplyDefectCorrection: Define a function type for DLL export function "ApplyBadLinesCorrection"
*
* ApplyDefectCorrection: Apply offline bad lines correction to an image captured by iRay FPD.
*
* @param pDftInfo [in] all data and parameters about defect
*
* @return 0: succeed, others: failed
*/
typedef	LAB_ERRCODE(*FnApplyBadLinesCorrection)(const DefectInfo* pDftInfo);


#endif //_ISIGNAL_PROCESSING_LIB_H