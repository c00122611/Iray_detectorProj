#pragma once
#include "stdafx.h"
#include "Detector.h"
#include "DisplayProgressbar.h"
#include <ctype.h>
#pragma comment(lib, "winmm.lib")

static DisplayProgressbar gs_ProgressBar;
static CDetector* gs_pDetInstance = NULL;
static int gs_TotalDarkFrames;
static int gs_TotalLightFrames;
static int gs_StartFrames;
static IRayTimer gs_timer;
static HEVENT gs_hNextStep = NULL;
static HEVENT gs_hErrorEvent = NULL;
static HEVENT gs_hEvents[2];

int Initializte();
void Deinit();
int InitCalibration();
int AcquireDarkImages();
int AcquireLightImages();
int GenerateOffsetTemplate();
int GenerateGainTemplate();
int GenerateDefectTemplate();
int GetValidDarkFrames();
int GetValidLightFrames();
int AbortCalibration();
void FinishCalibration();
//用于测试 校正+连接
void connectAndCalibration();