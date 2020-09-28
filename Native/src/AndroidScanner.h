
#ifndef __ANDROIDSCANNER_H__
#define __ANDROIDSCANNER_H__

#include "ConversionWchar.h"
#include "../include/mobile.h"

class IAddInDefBaseEx;
class IMemoryManager;

class CAndroidScanner
{
public:
	CAndroidScanner();
	~CAndroidScanner();

	wchar_t* GetDescription();

	uint32_t GetLastErrorCode();
	wchar_t* GetLastErrorDesc();

	wchar_t* GetParameters();
	bool SetParameter(wchar_t* name, wchar_t* value);

	void Open();
	void Close();

	wchar_t* GetDeviceId();
	wchar_t* GetDeviceDesc();
	bool IsDemoMode();

	bool DeviceTest();
	wchar_t* GetDeviceTestResult();

private:
	wchar_t m_ActionName[250];
	wchar_t m_ExtraData[250];

	wchar_t* m_TestResult = nullptr;
};

#endif