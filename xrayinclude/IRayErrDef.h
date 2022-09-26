/**
* File: IRayErrDef.h
*
* Purpose: IRay FPD error code definition
*
*
* @author Haitao.Ning
* @version 1.0 2015/4/23
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
#ifndef _IRAY_ERROR_DEF_H_
#define _IRAY_ERROR_DEF_H_

typedef int FPDRESULT;

#define Err_OK	0
#define Err_TaskPending	1
#define Err_Unknown	2
#define Err_DuplicatedCreation	3
#define Err_DetectorIdNotFound	4
#define Err_StateErr	5
#define Err_NotInitialized	6
#define Err_NotImplemented	7
#define Err_AccessDenied	8
#define Err_LoadDllFailed	9
#define Err_DllCreateObjFailed	10
#define Err_OpenFileFailed	11
#define Err_FileNotExist	12
#define Err_ConfigFileNotExist	13
#define Err_TemplateFileNotExist	14
#define Err_TemplateFileNotMatch	15
#define Err_InvalidFileFormat	16
#define Err_CreateLoggerFailed	17
#define Err_InvalidParamCount	18
#define Err_InvalidParamType	19
#define Err_InvalidParamValue	20
#define Err_PreCondition	21
#define Err_TaskTimeOut	22
#define Err_ProdInfoMismatch	23
#define Err_DetectorRespTimeout	24
#define Err_InvalidPacketNo	25
#define Err_InvalidPacketFormat	26
#define Err_PacketDataCheckFailed	27
#define Err_PacketLost_BufOverflow	28
#define Err_FrameLost_BufOverflow	29
#define Err_ImgChBreak	30
#define Err_BadImgQuality	31
#define Err_GeneralSocketErr	32
#define Err_DetectorSN_Mismatch	33
#define Err_CommDeviceNotFound	34
#define Err_CommDeviceOccupied	35
#define Err_CommParamNotMatch	36
#define Err_NotEnoughDiskSpace	37
#define Err_NotEnoughMemorySpace	38
#define Err_ApplyFirmwareFailed	39
#define Err_CallbackNotFinished	40
#define Err_FirmwareUpdated	41
#define Err_TooMuchDefectPoints	42
#define Err_TooLongFilePath	43
#define Err_ClientIPNotMatch	44
#define Err_AddressOccupied	45
#define Err_NotInSameNetworkSegment	46
#define Err_FPD_General_Detector_Error	1001
#define Err_FPD_General_ControlBox_Error	1002
#define Err_FPD_General_FirmwareUpgrade_Error	1003
#define Err_FPD_General_GSensor_Error	1004
#define Err_FPD_NotImplemented	1005
#define Err_FPD_SeqNoOutOfSync	1006
#define Err_FPD_Busy	1007
#define Err_FPD_Busy_Initializing	1018
#define Err_FPD_Busy_Last_Command_Suspending	1019
#define Err_FPD_Busy_Mode_Not_Supported	1020
#define Err_FPD_Busy_MCU_Busy	1021
#define Err_FPD_Busy_FPGA_Busy	1022
#define Err_FPD_Busy_FPGA_Timeout	1023
#define Err_FPD_Busy_Doing_Dynamic_Ghost	1024
#define Err_FPD_Busy_Doing_Dynamic_Preoffset	1025
#define Err_FPD_Busy_FTP_Image_Uploading	1026
#define Err_FPD_Busy_Capture_State_Recover	1027
#define Err_FPD_Busy_System_Error	1028
#define Err_FPD_Busy_BatteryLow	1029
#define Err_FPD_Occupied	1008
#define Err_FPD_SleepWakeupFailed	1009
#define Err_FPD_SleepCaptureError	1010
#define Err_FPD_CmdExecuteTimeout	1011
#define Err_FPD_FirmwareFallback	1012
#define Err_FPD_NotSupportInCurrMode	1013
#define Err_FPD_NoEnoughStorageSpace	1014
#define Err_FPD_FileNotExist	1015
#define Err_FPD_FtpServerAccessError	1016
#define Err_FPD_HWCaliFileError	1017
#define Err_FPD_AcquisitionBlock	1040
#define Err_FPD_SelfTestFailed	1041
#define Err_TemperatureHigh	1042
#define Err_Ftrans_Not_Ready	1043
#define Err_Ftrans_LocalFileOpenFailed	1044
#define Err_Ftrans_NetError	1045
#define Err_Ftrans_StartTimeout	1046
#define Err_Ftrans_StartError	1047
#define Err_Ftrans_FinishedTimeout	1048
#define Err_Ftrans_Aborted	1049
#define Err_Ftrans_DownloadTimeout	1050
#define Err_Ftrans_DownloadNotComplete	1051
#define Err_Ftrans_UploadTimeout	1052
#define Err_Ftrans_TcpBreak	1053
#define Err_Ftrans_FileLen_Zero	1054
#define Err_Ftrans_ParamError	1055
#define Err_Ftrans_FinishError	1056
#define Err_FPD_SelfTest_DiskReadOnly	1100
#define Err_FPD_SelfTest_SpaceNotEnough	1101
#define Err_FPD_SelfTest_CanNotGetFPDCfg	1102
#define Err_FPD_SelfTest_FPDCfgInvalid	1103
#define Err_FPD_SelfTest_TemplateFailed	1104
#define Err_FPD_SelfTest_SyncDftFailed	1105
#define Err_FPD_SelfTest_SyncGainFailed	1106
#define Err_FPD_SelfTest_Reserved	1199
#define Err_FTP_OpenLoginFailed	2001
#define Err_FTP_MkdirCdFailed	2002
#define Err_FTP_LocalFileOpenFailed	2003
#define Err_FTP_UploadFailed	2004
#define Err_FTP_DownloadFailed	2005
#define Err_FTP_FileVerifyFailed	2006
#define Err_FTP_TypeError	2007
#define Err_Cali_GeneralError	3001
#define Err_Cali_UnexpectImage_DoseHighHigh	3002
#define Err_Cali_UnexpectImage_ExpLineNotSatisfy	3003
#define Err_Cali_UnexpectImage_MistakeTrigger	3004
#define Err_Cali_DataNotReadyForGen	3005
#define Err_Cali_NotEnoughIntervalTime_OffsetTmpl	3006
#define Err_Cali_UnexpectImage_DoseOver	3007
#define Err_Cali_UnexpectImage_DoseUnder	3008
#define Err_Cali_UnexpectImage_ObjectDetected	3009
#define Err_Cali_UnexpectImage_PartialExposure	3010
#define Err_FPD_CertificationFailed	4000
#define Err_FPD_AccessDenied_Unauthorized	4001

#endif
