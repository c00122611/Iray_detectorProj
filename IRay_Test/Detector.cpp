#include "Detector.h"
#include <string.h>

#define EXTRACT_FUNC(funcPtr, flag) {m_p##funcPtr = (funcPtr)ExtractFunc(m_hModule, #flag); if(NULL==m_p##funcPtr) return Err_LoadDllFailed;}

const int CDetector::OFFSETMASK = Enm_CorrectOp_SW_PreOffset | Enm_CorrectOp_SW_PostOffset | Enm_CorrectOp_HW_PreOffset | Enm_CorrectOp_HW_PostOffset;
const int CDetector::GAINMASK = Enm_CorrectOp_SW_Gain | Enm_CorrectOp_HW_Gain;
const int CDetector::DEFECTMASK = Enm_CorrectOp_SW_Defect | Enm_CorrectOp_HW_Defect;

#if defined(linux)
bool IRayTimer::m_stoptimerFlag;
#endif
timeproc IRayTimer::timercallback=NULL;
CDetector::CDetector()
: m_nDetectorID(0)
, m_bInitilized(false)
{
	m_WaitAckEvent = CreateEvent(NULL, false, false, NULL);
}

CDetector::~CDetector()
{
	Destroy();
	FreeIRayLibrary();
	if (m_WaitAckEvent)
	{
		CloseHandle(m_WaitAckEvent);
		m_WaitAckEvent = NULL;
	}
}

int CDetector::LoadIRayLibrary()
{	
	m_hModule = OpenDLib("FpdSys.dll");
	if (NULL == m_hModule)
	{

		return Err_LoadDllFailed;
	}
	EXTRACT_FUNC(FnCreate, Create);
	EXTRACT_FUNC(FnDestroy, Destroy);
	EXTRACT_FUNC(FnGetAttr, GetAttr);
	EXTRACT_FUNC(FnSetAttr, SetAttr);
	EXTRACT_FUNC(FnInvoke, Invoke);
	EXTRACT_FUNC(FnAbort, Abort);
    EXTRACT_FUNC(FnSetUserCode, SetUserCode);
    EXTRACT_FUNC(FnGetErrInfo, GetErrInfo);
    EXTRACT_FUNC(FnReleaseLibraryResource, ReleaseLibraryResource);
    EXTRACT_FUNC(FnGetSDKVersion, GetSDKVersion);
    EXTRACT_FUNC(FnScanOnce, ScanOnce);
    EXTRACT_FUNC(FnScanOnceEx, ScanOnceEx);
    EXTRACT_FUNC(FnGetAuthority, GetAuthority);
    EXTRACT_FUNC(FnRegisterScanNotify, RegisterScanNotify);
    EXTRACT_FUNC(FnRegisterScanNotifyEx, RegisterScanNotifyEx)
    EXTRACT_FUNC(FnUseImageBuf, UseImageBuf);
    EXTRACT_FUNC(FnClearImageBuf, ClearImageBuf);
    EXTRACT_FUNC(FnQueryImageBuf, QueryImageBuf);
    EXTRACT_FUNC(FnGetImageFromBuf, GetImageFromBuf);
    EXTRACT_FUNC(FnOpenDefectTemplateFile, OpenDefectTemplateFile);
    EXTRACT_FUNC(FnSaveDefectTemplateFile, SaveDefectTemplateFile);
    EXTRACT_FUNC(FnCloseDefectTemplateFile, CloseDefectTemplateFile);
	m_bInitilized = true;
	return Err_OK;
}

void CDetector::FreeIRayLibrary()
{
	if (m_hModule)
	{
		CloseDLib(m_hModule);
		m_hModule = NULL;
	}
}
int CDetector::GetAuthority()
{
    int nAuthority = 0;
    return m_pFnGetAuthority(&nAuthority);
}

void CDetector::ScanOnce(const char* ip)
{
    m_pFnScanOnce((char*)ip);
}
void CDetector::ScanOnceEx(Enm_CommChannel eScanType, void* pInParam /*= nullptr*/)
{
    m_pFnScanOnceEx(eScanType, pInParam);
}

void CDetector::RegisterScanCB(FnNotifyScanResult scanRetCB)
{
    m_pFnRegisterScanNotify(scanRetCB);
}
void CDetector::RegisterScanExCB(FnNotifyScanResultEx scanRetCB)
{
    m_pFnRegisterScanNotifyEx(scanRetCB);
}

FPDRESULT CDetector::Create(const char* pszWorkDir, FnCallback fpCallback)
{
	if (!m_bInitilized)
	{
		FPDRESULT nRet = LoadIRayLibrary();
		if (Err_OK != nRet)
		{
			return nRet;
		}
	}

	FPDRESULT nRet = m_pFnCreate(pszWorkDir, fpCallback, &m_nDetectorID);
	if (Err_OK != nRet)
	{
		return nRet;
	}

	return nRet;
}


FPDRESULT CDetector::Destroy()
{
	if (m_nDetectorID > 0)
	{
		m_pFnDestroy(m_nDetectorID);
        m_pFnReleaseLibraryResource();
		m_nDetectorID = 0;
	}
	return Err_OK;
}

FPDRESULT CDetector::Abort()
{
	return m_pFnAbort(m_nDetectorID);
}
FPDRESULT CDetector::SetUserCode(char* pszUserCode)
{
    return m_pFnSetUserCode(pszUserCode);
}

FPDRESULT CDetector::UseImageBuf(unsigned long long ullBufSizeInBytes)
{
    return m_pFnUseImageBuf(m_nDetectorID, ullBufSizeInBytes);
}

FPDRESULT CDetector::ClearImageBuf()
{
    return m_pFnClearImageBuf(m_nDetectorID);
}

FPDRESULT CDetector::QueryImageBuf(int& nFrameNum, int& nImageSize, int& nPropListMemSize)
{
    int nImageHeight, nImageWidth, nBytesPerPixel;
    int result = m_pFnQueryImageBuf(m_nDetectorID, &nFrameNum, &nImageHeight, &nImageWidth, &nBytesPerPixel, &nPropListMemSize);
    nImageSize = nImageHeight * nImageWidth * nBytesPerPixel;
    return result;
}

FPDRESULT CDetector::GetImageFromBuf(void* pImage, int nImageDataSize, int nPropListMemSize, int& nFrameNum)
{
    IRayVariantMap pProperties;
    pProperties.nItemCount = nPropListMemSize / sizeof(IRayVariantMapItem);
    pProperties.pItems = new IRayVariantMapItem[pProperties.nItemCount];
    int result = Err_Unknown;
    do{
        result = m_pFnGetImageFromBuf(m_nDetectorID, pImage, nImageDataSize, pProperties.pItems, nPropListMemSize);
        if (Err_OK != result)
            break;
        nFrameNum = GetImagePropertyInt(&pProperties, Enm_ImageTag_FrameNo);
    } while (false);

    delete[]pProperties.pItems;
    return result;
}

FPDRESULT CDetector::SetAttr(int nAttrID, int nValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	IRayVariant var;
	var.vt = IVT_INT;
	var.val.nVal = nValue;
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}

FPDRESULT CDetector::SetAttr(int nAttrID, float fValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	IRayVariant var;
	var.vt = IVT_FLT;
	var.val.fVal = fValue;
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}
FPDRESULT CDetector::SetAttr(int nAttrID, const char* strValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	if (!strValue)
		return Err_InvalidParamValue;

	IRayVariant var;
	var.vt = IVT_STR;
	strncpy(var.val.strVal, strValue, IRAY_MAX_STR_LEN-1);
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}


int CDetector::GetAttr(int nAttrID, AttrResult& result)
{
	IRayVariant var;
	FPDRESULT ret = m_pFnGetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != ret)
	{
		memset(&result, 0, sizeof(result));
		return ret;
	}

	if (IVT_INT == var.vt)
	{
		result.nVal = var.val.nVal;
	}
	else if (IVT_FLT == var.vt)
	{
		result.fVal = var.val.fVal;
	}
	else if (IVT_STR == var.vt)
	{
		memcpy(result.strVal, var.val.strVal, IRAY_MAX_STR_LEN);
	}
    return Err_OK;
}

int CDetector::GetAttr(int nAttrID, int& retV)
{
    IRayVariant var;
    FPDRESULT ret = m_pFnGetAttr(m_nDetectorID, nAttrID, &var);
    if (Err_OK != ret)
    {
        return ret;
    }
    if (var.vt != IRAY_VAR_TYPE::IVT_INT)
    {
        return Err_InvalidParamType;
    }

    retV = var.val.nVal;
    return Err_OK;
}
int CDetector::GetAttr(int nAttrID, float& retV)
{
    IRayVariant var;
    FPDRESULT ret = m_pFnGetAttr(m_nDetectorID, nAttrID, &var);
    if (Err_OK != ret)
    {
        return ret;
    }
    if (var.vt != IRAY_VAR_TYPE::IVT_FLT)
    {
        return Err_InvalidParamType;
    }
    retV = var.val.fVal;
    return Err_OK;
}
int CDetector::GetAttr(int nAttrID, std::string& retV)
{
    IRayVariant var;
    FPDRESULT ret = m_pFnGetAttr(m_nDetectorID, nAttrID, &var);
    if (Err_OK != ret)
    {
        return ret;
    }
    if (var.vt != IRAY_VAR_TYPE::IVT_STR)
    {
        return Err_InvalidParamType;
    }
    retV = var.val.strVal;
    return Err_OK;
}


string CDetector::GetErrorInfo(int nErrorCode)
{
	ErrorInfo info;
	m_pFnGetErrInfo(nErrorCode, &info);
	return info.szDescription;
}

string CDetector::GetSDKVersion()
{
    char version[32] = { 0 };
    m_pFnGetSDKVersion(version);
    return string(version);
}


//编程指南有说明，total 512 bytes
FPDRESULT CDetector::WriteCustomerROM(void* pBlockData, unsigned size)
{
	if (!m_bInitilized)
		return Err_NotInitialized;
	if (size > 512)
		return Err_InvalidParamCount;
	ResetEvent(m_WaitAckEvent);
    m_CurCmdId = Cmd_WriteCustomROM;
	char mask[512] = { 0 };
	memset(mask, 1, size);
	IRayCmdParam param[2];
	param[0].pt = IPT_BLOCK;
	param[0].blc.pData = pBlockData;
	param[0].blc.uBytes = 512;
	param[1].pt = IPT_BLOCK;
	param[1].blc.pData = mask;
	param[1].blc.uBytes = 512;
    FPDRESULT result = m_pFnInvoke(m_nDetectorID, Cmd_WriteCustomROM, param, 2);
	if (Err_OK != result && Err_TaskPending != result)
	{
		//print("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
	}
	return result;
}

FPDRESULT CDetector::ReadCustomerROM() //读512参数结果通过 Evt_CustomROM 返回 ，解析按编程指南说明
{
    ResetEvent(m_WaitAckEvent);
    m_CurCmdId = Cmd_ReadCustomROM;
    FPDRESULT result = m_pFnInvoke(m_nDetectorID, Cmd_ReadCustomROM, nullptr, 0);
    if (Err_OK != result && Err_TaskPending != result)
    {
        //print("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
    }

    return result;
}

int CDetector::WaitEvent(int timeout)
{
	int wait = WaitForSingleObject(m_WaitAckEvent, timeout);
	if (WAIT_TIMEOUT == wait)
		return Err_TaskTimeOut;
	else
		return m_nLastError;
}

void CDetector::SDKCallback(int nDetectorID, int nEventID, int nEventLevel,
	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam)
{
    if ((Evt_TaskResult_Succeed == nEventID) || (Evt_TaskResult_Failed == nEventID) || (Evt_TaskResult_Canceled == nEventID))
	{

        if (Evt_TaskResult_Canceled == nEventID)
            m_nLastError = Err_Unknown;
        else
		    m_nLastError = nParam2;
		if (m_CurCmdId == nParam1)
		{
			SetEvent(m_WaitAckEvent);
		}
	}
}

int CDetector::GetImagePropertyInt(IRayVariantMap* pProperties, int nTagId)
{
    if (!pProperties) 
        return -1;

    for (int nItemIndex=0; nItemIndex < pProperties->nItemCount; nItemIndex++)
    {
        if (nTagId == pProperties->pItems[nItemIndex].nMapKey)
        {
            return pProperties->pItems[nItemIndex].varMapVal.vt == IVT_INT ? pProperties->pItems[nItemIndex].varMapVal.val.nVal : -1;
        }
    }
    return -1;
}

int CDetector::OpenDefectTemplateFile(
	const char* pszFilePath,
	void** ppHandler,
	unsigned short* pWidth,
	unsigned short* pHeight,
	char** ppPoints)
{
    char* ppRows = nullptr;
    char* ppCols = nullptr;
    char* ppDualReadCols2 = nullptr;
	return m_pFnOpenDefectTemplateFile(pszFilePath,ppHandler,pWidth, pHeight,ppPoints, &ppRows, &ppCols, &ppDualReadCols2);
}

int CDetector::SaveDefectTemplateFile(void* pHandler)
{
	return m_pFnSaveDefectTemplateFile(pHandler);
}
int CDetector::CloseDefectTemplateFile(void* pHandler)
{
	return m_pFnCloseDefectTemplateFile(pHandler);
}

std::string GetWorkDirPath()
{
	char buff[128] = {0};
	FILE* pFile = NULL;
	std::string workdir("");
	pFile = fopen("..\\work_dir\\workdir_path.txt", "r");
	if (pFile)
	{
		fgets(buff, 128, pFile);
		fclose(pFile);
		workdir = std::string(buff);
	}
	else
	{
		pFile = fopen("workdir_path.txt", "r");
		if (pFile)
		{
			fgets(buff, 128, pFile);
			fclose(pFile);
			workdir = std::string(buff);
		}
	}
	return workdir.erase(workdir.find_last_not_of("\n") + 1);
}
