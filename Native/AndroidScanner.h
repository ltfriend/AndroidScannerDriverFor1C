
#ifndef __ANDROIDSCANNER_H__
#define __ANDROIDSCANNER_H__

#include "ConversionWchar.h"
#include "include/AddInDefBase.h"
#include "include/IAndroidComponentHelper.h"
#include "include/IMemoryManager.h"
#include "include/mobile.h"
#include <string>

class CAndroidScanner
{
public:
	CAndroidScanner();
	~CAndroidScanner();

	void SetIConnect(IAddInDefBaseEx* piConnect);
	void SetIMemory(IMemoryManager* piMemory);

	WCHAR_T* GetDescription();

	uint32_t GetLastErrorCode();
	WCHAR_T* GetLastErrorDesc();

	WCHAR_T* GetParameters();
	bool SetParameter(wchar_t* name, wchar_t* value);

	void Open();
	void Close();

	WCHAR_T* GetDeviceId();
	WCHAR_T* GetDeviceDesc();
	bool IsDemoMode();

	void SendReceivedBarcode(std::wstring barcode);

private:
	jclass cc;
	jobject obj;
	IAddInDefBaseEx* cnn;
	IMemoryManager* mem;

	WCHAR_T* m_pwstrScanMode;
	WCHAR_T* m_pwstrActionName;
	WCHAR_T* m_pwstrExtraData;
	WCHAR_T* m_pwstrDescription;
	WCHAR_T* m_pwstrLastErrorDesc;
	WCHAR_T* m_pwstrParameters;
	WCHAR_T* m_pwstrDeviceId;
	WCHAR_T* m_pwstrDeviceDesc;

	bool isOpen;

	IAndroidComponentHelper* helper;
	jmethodID methodID_open;
	jmethodID methodID_close;
};

#endif