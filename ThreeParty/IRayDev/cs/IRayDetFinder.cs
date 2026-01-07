/**
* File: IRayDetFinder.cs
*
* Purpose: IRay detector finder definition
*
*
* @author Wei.You
* @version 1.0 2021/3/2
*
* Copyright (C) 2009, 2021, iRay Technology (Shanghai) Ltd.
*
*/
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace iDetector
{
    public enum Enm_CommChannel
    {
        Enm_CommChannel_Ethernet = 1,
        Enm_CommChannel_USB = 2,
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ChannelScanInput_Ethernet
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string strIP;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ChannelScanResult_Ethernet
    {
        public int nProdNo;
        public int nSubProdNo;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string strSN;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string strIP;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 120)]
        public string strReserved;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ChannelScanResult_USB
    {
        public int nProdNo;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string strUSBDeviceID;

        public byte IsValidID;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 219)]
        public string reserved;
    };

    public partial class SdkInterface
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ScanNotifyResultExHandler(Enm_CommChannel eCommChannel, IntPtr pScanResult);


        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ScanOnceEx(Enm_CommChannel eCommChannel, IntPtr pScanInput);

        [DllImport("FpdSys.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterScanNotifyEx(ScanNotifyResultExHandler fpCallback);
    }
}