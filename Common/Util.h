#ifndef _UTIL_H
#define _UTIL_H

typedef void(*timeproc)(int signo);
#include <string>
#include <string.h>

#ifdef WIN32
static std::string _wtoa(const std::wstring& w)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return "";
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return "";
	WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

static std::wstring _atow(const std::string& a)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, a.c_str(), (int)a.size(), 0, 0);
	if (nSize <= 0) return L"";
	wchar_t *pwszDst = new wchar_t[nSize + 1];
	if (NULL == pwszDst) return L"";
	MultiByteToWideChar(CP_ACP, 0, a.c_str(), (int)a.size(), pwszDst, nSize);
	pwszDst[nSize] = 0;
	if (pwszDst[0] == 0xFEFF) // skip Oxfeff
	for (int i = 0; i < nSize; i++)
	{
		pwszDst[i] = pwszDst[i + 1];
	}
	std::wstring wcharString(pwszDst);
	delete[]pwszDst;
	return wcharString;
}

class IRayTimer
{
public:
	static void CALLBACK MMTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
	{
		if (timercallback)
		{
			timercallback(uTimerID);
		}
	}
	void Init(timeproc timerhandler, int millseconds)
	{
		timercallback = timerhandler;
		m_timeID = timeSetEvent(millseconds, 1, MMTimeProc, 0, TIME_PERIODIC);
	}
	void Close()
	{
		timeKillEvent(m_timeID);
	}
private:
	unsigned  m_timeID;
	static timeproc timercallback;
};

#else
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
static std::string _wtoa(const std::wstring& w)
{
	int nSize = wcstombs(NULL, w.c_str(), 0);
	char* pszDst = new char[nSize+1];
	if (NULL == pszDst) return "";
	wcstombs(pszDst, w.c_str(), nSize);
	pszDst[nSize] = 0;
	std::string mbStr(pszDst);
	delete[] pszDst;
	return mbStr;
}

static std::wstring _atow(const std::string& a)
{
	int nSize = mbstowcs(NULL, a.c_str(), 0);
	wchar_t *pwszDst = new wchar_t[nSize + 1];
	if (NULL == pwszDst) return L"";
	mbstowcs(pwszDst, a.c_str(), nSize);
	pwszDst[nSize] = 0;
	std::wstring wcStr(pwszDst);
	delete[]pwszDst;
	return wcStr;
}

class IRayTimer
{
public:
	static void sigroutine(int signo)
	{
		if (timercallback)
		{
			switch (signo)
			{
			case SIGALRM:
				if (!m_stoptimerFlag)
				{
					//signal(SIGALRM, sigroutine);
					timercallback(signo);
				}
				break;
			default:
				break;
			}
		}
	}
	void Init(timeproc timerhandler, int millseconds)
	{
		struct itimerval value;
		timercallback = timerhandler;
		m_stoptimerFlag = false;
		signal(SIGALRM, sigroutine);
		value.it_value.tv_sec = millseconds / 1000;
		value.it_value.tv_usec = (millseconds % 1000) * 1000;
		value.it_interval.tv_sec = millseconds / 1000;
		value.it_interval.tv_usec =  (millseconds % 1000) * 1000;
		setitimer(ITIMER_REAL, &value, NULL);
	}
	void Close()
	{
		m_stoptimerFlag = true;
		struct itimerval stoptimer;
		memset(&stoptimer, 0, sizeof(stoptimer));
		setitimer(ITIMER_REAL, &stoptimer, NULL);
	}
	static bool m_stoptimerFlag;
	static timeproc timercallback;
};

#endif

#endif