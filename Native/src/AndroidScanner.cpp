
#include "AndroidScanner.h"
#include "ConversionWchar.h"
#include "../jni/jnienv.h"
#include <jni.h>
#include <wchar.h>

std::wstring jstring2wstring(JNIEnv* jenv, jstring aStr)
{
	std::wstring result;

	if (aStr)
	{
		const jchar* pCh = jenv->GetStringChars(aStr, 0);
		jsize len = jenv->GetStringLength(aStr);
		const jchar* temp = pCh;
		while (len > 0)
		{
			result += *(temp++);
			--len;
		}
		jenv->ReleaseStringChars(aStr, pCh);
	}
	return result;
}

extern "C" JNIEXPORT void JNICALL Java_com_ptolkachev_AndroidScanner_Scanner_OnBarcodeReceived(
	JNIEnv * jenv, jclass aClass, jlong pObject, jstring aBarcode)
{
	CAndroidScanner* scanner = (CAndroidScanner*)pObject;
	if (scanner)
	{
		JNIEnv* jenv = getJniEnv();
		std::wstring barcode = jstring2wstring(jenv, aBarcode);
		scanner->SendReceivedBarcode(barcode);
	}
}

CAndroidScanner::CAndroidScanner() : cc(0), obj(0), m_pwstrActionName(0), m_pwstrExtraData(0), isOpen(false)
{
}

CAndroidScanner::~CAndroidScanner()
{
	if (isOpen)
		Close();

	if (m_pwstrActionName)
		delete m_pwstrActionName;
	if (m_pwstrExtraData)
		delete m_pwstrExtraData;

	JNIEnv* jenv = getJniEnv();
	jenv->DeleteGlobalRef(obj);
	jenv->DeleteGlobalRef(cc);
}

void CAndroidScanner::SetIConnect(IAddInDefBaseEx* piConnect)
{
	cnn = piConnect;

	if (cnn)
	{
		cnn->SetEventBufferDepth(10);
		helper = (IAndroidComponentHelper*)cnn->GetInterface(eIAndroidComponentHelper);
		if (helper)
		{
			WCHAR_T* className = 0;
			convToShortWchar(&className, L"com/ptolkachev/androidscanner/Scanner");
			jclass ccloc = helper->FindClass(className);
			delete className;
			className = 0;

			if (ccloc)
			{
				JNIEnv* jenv = getJniEnv();
				cc = static_cast<jclass>(jenv->NewGlobalRef(ccloc));
				jenv->DeleteLocalRef(ccloc);

				jobject activity = helper->GetActivity();
				jmethodID ctorID = jenv->GetMethodID(cc, "<init>", "(Landroid/app/Activity;J)V");
				jobject objloc = jenv->NewObject(cc, ctorID, activity, (jlong)this);
				if (objloc)
				{
					obj = jenv->NewGlobalRef(objloc);
					jenv->DeleteLocalRef(objloc);
				}
				jenv->DeleteLocalRef(activity);

				// Run native
				jmethodID methodID_show = jenv->GetMethodID(cc, "show", "()V");
				jenv->CallVoidMethod(obj, methodID_show);

				// Methods
				methodID_open = jenv->GetMethodID(cc, "open", "(Ljava/lang/String;Ljava/lang/String;)V");
				methodID_close = jenv->GetMethodID(cc, "close", "()V");
			}
		}
	}
}

void CAndroidScanner::SetIMemory(IMemoryManager* piMemory)
{
	mem = piMemory;
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
	{
		if (m_pwstrActionName)
		{
			delete m_pwstrActionName;
			m_pwstrActionName = 0;
		}

		convToShortWchar(&m_pwstrActionName, value);
	}
	else if (wcscmp(name, L"ExtraData") == 0)
	{
		if (m_pwstrExtraData)
		{
			delete m_pwstrExtraData;
			m_pwstrExtraData = 0;
		}

		convToShortWchar(&m_pwstrExtraData, value);
	}
	else
		return false;

	return true;
}

void CAndroidScanner::Open()
{
	JNIEnv* jenv = getJniEnv();

	jstring actionName = jenv->NewString(m_pwstrActionName, getLenShortWcharStr(m_pwstrActionName));
	jstring extraData = jenv->NewString(m_pwstrExtraData, getLenShortWcharStr(m_pwstrExtraData));

	jenv->CallVoidMethod(cc, methodID_open, actionName, extraData);

	isOpen = true;
}

void CAndroidScanner::Close()
{
	JNIEnv* jenv = getJniEnv();
	jenv->CallVoidMethod(cc, methodID_close);

	isOpen = false;
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

void CAndroidScanner::SendReceivedBarcode(std::wstring barcode)
{
	if (cnn && mem)
	{
		WCHAR_T* pwstrDeviceId = NULL;
		uint32_t iActualSize = static_cast<uint32_t>(wcslen(GetDeviceId()) + 1);
		if (!mem->AllocMemory((void**)&pwstrDeviceId, iActualSize * sizeof(WCHAR_T)))
			return;
		convToShortWchar(&pwstrDeviceId, GetDeviceId());

		wchar_t dataType[] = L"Штрихкод";
		WCHAR_T* pwstrDataType = NULL;
		iActualSize = static_cast<uint32_t>(wcslen(dataType) + 1);
		if (!mem->AllocMemory((void**)&pwstrDataType, iActualSize * sizeof(WCHAR_T)))
			return;
		convToShortWchar(&pwstrDataType, dataType);

		WCHAR_T* pwstrBarCode = NULL;
		iActualSize = static_cast<uint32_t>(barcode.size() + 1);
		if (!mem->AllocMemory((void**)&pwstrBarCode, iActualSize * sizeof(WCHAR_T)))
			return;
		convToShortWchar(&pwstrBarCode, barcode.c_str());

		cnn->ExternalEvent(pwstrDeviceId, pwstrDataType, pwstrBarCode);
	}
}