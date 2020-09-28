
#include "AndroidScanner.h"
#include "ConversionWchar.h"
#include "../include/AddInDefBase.h"
#include <wchar.h>

CAndroidScanner::CAndroidScanner()
{
}

CAndroidScanner::~CAndroidScanner()
{
}

wchar_t* CAndroidScanner::GetDescription()
{
	return L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><DriverDescription Name=\"Сканеры штрихкода Android\" Description=\"Сканеры штрихкода Android\" EquipmentType=\"СканерШтрихкода\" IntegrationComponent=\"false\" MainDriverInstalled=\"false\" DriverVersion=\"1.0\" IntegrationComponentVersion=\"1.0\" DownloadURL=\"\" LogIsEnabled=\"false\" LogPath = \"\"/>";
}

uint32_t CAndroidScanner::GetLastErrorCode()
{
	return 0L;
}

wchar_t* CAndroidScanner::GetLastErrorDesc()
{
	return L"";
}

wchar_t* CAndroidScanner::GetParameters()
{
	return L"<Settings><Group Caption=\"Параметры подключения\"><Parameter Name=\"ActionName\" Caption=\"Action Name\" TypeValue=\"String\" DefaultValue=\"\"/><Parameter Name=\"ExtraData\" Caption=\"Extra Data\" TypeValue=\"String\" DefaultValue=\"\"/></Group></Settings>";
}

bool CAndroidScanner::SetParameter(wchar_t* name, wchar_t* value)
{
	if (wcscmp(name, L"ActionName") == 0)
		wcscpy(m_ActionName, value);
	else if (wcscmp(name, L"ExtraData") == 0)
		wcscpy(m_ExtraData, value);
	else
		return false;

	return true;
}

void CAndroidScanner::Open()
{
	//TODO: —оздать прослушку сканировани¤.
}

void CAndroidScanner::Close()
{
	//TODO: ќтключить прослушку сканировани¤.
}

wchar_t* CAndroidScanner::GetDeviceId()
{
	return L"com_ptolkachev_androidscanner";
}

wchar_t* CAndroidScanner::GetDeviceDesc()
{
	return L"Android Scanner";
}

bool CAndroidScanner::IsDemoMode()
{
	return false;
}

bool CAndroidScanner::DeviceTest()
{
	m_TestResult = GetDeviceDesc();
	return true;
}

wchar_t* CAndroidScanner::GetDeviceTestResult()
{
	return m_TestResult;
}
