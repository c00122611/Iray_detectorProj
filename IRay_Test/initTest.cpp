/*
 * @brief: 探测器连接测试
 * @date:
 * @file:
 */
#include "stdafx.h"
#include "IRayInclude.h"
#include "Detector.h"

static CDetector* gs_pDetInstance = NULL;
int Initializte();
void Deinit();
//回调函数初始化
void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam)
{
	gs_pDetInstance->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);
	switch (nEventID)
	{
	case Evt_ConnectProcess:
		//TRACE(pszMsg);
		//TRACE("\n");
		break;
	default:
		break;
	}
}
//int main(int argc, char* argv[])
//{	
//	// 初始化探测器类
//	gs_pDetInstance = new CDetector();
//	do
//	{
//		TRACE("Load libray");
//		// dll加载
//		int ret = gs_pDetInstance->LoadIRayLibrary();
//		if (Err_OK != ret)
//		{
//			TRACE("\t\t\t[No ]\n");
//			break;
//		}
//		else
//			TRACE("\t\t\t[Yes]\n");
//
//		TRACE("Create instance");
//		string str = GetWorkDirPath().c_str();
//		//设置回调函数
//		ret = gs_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
//		if (Err_OK != ret)
//		{
//			TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
//			return ret;
//		}
//		else
//			TRACE("\t\t\t[Yes]\n");
//		//执行初始化逻辑
//		Initializte();
//	} while (false);
//
//	TRACE("Press [Enter] to exit\n");
//	getchar();
//	Deinit();
//	return 0;
//}

//初始化连接
int Initializte()
{	//连接设备，调用 Cmd_Connect命令
	TRACE("Connect device");
	int ret = gs_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	//Notice:It's necessary for Mercu-series products, or skip this step
	TRACE("Set application-mode");
	//"Mode1" defined in DynamicApplicationMode.ini subset=Mode1
	ret = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode1", 5000);
	if (Err_OK != ret)
	{
		TRACE("\t\t[No ]\n");
		return ret;
	}
	else
		TRACE("\t\t[Yes]\n");

	TRACE("Set correction option");
	ret = gs_pDetInstance->SyncInvoke(Cmd_SetCorrectOption, Enm_CorrectOp_SW_PreOffset | Enm_CorrectOp_SW_Gain | Enm_CorrectOp_SW_Defect, 5000);
	if (Err_OK != ret)
	{
		TRACE("\t\t[No ]\n");
		return ret;
	}
	else
		TRACE("\t\t[Yes]\n");

	return Err_OK;
}
//断开连接
void Deinit()
{
	if (gs_pDetInstance)
	{
		gs_pDetInstance->Destroy();
		gs_pDetInstance->FreeIRayLibrary();
		delete gs_pDetInstance;
		gs_pDetInstance = NULL;
	}
}