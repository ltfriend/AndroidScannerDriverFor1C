
#include "AndroidScanner.h"
#include "ConversionWchar.h"
#include "jni/jnienv.h"
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

extern "C" JNIEXPORT void JNICALL Java_com_ptolkachev_androidscanner_Scanner_OnBarcodeReceived(
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

CAndroidScanner::CAndroidScanner() :
	cc(0),
	obj(0),
	cnn(0),
	mem(0),
	m_pwstrActionName(0),
	m_pwstrExtraData(0),
	m_pwstrLastErrorDesc(0),
	isOpen(false)
{
	wchar_t description[] =
		L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><DriverDescription Name=\"Сканеры штрихкода Android\" Description=\"Сканеры штрихкода Android\" EquipmentType=\"СканерШтрихкода\" IntegrationComponent=\"false\" MainDriverInstalled=\"false\" DriverVersion=\"1.0.1.1\" IntegrationComponentVersion=\"1.0\" DownloadURL=\"\" LogIsEnabled=\"false\" LogPath = \"\"/>";
	m_pwstrDescription = 0;
	convToShortWchar(&m_pwstrDescription, description);

	wchar_t parameters[] =
		L"<Settings><Group Caption=\"Параметры подключения\"><Parameter Name=\"ActionName\" Caption=\"Action Name\" TypeValue=\"String\" DefaultValue=\"\"/><Parameter Name=\"ExtraData\" Caption=\"Extra Data\" TypeValue=\"String\" DefaultValue=\"\"/></Group></Settings>";
	m_pwstrParameters = 0;
	convToShortWchar(&m_pwstrParameters, parameters);

	wchar_t deviceId[] = L"com_ptolkachev_AndroidScanner";
	m_pwstrDeviceId = 0;
	convToShortWchar(&m_pwstrDeviceId, deviceId);

	wchar_t deviceDesc[] = L"Android Scanner";
	m_pwstrDeviceDesc = 0;
	convToShortWchar(&m_pwstrDeviceDesc, deviceDesc);
}

CAndroidScanner::~CAndroidScanner()
{
	if (isOpen)
		Close();

	if (m_pwstrActionName)
		delete m_pwstrActionName;
	if (m_pwstrExtraData)
		delete m_pwstrExtraData;

	delete m_pwstrDescription;
	if (m_pwstrLastErrorDesc)
		delete m_pwstrLastErrorDesc;
	delete m_pwstrParameters;
	delete m_pwstrDeviceId;
	delete m_pwstrDeviceDesc;

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
				methodID_open = jenv->GetMethodID(cc, "start", "(Ljava/lang/String;Ljava/lang/String;)V");
				methodID_close = jenv->GetMethodID(cc, "stop", "()V");
			}
		}
	}
}

void CAndroidScanner::SetIMemory(IMemoryManager* piMemory)
{
	mem = piMemory;
}

WCHAR_T* CAndroidScanner::GetDescription()
{
	return m_pwstrDescription;
}

uint32_t CAndroidScanner::GetLastErrorCode()
{
	return 0L;
}

WCHAR_T* CAndroidScanner::GetLastErrorDesc()
{
	return m_pwstrLastErrorDesc;
}

WCHAR_T* CAndroidScanner::GetParameters()
{
	return m_pwstrParameters;
}

bool CAndroidScanner::SetParameter(wchar_t* name, wchar_t* value)
{
	if (!name)
		return false;

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
	{
		return false;
	}

	return true;
}

void CAndroidScanner::Open()
{
	//TODO: Проверять параметры на NULL.
	JNIEnv* jenv = getJniEnv();

	jstring actionName = jenv->NewString(m_pwstrActionName, getLenShortWcharStr(m_pwstrActionName));
	jstring extraData = jenv->NewString(m_pwstrExtraData, getLenShortWcharStr(m_pwstrExtraData));

	jenv->CallVoidMethod(obj, methodID_open, actionName, extraData);

	isOpen = true;
}

void CAndroidScanner::Close()
{
	JNIEnv* jenv = getJniEnv();
	jenv->CallVoidMethod(obj, methodID_close);

	isOpen = false;
}

WCHAR_T* CAndroidScanner::GetDeviceId()
{
	return m_pwstrDeviceId;
}

WCHAR_T* CAndroidScanner::GetDeviceDesc()
{
	return m_pwstrDeviceDesc;
}

bool CAndroidScanner::IsDemoMode()
{
	return false;
}

void CAndroidScanner::SendReceivedBarcode(std::wstring barcode)
{
	if (cnn && mem)
	{
		WCHAR_T* pwstrDeviceId = NULL;
		uint32_t iActualSize = static_cast<uint32_t>(getLenShortWcharStr(m_pwstrDeviceId) + 1);
		if (!mem->AllocMemory((void**)&pwstrDeviceId, iActualSize * sizeof(WCHAR_T)))
			return;
		memcpy(pwstrDeviceId, m_pwstrDeviceId, iActualSize * sizeof(WCHAR_T));

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