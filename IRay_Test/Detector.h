#ifndef _DETECTOR_H
#define _DETECTOR_H
#include <Windows.h>
#ifdef WIN32
typedef HMODULE LIBMODULE;
#define ExtractFunc			GetProcAddress
#define OpenDLib(libname)	LoadLibrary(L##libname)
#define CloseDLib			FreeLibrary
#define Sleep				Sleep
#else
#include <dlfcn.h>
#include <unistd.h>
typedef void* LIBMODULE;
#define ExtractFunc 		dlsym
#define OpenDLib(libname)	dlopen("lib" libname".so", RTLD_LAZY)
#define CloseDLib			dlclose
#define L                   ""
#define Sleep(time)			usleep(time*1000)
#endif

#include "IRayFpdSys.h"
#include "IRayFpdSysEx.h"
#include "IRayDetFinder.h"
#include "Util.h"
#include "winevent.h"
#include <vector>
#include <string>
#include <stdio.h>
using namespace::std;

#define TRACE printf

class IRayCmdObject
{
public:
    enum datatype { INT, FLT, STR};
public:
    IRayCmdObject(int data) :m_intV(data), m_type(INT){}
    IRayCmdObject(double data) :m_doubleV(data), m_type(FLT){}
    IRayCmdObject(const char* data) :m_strV(data), m_type(STR){}
    IRayCmdObject(const string& data) :m_strV(data), m_type(STR){}
    operator int()
    {
        return m_intV;
    }
    operator float()
    {
        return (float)m_doubleV;
    }
    operator std::string()
    {
        return m_strV;
    }
    datatype gettype() const{ return m_type; }
private:
    datatype m_type;
    int m_intV;
    double m_doubleV;
    std::string m_strV;
};

union AttrResult
{
	int   nVal;
	float fVal;
	char  strVal[IRAY_MAX_STR_LEN];
};

class CDetectorManager;
class iEventReceiver;
class CDetector
{
    void ParseIRayCmdObject(std::vector<IRayCmdParam>& paramlist, IRayCmdObject obj)
    {
        IRayCmdParam cmdparam;
        if (obj.gettype() == IRayCmdObject::INT)
        {
            cmdparam.pt = IPT_VARIANT;
            cmdparam.var.vt = IVT_INT;
            cmdparam.var.val.nVal = obj;
        }
        else if (obj.gettype() == IRayCmdObject::FLT)
        {
            cmdparam.pt = IPT_VARIANT;
            cmdparam.var.vt = IVT_FLT;
            cmdparam.var.val.fVal = obj;
        }
        else if (obj.gettype() == IRayCmdObject::STR)
        {
            cmdparam.pt = IPT_VARIANT;
            cmdparam.var.vt = IVT_STR;
            const std::string& strV = obj;
            cmdparam.var.val.strVal[strV.length()] = '\0';
            strncpy_s(cmdparam.var.val.strVal, strV.c_str(), strV.length());
        }
        paramlist.emplace_back(cmdparam);
    }
    void ParseCmdParams(std::vector<IRayCmdParam>& param, IRayCmdObject obj)
    {
        ParseIRayCmdObject(param, obj);
    }
    template<typename ... Args>
    void ParseCmdParams(std::vector<IRayCmdParam>& param, IRayCmdObject first, Args ...rest)
    {
        ParseIRayCmdObject(param, first);
        ParseCmdParams(param, rest...);
    }
public:
	CDetector();
	~CDetector();
	int  LoadIRayLibrary();
	void FreeIRayLibrary();

    void RegisterScanCB(FnNotifyScanResult scanRetCB);
    void RegisterScanExCB(FnNotifyScanResultEx scanRetCB);
    DEPRECATED("Consider using ScanOnceEx instead")
    void ScanOnce(const char* ip);
    void ScanOnceEx(Enm_CommChannel eScanType, void* pInParam=nullptr);
    int GetAuthority();

	FPDRESULT Create(const char* pszWorkDir, FnCallback fpCallback);
	FPDRESULT Destroy();
	FPDRESULT SetAttr(int nAttrID, int nValue);
	FPDRESULT SetAttr(int nAttrID, float fValue);
	FPDRESULT SetAttr(int nAttrID, const char* strValue);
	
    DEPRECATED("it's deprecated")
	int GetAttr(int nAttrID, AttrResult& result);

    int GetAttr(int nAttrID, int& retV);
    int GetAttr(int nAttrID, float& retV);
    int GetAttr(int nAttrID, std::string& retV);

    int GetAttrInt(int nAttrID)
    {
        int nValue = 0;
        GetAttr(nAttrID, nValue);
        return nValue;
    }

    FPDRESULT Invoke(int nCmdId)
    {
        if (!m_bInitilized)
            return Err_NotInitialized;

        ResetEvent(m_WaitAckEvent);
        m_CurCmdId = nCmdId;
        return m_pFnInvoke(m_nDetectorID, nCmdId, nullptr, 0);
    }
    template<typename ... Args>
    FPDRESULT Invoke(int nCmdId, Args...args)
    {
        if (!m_bInitilized)
            return Err_NotInitialized;

        ResetEvent(m_WaitAckEvent);
        m_CurCmdId = nCmdId;
        std::vector<IRayCmdParam> cmdParamList;
        ParseCmdParams(cmdParamList, args...);
        IRayCmdParam *pCmdParams = &cmdParamList[0];
        return m_pFnInvoke(m_nDetectorID, nCmdId, pCmdParams, cmdParamList.size());
    }

    FPDRESULT ParamInvoke(int cmdId, IRayCmdParam* paramValue, int paramLength, int timeOut)
    {
        ResetEvent(m_WaitAckEvent);
        m_CurCmdId = cmdId;
        FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, paramValue, paramLength);
        if (Err_TaskPending == result)
        {
            result = WaitEvent(timeOut);
        }
        return result;
    }

    //the last parameter is timeout
    template<typename ... Args>
    FPDRESULT SyncInvoke(int nCmdId, Args...args)
    {
        if (!m_bInitilized)
            return Err_NotInitialized;

        ResetEvent(m_WaitAckEvent);
        m_CurCmdId = nCmdId;
        std::vector<IRayCmdParam> cmdParamList;
        ParseCmdParams(cmdParamList, args...);
        int timeout = cmdParamList[cmdParamList.size() - 1].var.val.nVal;
        IRayCmdParam *pCmdParams = &cmdParamList[0];
        FPDRESULT result = m_pFnInvoke(m_nDetectorID, nCmdId, pCmdParams, cmdParamList.size() - 1);
        if (Err_TaskPending == result)
        {
            result = WaitEvent(timeout);
        }
        return result;
    }

	FPDRESULT WriteCustomerROM(void* pBlockData, unsigned size);
    FPDRESULT ReadCustomerROM();
    FPDRESULT Abort();
    FPDRESULT SetUserCode(char* pszUserCode);

    FPDRESULT UseImageBuf(unsigned long long ullBufSizeInBytes);
    FPDRESULT ClearImageBuf();
    FPDRESULT QueryImageBuf(int& nFrameNum, int& nImageSize, int& nPropListMemSize);
    FPDRESULT GetImageFromBuf(void* pImage, int nImageDataSize, int nPropListMemSize, int& nFrameNum);

    string    GetErrorInfo(int nErrorCode);
    string    GetSDKVersion();
	int       DetectorID(){ return m_nDetectorID; }
	void      SDKCallback(int nDetectorID, int nEventID, int nEventLevel,
						  const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);
	int WaitEvent(int timeout);

    int GetImagePropertyInt(IRayVariantMap* pProperties, int nTagId);

	int OpenDefectTemplateFile(
		const char* pszFilePath,
		void** ppHandler,
		unsigned short* pWidth,
		unsigned short* pHeight,
		char** ppPoints);
	int SaveDefectTemplateFile(void* pHandler);
	int CloseDefectTemplateFile(void* pHandler);

	static const int OFFSETMASK;
	static const int GAINMASK;
	static const int DEFECTMASK;

    FnOpenDefectTemplateFile m_pFnOpenDefectTemplateFile;
    FnSaveDefectTemplateFile m_pFnSaveDefectTemplateFile;
    FnCloseDefectTemplateFile m_pFnCloseDefectTemplateFile;
private:
	bool m_bInitilized;
	LIBMODULE m_hModule;
	FnCreate  m_pFnCreate;
	FnDestroy m_pFnDestroy;
	FnGetAttr m_pFnGetAttr;
	FnSetAttr m_pFnSetAttr;
	FnInvoke  m_pFnInvoke;
	FnAbort   m_pFnAbort;
    FnSetUserCode  m_pFnSetUserCode;
    FnReleaseLibraryResource m_pFnReleaseLibraryResource;
    FnGetErrInfo m_pFnGetErrInfo;
    FnGetSDKVersion m_pFnGetSDKVersion;
    FnScanOnce m_pFnScanOnce;
    FnScanOnceEx m_pFnScanOnceEx;
    FnGetAuthority m_pFnGetAuthority;
    FnRegisterScanNotify m_pFnRegisterScanNotify;
    FnRegisterScanNotifyEx m_pFnRegisterScanNotifyEx;
    FnUseImageBuf m_pFnUseImageBuf;
    FnClearImageBuf m_pFnClearImageBuf;
    FnQueryImageBuf m_pFnQueryImageBuf;
    FnGetImageFromBuf m_pFnGetImageFromBuf;


	int m_nDetectorID;
	HEVENT m_WaitAckEvent;
	int m_CurCmdId;
	int m_nLastError;
};

std::string GetWorkDirPath();

#endif