using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace iDetector
{
    public partial class SdkInterface
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void SdkCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
            IntPtr pszMsg, int nParam1, int nParam2, int nPtrParamLen, IntPtr pParam);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ScanCallbackHandler(IntPtr pProfile); 

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetSDKVersion(StringBuilder szVersion); // size = 32
   
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetUserCode([MarshalAs(UnmanagedType.LPStr)]string strUserCode); 

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterScanNotify(ScanCallbackHandler fpCallback);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ScanOnce([MarshalAs(UnmanagedType.LPStr)]string strLocalIP);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Create([MarshalAs(UnmanagedType.LPStr)]string strWorkDir, SdkCallbackHandler fpCallback, ref int pDetectorID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Destroy(int nDetectorID);
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReleaseLibraryResource();
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAttr(int nDetectorID, int nAttrID, ref IRayVariant pVar);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetAttr(int nDetectorID, int nAttrID, ref IRayVariant pVar);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Invoke(int nDetectorID, int nCommandID, IRayCmdParam[] pars, int nParCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Abort(int nDetectorID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int OpenDefectTemplateFile([MarshalAs(UnmanagedType.LPStr)]string strFilePath, 
            ref IntPtr ppHandler, 
            ref ushort usWidth, 
            ref ushort usHeight,
            ref IntPtr ppPoints,       
            ref IntPtr ppRows,         
            ref IntPtr ppCols,         
            ref IntPtr ppDualReadCols2 
	        );

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int CalcDefectWithImageData(
            IntPtr pData,
            ushort usWidth,
            ushort usHeight,
            ushort usDummyTop,
            ushort usDummyBottom,
            ushort usDummyLeft,
            ushort usDummyRight,
            IntPtr pPoints,
            IntPtr pRows,
            IntPtr pCols
            );

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TestCollimatorWithImageData(
            IntPtr pData,
            ushort usWidth,
            ushort usHeight,
            ushort usDummyTop,
            ushort usDummyBottom,
            ushort usDummyLeft,
            ushort usDummyRight,
            ref bool bResult
            );

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SaveDefectTemplateFile(IntPtr pHandler);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int CloseDefectTemplateFile(IntPtr pHandler);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int DoExpLineCorrect(IntPtr pHandler, ushort usWidth, ushort usHeight, ushort usExpLine);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxCreate(int nComPort, ref int pSyncboxID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxDestroy(int nSyncboxID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxBind(int nSyncboxID, int nDetectorID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxGetBind(int nSyncboxID, ref int pDetectorID);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxGetTubeReadyTime(int nSyncboxID, ref int pTimeInMS);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxSetTubeReadyTime(int nSyncboxID, int nTimeInMS);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SyncBoxGetState(int nSyncboxID, ref int pState);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int UseImageBuf(int nDetectorID, UInt64 ullBufSizeInBytes);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ClearImageBuf(int nDetectorID);
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int QueryImageBuf(
            int nDetectorID,
            out int pnFrameNum, 
            out int pnImageHeight, 
            out int pnImageWidth, 
            out int pnBytesPerPixel, 
            out int pnPropListMemSize);
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetImageFromBuf(
            int nDetectorID, 
            IntPtr pImage, 
            int nImageDataSize, 
            IntPtr pProperties, 
            int nPropListMemSize);

        public static int SetAttr(int nDetectorID, int nAttrID, int nValue)
        {
            IRayVariant var = new IRayVariant();
            var.vt = IRAY_VAR_TYPE.IVT_INT;
            var.val.nVal = nValue;
            return SetAttr(nDetectorID, nAttrID, ref var);
        }
        public static int SetAttr(int nDetectorID, int nAttrID, float fValue)
        {
            IRayVariant var = new IRayVariant();
            var.vt = IRAY_VAR_TYPE.IVT_FLT;
            var.val.fVal = fValue;
            return SetAttr(nDetectorID, nAttrID, ref var);
        }
        public static int SetAttr(int nDetectorID, int nAttrID, string strValue)
        {
            IRayVariant var = new IRayVariant();
            var.vt = IRAY_VAR_TYPE.IVT_STR;
            var.val.strVal = strValue;
            return SetAttr(nDetectorID, nAttrID, ref var);
        }
        public static int GetAttrInt(int nDetectorID, int nAttrID)
        {
            IRayVariant var = new IRayVariant();
            SdkInterface.GetAttr(nDetectorID, nAttrID, ref var);
            if (var.vt != IRAY_VAR_TYPE.IVT_INT)
                throw new Exception("Attr type error");
            return var.val.nVal;
        }
    }

    public class SdkParamConvertor<T>
    {
        public static bool IntPtrToStructArray(IntPtr ptr, ref T[] arr)
        {
            if (ptr == IntPtr.Zero || arr == null)
            {
                return false;
            }

            IntPtr pos;
            int nStructSize = Marshal.SizeOf(typeof(T));
            for (int i = 0; i < arr.Length; i++)
            {
                if (IntPtr.Size == sizeof(UInt32))
                {
                    pos = (IntPtr)((UInt32)ptr + i * nStructSize);
                }
                else if (IntPtr.Size == sizeof(UInt64))
                {
                    pos = (IntPtr)((UInt64)ptr + (UInt64)(i * nStructSize));
                }
                else 
                {
                    return false;
                }
                arr[i] = (T)Marshal.PtrToStructure(pos, typeof(T));
            }
            return true;
        }

        public static bool StructArrayToIntPtr(T[] arr, ref IntPtr ptr)
        {
            if (ptr == IntPtr.Zero || arr == null)
            {
                return false;
            }

            IntPtr pos;
            int nStructSize = Marshal.SizeOf(typeof(T));
            for (int i = 0; i < arr.Length; i++)
            {
                if (IntPtr.Size == sizeof(UInt32))
                {
                    pos = (IntPtr)((UInt32)ptr + i * nStructSize);
                }
                else if (IntPtr.Size == sizeof(UInt64))
                {
                    pos = (IntPtr)((UInt64)ptr + (UInt64)(i * nStructSize));
                }
                else
                {
                    return false;
                }
                Marshal.StructureToPtr(arr[i], pos, false);
            }

            return true;
        }

        public static byte[] StructToBytes(T[] structObj)
        {
            int items = structObj.Length;
            if (0 == items)
                return null;
            int size = Marshal.SizeOf(structObj[0]);
            byte[] buffer = new byte[size * items];
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            for(int index = 0; index < items; ++index)
            {
                Marshal.StructureToPtr(structObj[index], structPtr, false);
                Marshal.Copy(structPtr, buffer, size * index, size);
            }
            Marshal.FreeHGlobal(structPtr);
            return buffer;
        }

        public static T[] BytesToStruct(byte[] data)
        {
            int size = Marshal.SizeOf(typeof(T));
            int items = data.Length / size;
            if (0 == items)
                return null;
            T[] structArray = new T[items];
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            for(int index = 0; index < items; ++index)
            {
                Marshal.Copy(data, index * size, structPtr, size);
                structArray[index] = (T)Marshal.PtrToStructure(structPtr, typeof(T));
            }
            Marshal.FreeHGlobal(structPtr);
            return structArray;
        }
    }
    public static class EnumConvert<T>
    {
        /// <summary> Parse string to enum type
        /// </summary>
        /// <param name="s"> Input string </param>
        /// <returns> Parsing result enum object </returns>
        public static T ParseFromString(string s) { return (T)Enum.Parse(typeof(T), s); }
    }
}
