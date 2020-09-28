
#ifndef __ANDROIDSCANNER_H__
#define __ANDROIDSCANNER_H__

#include "ConversionWchar.h"
#include "../include/AddInDefBase.h"
#include "../include/IAndroidComponentHelper.h"
#include "../include/IMemoryManager.h"
#include "../include/mobile.h"
#include <string>

class CAndroidScanner
{
public:
	CAndroidScanner();
	~CAndroidScanner();

	void SetIConnect(IAddInDefBaseEx* piConnect);
	void SetIMemory(IMemoryManager* piMemory);

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

	void SendReceivedBarcode(std::wstring barcode);

private:
	jclass cc;
	jobject obj;
	IAddInDefBaseEx* cnn;
	IMemoryManager* mem;

	WCHAR_T* m_pwstrActionName;
	WCHAR_T* m_pwstrExtraData;

	wchar_t* m_TestResult = nullptr;
	bool isOpen;

	IAndroidComponentHelper* helper;
	jmethodID methodID_open;
	jmethodID methodID_close;
};

#endif