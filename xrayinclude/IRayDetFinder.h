/**
* File: IRayDetFinder.h
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
#ifndef _IRAY_DETFINDER_H_
#define _IRAY_DETFINDER_H_

enum Enm_CommChannel
{
    Enm_CommChannel_Ethernet = 1,
    Enm_CommChannel_USB = 2,
};

#pragma pack(push, 1)

typedef struct _tagChannelScanInput_Ethernet
{
    char szIP[64];
}ChannelScanInput_Ethernet;

typedef struct _tagChannelScanResult_Ethernet
{
    int nProdNo;
    int nSubProdNo;
    char szSN[64];
    char szIP[64];
    char reserved[120];
}ChannelScanResult_Ethernet;

typedef struct _tagChannelScanResult_USB
{
    int nProdNo;
    char szUSBDeviceID[32];
    char reserved[220];
}ChannelScanResult_USB;

#pragma pack(pop)

/**
* FnNotifyScanResultEx: Define a function type for register a scanning callback function
* 
* @param eCommChannel [out] communication channel
* @param pScanResult [out] scan result data struct, it could be ChannelScanResult_Ethernet or
* ChannelScanResult_USB according to eCommChannel
*
* @return void
*/
typedef void(*FnNotifyScanResultEx)(Enm_CommChannel eCommChannel, void* pScanResult);

/**
* FnRegisterScanNotifyEx: Define a function type for DLL export function "FnNotifyScanResultEx"
*
* RegisterScanNotifyEx: register the scanning result callback function, 
* it will return multiple times if there are multiple devices is available
*
* @param pCallback [in] callback function pointer, return the scan result from operation "ScanOnceEx" or
*                       firmware pushed online notification.
*
* @return 0: succeed, Non-Zero: error code
*/
typedef int(*FnRegisterScanNotifyEx)(FnNotifyScanResultEx pCallback);

/**
* FnScanOnceEx: Define a function type for start a scan to find reachable detectors,
*  the result will be returned through the registered callback function. 
*
* @param eCommChannel [in] communication channel
* @param pScanInput [in] scan result data struct, it could be ChannelScanInput_Ethernet or
* null according to eCommChannel
*
* @return void
*/
typedef int(*FnScanOnceEx)(Enm_CommChannel eCommChannel, void* pScanInput);


#endif