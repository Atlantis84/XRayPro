#include "Detector.h"
#include <string.h>

#define EXTRACT_FUNC(funcPtr, flag) {m_p##funcPtr = (funcPtr)ExtractFunc(m_hModule, #flag); if(NULL==m_p##funcPtr) return Err_LoadDllFailed;}

const int CDetector::OFFSETMASK = Enm_CorrectOp_SW_PreOffset | Enm_CorrectOp_SW_PostOffset | Enm_CorrectOp_HW_PreOffset | Enm_CorrectOp_HW_PostOffset;
const int CDetector::GAINMASK = Enm_CorrectOp_SW_Gain | Enm_CorrectOp_HW_Gain;
const int CDetector::DEFECTMASK = Enm_CorrectOp_SW_Defect | Enm_CorrectOp_HW_Defect;

#if defined(linux)
bool IRayTimer::m_stoptimerFlag;
#endif
timeproc IRayTimer::timercallback=NULL;
CDetector::CDetector()
: m_nDetectorID(0)
, m_bInitilized(false)
{
	m_WaitAckEvent = CreateEvent(NULL, false, false, NULL);
}

CDetector::~CDetector()
{
	Destroy();
	FreeIRayLibrary();
	if (m_WaitAckEvent)
	{
		CloseHandle(m_WaitAckEvent);
		m_WaitAckEvent = NULL;
	}
}

int CDetector::LoadIRayLibrary()
{
	m_hModule = OpenDLib("FpdSys");
	if (NULL == m_hModule)
	{
		return Err_LoadDllFailed;
	}
	EXTRACT_FUNC(FnCreate, Create);
	EXTRACT_FUNC(FnDestroy, Destroy);
	EXTRACT_FUNC(FnGetAttr, GetAttr);
	EXTRACT_FUNC(FnSetAttr, SetAttr);
	EXTRACT_FUNC(FnInvoke, Invoke);
	EXTRACT_FUNC(FnAbort, Abort);
	EXTRACT_FUNC(FnGetErrInfo, GetErrInfo);
	m_bInitilized = true;
	return Err_OK;
}

void CDetector::FreeIRayLibrary()
{
	if (m_hModule)
	{
		CloseDLib(m_hModule);
		m_hModule = NULL;
	}
}

FPDRESULT CDetector::Create(const char* pszWorkDir, FnCallback fpCallback)
{
	if (!m_bInitilized)
	{
		FPDRESULT nRet = LoadIRayLibrary();
		if (Err_OK != nRet)
		{
			return nRet;
		}
	}

	FPDRESULT nRet = m_pFnCreate(pszWorkDir, fpCallback, &m_nDetectorID);
	if (Err_OK != nRet)
	{
		return nRet;
	}

	return nRet;
}


FPDRESULT CDetector::Destroy()
{
	if (m_nDetectorID > 0)
	{
		m_pFnDestroy(m_nDetectorID);
		m_nDetectorID = 0;
	}
	return Err_OK;
}

FPDRESULT CDetector::Abort()
{
	return m_pFnAbort(m_nDetectorID);
}


FPDRESULT CDetector::SetAttr(int nAttrID, int nValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	IRayVariant var;
	var.vt = IVT_INT;
	var.val.nVal = nValue;
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}

FPDRESULT CDetector::SetAttr(int nAttrID, float fValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	IRayVariant var;
	var.vt = IVT_FLT;
	var.val.fVal = fValue;
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}
FPDRESULT CDetector::SetAttr(int nAttrID, const char* strValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	if (!strValue)
		return Err_InvalidParamValue;

	IRayVariant var;
	var.vt = IVT_STR;
	strncpy(var.val.strVal, strValue, IRAY_MAX_STR_LEN-1);
	FPDRESULT result = m_pFnSetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != result)
	{
	}
	return result;
}

void CDetector::GetAttr(int nAttrID, AttrResult& result)
{
	IRayVariant var;
	FPDRESULT ret = m_pFnGetAttr(m_nDetectorID, nAttrID, &var);
	if (Err_OK != ret)
	{
		memset(&result, 0, sizeof(result));
		return;
	}

	if (IVT_INT == var.vt)
	{
		result.nVal = var.val.nVal;
	}
	else if (IVT_FLT == var.vt)
	{
		result.fVal = var.val.fVal;
	}
	else if (IVT_STR == var.vt)
	{
		memcpy(result.strVal, var.val.strVal, IRAY_MAX_STR_LEN);
	}
}
int CDetector::GetAttrInt(int nAttrID)
{
    AttrResult ret;
    GetAttr(nAttrID, ret);
    return ret.nVal;
}

string CDetector::GetErrorInfo(int nErrorCode)
{
	ErrorInfo info;
	m_pFnGetErrInfo(nErrorCode, &info);
	return info.szDescription;
}


FPDRESULT CDetector::Invoke(int cmdId)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = cmdId;
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, NULL, 0);
	if (Err_OK != result && Err_TaskPending != result)
	{
	}
	return result;
}

FPDRESULT CDetector::Invoke(int cmdId, int nValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = cmdId;
	IRayCmdParam param;
	param.pt = IPT_VARIANT;
	param.var.vt = IVT_INT;
	param.var.val.nVal = nValue;
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, &param, 1);
	if (Err_OK != result && Err_TaskPending != result)
	{
	}
	return result;
}

FPDRESULT CDetector::Invoke(int cmdId, int nPar1, int nPar2)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = cmdId;
	IRayCmdParam param[2];
	param[0].pt = IPT_VARIANT;
	param[0].var.vt = IVT_INT;
	param[0].var.val.nVal = nPar1;
	param[1].pt = IPT_VARIANT;
	param[1].var.vt = IVT_INT;
	param[1].var.val.nVal = nPar2;
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, param, 2);
	if (Err_OK != result && Err_TaskPending != result)
	{
	}
	return result;
}

FPDRESULT CDetector::Invoke(int cmdId, float fValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = cmdId;
	IRayCmdParam param;
	param.pt = IPT_VARIANT;
	param.var.vt = IVT_FLT;
	param.var.val.fVal = fValue;
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, &param, 1);
	if (Err_OK != result && Err_TaskPending != result)
	{
	}
	return result;
}

FPDRESULT CDetector::Invoke(int cmdId, const char* strValue)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	if (!strValue)
		return Err_InvalidParamValue;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = cmdId;
	IRayCmdParam param;
	param.pt = IPT_VARIANT;
	param.var.vt = IVT_STR;
	strncpy(param.var.val.strVal, strValue, IRAY_MAX_STR_LEN-1);
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, &param, 1);
	if (Err_OK != result && Err_TaskPending != result)
	{
		//printf("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
	}
	return result;
}

FPDRESULT CDetector::Invoke(int nCmdId, int nPara1, int nPara2, const char* strPara1)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = nCmdId;
	IRayCmdParam param[3];
	param[0].pt = IPT_VARIANT;
	param[0].var.vt = IVT_INT;
	param[0].var.val.nVal = nPara1;

	param[1].pt = IPT_VARIANT;
	param[1].var.vt = IVT_INT;
	param[1].var.val.nVal = nPara2;

	param[2].pt = IPT_VARIANT;
	param[2].var.vt = IVT_STR;
	strncpy(param[2].var.val.strVal, strPara1,IRAY_MAX_STR_LEN - 1);

	FPDRESULT result = m_pFnInvoke(m_nDetectorID, nCmdId, param, 3);
	if (Err_OK != result && Err_TaskPending != result)
	{
		//print("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
	}
	return result;
}


FPDRESULT CDetector::Invoke(int nCmdId, int nPara1, int nPara2, const char* strPara1, const char* strPara2)
{
	if (!m_bInitilized)
		return Err_NotInitialized;

	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = nCmdId;
	IRayCmdParam param[4];
	param[0].pt = IPT_VARIANT;
	param[0].var.vt = IVT_INT;
	param[0].var.val.nVal = nPara1;

	param[1].pt = IPT_VARIANT;
	param[1].var.vt = IVT_INT;
	param[1].var.val.nVal = nPara2;

	param[2].pt = IPT_VARIANT;
	param[2].var.vt = IVT_STR;
	strncpy(param[2].var.val.strVal, strPara1, IRAY_MAX_STR_LEN-1);

	param[3].pt = IPT_VARIANT;
	param[3].var.vt = IVT_STR;
	strncpy(param[3].var.val.strVal, strPara2, IRAY_MAX_STR_LEN-1);

	FPDRESULT result = m_pFnInvoke(m_nDetectorID, nCmdId, param, 4);
	if (Err_OK != result && Err_TaskPending != result)
	{
		//print("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
	}
	return result;
}

FPDRESULT CDetector::Invoke(int cmdId, int nPar1, int nPar2, int nPar3)
{
    if (!m_bInitilized)
        return Err_NotInitialized;

    ResetEvent(m_WaitAckEvent);
    m_CurCmdId = cmdId;
    IRayCmdParam param[3];
    param[0].pt = IPT_VARIANT;
    param[0].var.vt = IVT_INT;
    param[0].var.val.nVal = nPar1;
    param[1].pt = IPT_VARIANT;
    param[1].var.vt = IVT_INT;
    param[1].var.val.nVal = nPar2;
    param[2].pt = IPT_VARIANT;
    param[2].var.vt = IVT_INT;
    param[2].var.val.nVal = nPar3;
    FPDRESULT result = m_pFnInvoke(m_nDetectorID, cmdId, param, 3);
    if (Err_OK != result && Err_TaskPending != result)
    {
    }
    return result;
}

//编程指南有说明，total 512 bytes
FPDRESULT CDetector::WriteCustomerROM(int nCmdId, void* pBlockData, unsigned size)
{
	if (!m_bInitilized)
		return Err_NotInitialized;
	if (size > 512)
		return Err_InvalidParamCount;
	ResetEvent(m_WaitAckEvent);
	m_CurCmdId = nCmdId;
	char mask[512] = { 0 };
	memset(mask, 1, size);
	IRayCmdParam param[2];
	param[0].pt = IPT_BLOCK;
	param[0].blc.pData = pBlockData;
	param[0].blc.uBytes = 512;
	param[1].pt = IPT_BLOCK;
	param[1].blc.pData = mask;
	param[1].blc.uBytes = 512;
	FPDRESULT result = m_pFnInvoke(m_nDetectorID, nCmdId, param, 2);
	if (Err_OK != result && Err_TaskPending != result)
	{
		//print("Invoke  failed! Err = %s", GetErrorInfo(result).c_str());
	}
	return result;
}

int CDetector::WaitEvent(int timeout)
{
	int wait = WaitForSingleObject(m_WaitAckEvent, timeout);
	if (WAIT_TIMEOUT == wait)
		return Err_TaskTimeOut;
	else
		return m_nLastError;
}

FPDRESULT CDetector::SyncInvoke(int cmdId, int timeout)
{
	m_nLastError = Err_TaskTimeOut;
	int result = Invoke(cmdId);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}

FPDRESULT CDetector::SyncInvoke(int cmdId, int nValue, int timeout)
{
	m_nLastError = Err_TaskTimeOut;
	int result = Invoke(cmdId, nValue);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}
FPDRESULT CDetector::SyncInvoke(int cmdId, int nPar1, int nPar2, int timeout)
{
	m_nLastError = Err_TaskTimeOut;
	int result = Invoke(cmdId, nPar1, nPar2);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}
FPDRESULT CDetector::SyncInvoke(int cmdId, float fValue, int timeout)
{
	m_nLastError = Err_TaskTimeOut;
	int result = Invoke(cmdId, fValue);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}
FPDRESULT CDetector::SyncInvoke(int cmdId, const char* strValue, int timeout)
{
	m_nLastError = Err_TaskTimeOut;
	int result = Invoke(cmdId, strValue);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}

FPDRESULT CDetector::SyncInvoke(int cmdId, int nPar1, int nPar2, int nPar3, int timeout)
{
    m_nLastError = Err_TaskTimeOut;
    int result = Invoke(cmdId, nPar1, nPar2, nPar3);
    if (Err_TaskPending == result)
    {
        result = WaitEvent(timeout);
    }
    return result;
}

FPDRESULT CDetector::SyncInvoke(int nCmdId, int nPara1, int nPara2, const char* strPara1, int timeout)
{
	int result = Invoke(nCmdId, nPara1,nPara2,strPara1);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}

FPDRESULT CDetector::SyncInvoke(int nCmdId, int nPara1, int nPara2, const char* strPara1, const char* strPara2, int timeout)
{
	int result = Invoke(nCmdId, nPara1, nPara2, strPara1,strPara2);
	if (Err_TaskPending == result)
	{
		result = WaitEvent(timeout);
	}
	return result;
}

void CDetector::SDKCallback(int nDetectorID, int nEventID, int nEventLevel,
	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam)
{
	if ((Evt_TaskResult_Succeed == nEventID) || (Evt_TaskResult_Failed == nEventID))
	{
		if (Evt_TaskResult_Succeed == nEventID)
			m_nLastError = Err_OK;
		else
			m_nLastError = nParam2;
		if (m_CurCmdId == nParam1)
		{
			SetEvent(m_WaitAckEvent);
		}
	}
}

int CDetector::GetImagePropertyInt(IRayVariantMap* pProperties, int nTagId)
{
    if (!pProperties) 
        return -1;

    for (int nItemIndex=0; nItemIndex < pProperties->nItemCount; nItemIndex++)
    {
        if (nTagId == pProperties->pItems[nItemIndex].nMapKey)
        {
            return pProperties->pItems[nItemIndex].varMapVal.vt == IVT_INT ? pProperties->pItems[nItemIndex].varMapVal.val.nVal : -1;
        }
    }
    return -1;
}

std::string GetWorkDirPath()
{
	char buff[128] = {0};
	FILE* pFile = NULL;
	std::string workdir("");
	pFile = fopen("..\\common\\workdir_path.txt", "r");
	if (pFile)
	{
		fgets(buff, 128, pFile);
		fclose(pFile);
		workdir = std::string(buff);
	}
	else
	{
		pFile = fopen("workdir_path.txt", "r");
		if (pFile)
		{
			fgets(buff, 128, pFile);
			fclose(pFile);
			workdir = std::string(buff);
		}
	}
	return workdir.erase(workdir.find_last_not_of("\n") + 1);
}