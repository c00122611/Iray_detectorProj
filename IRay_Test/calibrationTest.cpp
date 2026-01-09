// Dynamic_GenerateAllTemplates.cpp : Defines the entry point for the console application.
#include<calibration.h>
void TimeProc(int uTimerID)
{
	s_nExpWindow -= 1;
	if (0 == s_nExpWindow)
	{
		s_timer.Close();
		return;
	}
	TRACE("Please expose in %ds\r", s_nExpWindow);
}
void AcquiringDarkTimer(int uTimerID)
{
	int nValid = GetValidDarkFrames();
	gs_ProgressBar.SetProgress(nValid, gs_TotalDarkFrames);
	if (nValid == gs_TotalDarkFrames)
	{
		gs_timer.Close();
		TRACE("\n");
		SetEvent(gs_hNextStep);
	}
}
void AcquiringLightTimer(int uTimerID)
{
	int nValid = GetValidLightFrames();
	gs_ProgressBar.SetProgress(nValid, gs_TotalLightFrames);
	if (nValid == gs_TotalLightFrames)
	{
		gs_timer.Close();
		TRACE("\n");
		SetEvent(gs_hNextStep);
	}
}
void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam)
{
	gs_pDetInstance->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);
	switch (nEventID)
	{
		//连接信息
	case Evt_ConnectProcess:
		TRACE(pszMsg);
		TRACE("\n");
		break;
		//事件失败信息
	case Evt_TaskResult_Failed:
		if (nParam1 == Cmd_ForceDarkContinuousAcq)
		{
			TRACE("\nA error happened! error=%s\n", gs_pDetInstance->GetErrorInfo(nParam2).c_str());
			SetEvent(gs_hErrorEvent);
		}
		break;

	case Evt_Exp_Enable:
		TRACE("Prepare to expose\n");
		s_timer.Init(TimeProc, 1000);
		s_nExpWindow = nParam1 / 1000;
		TRACE("Please expose in %ds\r", s_nExpWindow);
		break;
		//获取图像的回调信息
	case Evt_Image:
		TRACE("\nGot image\n");
		{
			//must make deep copies of pParam
			IRayImage* pImg = (IRayImage*)pParam;
			unsigned short* pImageData = pImg->pData;
			int nImageSize = pImg->nWidth * pImg->nHeight * pImg->nBytesPerPixel;
			int nFrameNo = gs_pDetInstance->GetImagePropertyInt(&pImg->propList, Enm_ImageTag_FrameNo);
		}
		break;
	default:
		break;
	}

}
char DoSelection(Enm_FileTypes type)
{
	if (type != Enm_File_Gain && type != Enm_File_Defect)
		return 'q';
	TRACE("1. Press [Enter] to generate %s map or\n", type == Enm_File_Gain ? "gain" : "defect");
	TRACE("2. Press 'q' to exit\n");
	return tolower(getchar());
}
void connectAndCalibration() {
	do
	{
		if (Err_OK != Initializte())
		{
			break;
		}
		gs_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);//none zero
		gs_hEvents[0] = gs_hNextStep = CreateEvent(NULL, false, false, NULL);
		gs_hEvents[1] = gs_hErrorEvent = CreateEvent(NULL, false, false, NULL);
		int ret = InitCalibration();
		if (Err_OK != ret)
		{
			TRACE("InitCalibration failed!err=%s", gs_pDetInstance->GetErrorInfo(ret).c_str());
			break;
		}

		TRACE("Please make sure X-ray had been turned off and Press [Enter] to start collecting dark images");
		getchar();
		AcquireDarkImages();
		TRACE("    Generating offset map...");
		ret = GenerateOffsetTemplate();
		if (Err_OK != ret)
		{
			TRACE("Generate offset map failed! err=%s", gs_pDetInstance->GetErrorInfo(ret).c_str());
			break;
		}
		else
			TRACE("\t[Yes]\n");
		if ('q' == DoSelection(Enm_File_Gain))
			break;
		TRACE("Please make sure X-ray had been turned on and start collecting light images\n");
		AcquireLightImages();
		TRACE("    Generating gain map...");
		ret = GenerateGainTemplate();
		if (Err_OK != ret)
		{
			TRACE("Generate gain map failed! err=%s", gs_pDetInstance->GetErrorInfo(ret).c_str());
			break;
		}
		else
			TRACE("\t[Yes]\n");
		if ('q' == DoSelection(Enm_File_Defect))
			break;
		TRACE("    Generating defect map...");
		ret = GenerateDefectTemplate();
		if (Err_OK != ret)
		{
			TRACE("Generate defect map failed! err=%s", gs_pDetInstance->GetErrorInfo(ret).c_str());
		}
		else
			TRACE("\t[Yes]\n");
	} while (false);
	FinishCalibration();
	TRACE("Press [Enter] to exit");
	getchar();
	Deinit();
}
int Initializte()
{
	gs_pDetInstance = new CDetector();
	TRACE("Load libray");
	int ret = gs_pDetInstance->LoadIRayLibrary();
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ]\n");
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	TRACE("Create instance");
	ret = gs_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	TRACE("Connect device");
	ret = gs_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	TRACE("Set application-mode");
	ret = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode1", 5000);
	if (Err_OK != ret)
		TRACE("\t\t[No ]\n");
	else
		TRACE("\t\t[Yes]\n");
	return ret;
}
void Deinit()
{
	if (gs_hEvents[0])
	{
		CloseHandle(gs_hEvents[0]);
		gs_hEvents[0] = NULL;
	}
	if (gs_hEvents[1])
	{
		CloseHandle(gs_hEvents[1]);
		gs_hEvents[1] = NULL;
	}
	if (gs_pDetInstance)
	{
		gs_pDetInstance->Destroy();
		gs_pDetInstance->FreeIRayLibrary();
		delete gs_pDetInstance;
		gs_pDetInstance = NULL;
	}
}
int InitCalibration()
{
	int ret = gs_pDetInstance->SyncInvoke(Cmd_CalibrationInit, 5000);
	if (Err_OK == ret)
	{
		gs_TotalDarkFrames = gs_pDetInstance->GetAttrInt(Attr_OffsetTotalFrames);
		gs_TotalLightFrames = gs_pDetInstance->GetAttrInt(Attr_GainTotalFrames);
	}
	return ret;
}
int AcquireDarkImages()
{
	gs_pDetInstance->Invoke(Cmd_ForceDarkContinuousAcq, 0);
	gs_timer.Init(AcquiringDarkTimer, 100);
	int wait = WaitForMultipleObjects(2, gs_hEvents, false, WAIT_FOREVER);
	if (WAIT_OBJECT_0 + 1 == wait)
	{
		gs_timer.Close();
		return Err_Unknown;
	}
	return Err_OK;
}
int AcquireLightImages()
{
	gs_pDetInstance->Invoke(Cmd_StartAcq);
	gs_timer.Init(AcquiringLightTimer, 100);
	int wait = WaitForMultipleObjects(2, gs_hEvents, false, WAIT_FOREVER);
	if (WAIT_OBJECT_0 + 1 == wait)
	{
		gs_timer.Close();
		return Err_Unknown;
	}
	return Err_OK;
}
//Acquire dark images firstly
int GenerateOffsetTemplate()
{
	return gs_pDetInstance->Invoke(Cmd_OffsetGeneration);
}
//Acquire dark and light images firstly
int GenerateGainTemplate()
{
	return gs_pDetInstance->Invoke(Cmd_GainGeneration);
}
//Acquire dark and light images firstly
int GenerateDefectTemplate()
{
	return gs_pDetInstance->Invoke(Cmd_DefectGeneration);
}
int AbortCalibration()
{
	return  gs_pDetInstance->Abort();
}
void FinishCalibration()
{
	gs_pDetInstance->SyncInvoke(Cmd_FinishGenerationProcess, 3000);
}
int GetValidDarkFrames()
{
	return gs_pDetInstance->GetAttrInt(Attr_OffsetValidFrames);
}
int GetValidLightFrames()
{
	return gs_pDetInstance->GetAttrInt(Attr_GainValidFrames);
}