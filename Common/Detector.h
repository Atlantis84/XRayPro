#ifndef _DETECTOR_H
#define _DETECTOR_H

#ifdef WIN32
#include <Windows.h>
typedef HMODULE LIBMODULE;
#define ExtractFunc			GetProcAddress
#define OpenDLib(libname)	LoadLibrary(L##libname)
#define CloseDLib			FreeLibrary
#define Sleep				Sleep
#else
#include <dlfcn.h>
#include <unistd.h>
typedef void* LIBMODULE;
#define ExtractFunc 		dlsym
#define OpenDLib(libname)	dlopen("lib" libname".so", RTLD_LAZY)
#define CloseDLib			dlclose
#define L                   ""
#define Sleep(time)			usleep(time*1000)
#endif

#include "IRayFpdSys.h"
#include "IRayFpdSysEx.h"
#include "Util.h"
#include "winevent.h"
#include <vector>
#include <string>
#include <stdio.h>
using namespace::std;

#define TRACE printf

union AttrResult
{
	int   nVal;
	float fVal;
	char  strVal[IRAY_MAX_STR_LEN];
};

class CDetectorManager;
class iEventReceiver;
class CDetector
{
public:
	CDetector();
	~CDetector();
	int  LoadIRayLibrary();
	void FreeIRayLibrary();

	FPDRESULT Create(const char* pszWorkDir, FnCallback fpCallback);
	FPDRESULT Destroy();
	FPDRESULT SetAttr(int nAttrID, int nValue);
	FPDRESULT SetAttr(int nAttrID, float fValue);
	FPDRESULT SetAttr(int nAttrID, const char* strValue);
	void      GetAttr(int nAttrID, AttrResult& result);
    int       GetAttrInt(int nAttrID);
	FPDRESULT Invoke(int cmdId);
	FPDRESULT Invoke(int cmdId, int nValue);
	FPDRESULT Invoke(int cmdId, int nPar1, int nPar2);
    FPDRESULT Invoke(int cmdId, int nPar1, int nPar2, int nPar3);
	FPDRESULT Invoke(int cmdId, float fValue);
	FPDRESULT Invoke(int cmdId, const char* strValue);
	FPDRESULT Invoke(int nCmdId, int nPara1, int nPara2, const char* strPara1);
	FPDRESULT Invoke(int nCmdId, int nPara1, int nPara2, const char* strPara1, const char* strPara2);
	FPDRESULT SyncInvoke(int cmdId, int timeout);
	FPDRESULT SyncInvoke(int cmdId, int nValue, int timeout);
	FPDRESULT SyncInvoke(int cmdId, int nPar1, int nPar2, int timeout);
    FPDRESULT SyncInvoke(int cmdId, int nPar1, int nPar2, int nPar3, int timeout);
	FPDRESULT SyncInvoke(int cmdId, float fValue, int timeout);
    FPDRESULT SyncInvoke(int cmdId, const char* strValue, int timeout);
	FPDRESULT SyncInvoke(int nCmdId, int nPara1, int nPara2, const char* strPara, int timeout);
	FPDRESULT SyncInvoke(int nCmdId, int nPara1, int nPara2, const char* strPara1, const char* strPara2, int timeout);
	FPDRESULT WriteCustomerROM(int nCmdId, void* pBlockData, unsigned size);
	FPDRESULT Abort();
	string    GetErrorInfo(int nErrorCode);
	int       DetectorID(){ return m_nDetectorID; }
	void      SDKCallback(int nDetectorID, int nEventID, int nEventLevel,
						  const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);
	int WaitEvent(int timeout);

    int GetImagePropertyInt(IRayVariantMap* pProperties, int nTagId);

	static const int OFFSETMASK;
	static const int GAINMASK;
	static const int DEFECTMASK;
private:
	bool m_bInitilized;
	LIBMODULE m_hModule;
	FnCreate  m_pFnCreate;
	FnDestroy m_pFnDestroy;
	FnGetAttr m_pFnGetAttr;
	FnSetAttr m_pFnSetAttr;
	FnInvoke  m_pFnInvoke;
	FnAbort   m_pFnAbort;
	FnGetErrInfo m_pFnGetErrInfo;
	int m_nDetectorID;
	HEVENT m_WaitAckEvent;
	int m_CurCmdId;
	int m_nLastError;
};

std::string GetWorkDirPath();

#endif