using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace iDetector
{
    public partial class SdkInterface
    {
        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAuthority(ref int nAuthority);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAttrsCount(int nDetectorID, ref int pCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAttrIDList(int nDetectorID, int[] pIDList, int nCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetAttrInfo(int nDetectorID, int nAttrID, ref AttrInfo pInfo);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetCommandCount(int nDetectorID, ref int pCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetCommandInfoList(int nDetectorID, IntPtr pCmdInfoList, int nCount);// CmdInfo[] pList

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetCmdParamCount(int nDetectorID, int nCmdID, ref int pCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetCmdParamInfo(int nDetectorID, int nCmdID, IntPtr pCmdParamInfoList, int nCount); // CmdParamInfo[] pList

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetProdCount(ref int pCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetProdList(IntPtr pProdList, int nCount); // ProdInfo[] pProdList

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetErrInfo(int nErrorCode, ref ErrorInfo pInfo);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetEnumItemsCount([MarshalAs(UnmanagedType.LPStr)]string strEnumTypeName, ref int pCount);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetEnumItemList([MarshalAs(UnmanagedType.LPStr)]string strEnumTypeName, IntPtr pEnumItemList, int nCount); // EnumItem[] pList

    }

}
