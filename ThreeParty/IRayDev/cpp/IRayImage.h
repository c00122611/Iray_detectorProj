/**
* File: IRayImage.h
*
* Purpose: Definit a struct for image data and information
*
*
* @author Haitao.Ning
* @version 1.1 2022/09/06
*
* Copyright (C) 2009, 2022, iRay Technology (Shanghai) Ltd.
*
*/
#ifndef _IRAY_IMAGE_H_
#define _IRAY_IMAGE_H_  

#include "IRayVariant.h"
#include "IRayEnumDef.h"

#pragma pack(push, 1)

typedef IRayVariantMap IRayImagePropertList;

typedef struct _tagIRayImage
{
	int nWidth;
	int nHeight;
	int nBytesPerPixel;
	unsigned short* pData;
	IRayImagePropertList propList;
}IRayImage;

#pragma pack(pop)

#endif