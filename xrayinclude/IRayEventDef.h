/**
* File: IRayEventDef.h
*
* Purpose: IRay FPD callback event definition
*
*
* @author Haitao.Ning
* @version 1.0 2015/4/23
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
#ifndef _IRAY_EVENT_DEF_H_
#define _IRAY_EVENT_DEF_H_

#define Evt_GeneralInfo	1 // General Info
#define Evt_GeneralWarn	2 // General Warn
#define Evt_GeneralError	3 // General Error
#define Evt_TaskResult_Succeed	4 // Task succeed
#define Evt_TaskResult_Failed	5 // Task failed
#define Evt_TaskResult_Canceled	6 // Task been canceled
#define Evt_AutoTask_Started	7 // Task is started which is not emitted by user command, outer or AED image acquiring will trigger this event.
#define Evt_ScanOnce_Request	8 // Request scanner to start a broadcast scanning
#define Evt_ImageFileUpload_Result	9 // Image file upload result notify
#define Evt_TemplateFileUpload_Result	10 // Template file upload result notify
#define Evt_TemplateFileDownload_Result	11 // Template file download result notify
#define Evt_HwCaliTemplateList	12 // Return hardware calibration template list
#define Evt_HwWiFiScanList	13 // Return hardware WiFi scan list
#define Evt_HistoricalImageList	14 // Return hardware historical image list
#define Evt_TimeDiff	15 // Return time difference between detector and PC in seconds
#define Evt_LivingTime	16 // Return living time from detector in seconds
#define Evt_TransactionAborted	17 // Notify that current transaction was aborted
#define Evt_HwAcquisitionSettingList	18 // Return hardware acquisition setting list
#define Evt_Image	1001 // Image received
#define Evt_Prev_Image	1002 // Preview image received
#define Evt_Retransfer_Image	1012 // Retransfered image received after break and re-connecting
#define Evt_WaitImage_Timeout	1003 // Acqusition operation time out(equal: Possiable Image Loss)
#define Evt_Exp_Prohibit	1004 // Exposure prohibit
#define Evt_Exp_Enable	1005 // Exposure enable
#define Evt_Exp_Timeout	1006 // Exposure timeout
#define Evt_OffsetAmassingTime	1008 // Pixel base amassing time,typically from previous clear operation to current acquisition
#define Evt_MistakenTrigger	1009 // Mistaken trigger in FreeSync mode
#define Evt_Image_Trans_Slow	1010 // Warn that image transfer speed low, usually at wireless connection.
#define Evt_Image_Abandon	1011 // Warn that currently acquiring image abandoned because bad network situation
#define Evt_LastImageID	1013 // Return the image ID while invoking Cmd_QueryLastImageID
#define Evt_CustomROM	1014 // Return the customized data while invoking Cmd_ReadCustomROM
#define Evt_LicenseData	1015 // Return the license data while invoking Cmd_ReadLicenseData
#define Evt_NextStitchingPositionEnable	1016 // Inform the system that the acquisition of post-offset dark image is finished
#define Evt_StitchingFlowTimeout	1017 // If two stitching exposures time out, the detector will exit the stitching workflow and back to normal workflow
#define Evt_FreshImageIDs	1018 // Return remain images that haven't been transferred. If all the images have been transferred, then nFirstImageID and nFreshImageCount will be "0"
#define Evt_CustomData	1019 // Return the customized data while invoking Cmd_ReadCustomData
#define Evt_AutoPoweroff	1020 // Power off warn
#define Evt_ConnectProcess	2001 // Report Process info while connecting
#define Evt_CommFailure	2002 // Communication failed
#define Evt_TemperatureHigh	2003 // Temperature too high
#define Evt_FpsOutOfRange	2004 // Fps too high or too low
#define Evt_LowBattery	2005 // Low battery warn
#define Evt_TemplateOverDue	2006 // Calibration template file over due
#define Evt_SWHWDataNotMatch	2007 // The correct data of software and hardware not match, for example:Software defect template create time not same as Software defect template create time
#define Evt_DefectPointsIncreased	2008 // Notify user after new defect map generated if new defect points are found.
#define Evt_CtrlBoxOnlineState	2009 // Notify user current controlbox connection state changed
#define Evt_IdcRegionConflict	2010 // Notify user Region selection conflict
#define Evt_IdcCorrectionFactorParams	2011 // Return IDC correction factor
#define Evt_IdcPresetGrayFactorParams	2012 // Return IDC preset gray
#define Evt_IdcPresetDoseFactorParams	2013 // Return IDC preset dose
#define Evt_IdcSensitivityFactorParams	2014 // Return IDC SPL factor
#define Evt_IdcCutOffThresholdParams	2015 // Return IDC cut-off threshold parameters
#define Evt_IdcCtrlBoxRomParams	2016 // Return IDC control box rom parameters
#define Evt_FwMismatch	2030 // Notify user current firmware version mismatch with main version
#define Evt_FpdSelfTestFailed	2031 // Notify user current panel connected, but it working in error state.

#endif
