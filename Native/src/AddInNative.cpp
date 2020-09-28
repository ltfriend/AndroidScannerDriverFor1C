
#include "AddInNative.h"
#include "ConversionWchar.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t* g_PropNames[] =
{
    L"DriverDescription",
    L"ErrorDescription",
    L"TableParameters",
    L"Name",
    L"Value",
    L"DeviceID",
    L"Description",
    L"DemoModeIsActivated",
    L"TableActions",
    L"ActionName"
};

static const wchar_t* g_MethodNames[] =
{
    L"GetInterfaceRevision",
    L"GetDescription",
    L"GetLastError",
    L"GetParameters",
    L"SetParameter",
    L"Open",
    L"Close",
    L"DeviceTest",
    L"GetAdditionalActions",
    L"DoAdditionalAction"
};

static const wchar_t* g_PropNamesRu[] =
{
    L"ОписаниеДрайвера",
    L"ОписаниеОшибки",
    L"ТаблицаПараметров",
    L"Имя",
    L"Значение",
    L"ИДУстройства",
    L"Описание",
    L"АктивированДемоРежим",
    L"ТаблицаДействий",
    L"ИмяДействия"
};

static const wchar_t* g_MethodNamesRu[] =
{
    L"ПолучитьРевизиюИнтерфейса",
    L"ПолучитьОписание",
    L"ПолучитьОшибку",
    L"ПолучитьПараметры",
    L"УстановитьПараметр",
    L"Подключить",
    L"Отключить",
    L"ТестУстройства",
    L"ПолучитьДополнительныеДействия",
    L"ВыполнитьДополнительноеДействие"
};

static const wchar_t g_ComponentNameType[] = L"com_ptolkachev_AndroidScannerExtension";
static const wchar_t g_kClassNames[] = L"AndroidScanner";
static WcharWrapper s_kClassNames(g_kClassNames);

// This component supports 2.1 version
const long g_VersionAddIn = 2100;

const long g_InterfaceRev = 3003; // Version 3.3

static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface = new AddInNative();
        return (long)*pInterface;
    }
    return 0;
}
    
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}
    
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pInterface)
{
    if(!*pInterface)
        return -1;

    delete *pInterface;
    *pInterface = 0;
    return 0;
}
    
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    return s_kClassNames;
}
    
//---------------------------------------------------------------------------//
// AddInNative
AddInNative::AddInNative() : m_iConnect(0), m_iMemory(0)
{
}
//---------------------------------------------------------------------------//
AddInNative::~AddInNative()
{
}
//---------------------------------------------------------------------------//
bool AddInNative::Init(void* pConnection)
{
    m_iConnect = (IAddInDefBaseEx*)pConnection;
    return m_iConnect != nullptr;
}
//---------------------------------------------------------------------------//
long AddInNative::GetInfo()
{ 
    return g_VersionAddIn;
}
//---------------------------------------------------------------------------//
void AddInNative::Done()
{
    m_iConnect = nullptr;
    m_iMemory = nullptr;
}
//---------------------------------------------------------------------------//
bool AddInNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
    const wchar_t* wsExtension = g_ComponentNameType;
    uint32_t iActualSize = static_cast<uint32_t>(::wcslen(wsExtension) + 1);

    if (m_iMemory)
    {
        if (m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
        {
            convToShortWchar(wsExtensionName, wsExtension, iActualSize);
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------//
long AddInNative::GetNProps()
{ 
    return eLastProp;
}
//---------------------------------------------------------------------------//
long AddInNative::FindProp(const WCHAR_T* wsPropName)
{ 
    long plPropNum = -1;
    wchar_t* propName = 0;
    convFromShortWchar(&propName, wsPropName);

    plPropNum = findName(g_PropNames, propName, eLastProp);

    if (plPropNum == -1)
        plPropNum = findName(g_PropNamesRu, propName, eLastProp);

    delete[] propName;

    return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddInNative::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= eLastProp)
        return NULL;

    wchar_t* wsCurrentName = NULL;
    WCHAR_T* wsPropName = NULL;

    switch (lPropAlias)
    {
    case 0: // First language (english)
        wsCurrentName = (wchar_t*)g_PropNames[lPropNum];
        break;
    case 1: // Second language (local)
        wsCurrentName = (wchar_t*)g_PropNamesRu[lPropNum];
        break;
    default:
        return 0;
    }

    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);

    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
    }

    return wsPropName;
}
//---------------------------------------------------------------------------//
bool AddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    switch (lPropNum)
    {
    case ePropDriverDescription:
    {
        wchar_t* description = m_Scanner.GetDescription();
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(description) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, description, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    case ePropErrorDescription:
    {
        wchar_t* description = m_Scanner.GetLastErrorDesc();
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(description) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, description, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    case ePropTableParameters:
    {
        wchar_t* parameters = m_Scanner.GetParameters();
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(parameters) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, parameters, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    case ePropDeviceID:
    {
        wchar_t* deviceId = m_Scanner.GetDeviceId();
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(deviceId) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, deviceId, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    case ePropDescription:
    {
        wchar_t* deviceTestResult = m_Scanner.GetDeviceTestResult();
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(deviceTestResult) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, deviceTestResult, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    case ePropDemoModeIsActivated:
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = m_Scanner.IsDemoMode();
        break;
    case ePropTableActions:
    {
        wchar_t* tableActions = L"";
        uint32_t iActualSize = static_cast<uint32_t>(wcslen(tableActions) + 1);
        if (m_iMemory)
        {
            if (m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T)))
            {
                convToShortWchar(&pvarPropVal->pwstrVal, tableActions, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
        }
        break;
    }
    default:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------//
bool AddInNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{ 
    switch (lPropNum)
    {
    case ePropName:
        if (varPropVal->vt == VTYPE_PWSTR)
        {
            wchar_t* paramName = m_ParamName;
            convFromShortWchar(&paramName, varPropVal->pwstrVal);
        }
        break;
    case ePropValue:
        if (varPropVal->vt == VTYPE_PWSTR)
        {
            wchar_t* paramValue = m_ParamValue;
            convFromShortWchar(&paramValue, varPropVal->pwstrVal);
        }
        break;
    case ePropDeviceID:
        if (varPropVal->vt == VTYPE_PWSTR)
        {
            wchar_t* deviceId = m_DeviceId;
            convFromShortWchar(&deviceId, varPropVal->pwstrVal);
        }
    case ePropActionName:
        break;
    default:
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------//
bool AddInNative::IsPropReadable(const long lPropNum)
{ 
    return true;
}
//---------------------------------------------------------------------------//
bool AddInNative::IsPropWritable(const long lPropNum)
{
    switch (lPropNum)
    {
    case ePropDriverDescription:
    case ePropErrorDescription:
    case ePropTableParameters:
    case ePropDeviceID:
    case ePropDescription:
    case ePropDemoModeIsActivated:
    case ePropTableActions:
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------//
long AddInNative::GetNMethods()
{ 
    return eLastMethod;
}
//---------------------------------------------------------------------------//
long AddInNative::FindMethod(const WCHAR_T* wsMethodName)
{ 
    long plMethodNum = -1;
    wchar_t* name = 0;
    convFromShortWchar(&name, wsMethodName);

    plMethodNum = findName(g_MethodNames, name, eLastMethod);

    if (plMethodNum == -1)
        plMethodNum = findName(g_MethodNamesRu, name, eLastMethod);

    delete[] name;

    return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddInNative::GetMethodName(const long lMethodNum, 
                            const long lMethodAlias)
{ 
    if (lMethodNum >= eLastMethod)
        return NULL;

    wchar_t* wsCurrentName = NULL;
    WCHAR_T* wsMethodName = NULL;

    switch (lMethodAlias)
    {
    case 0: // First language (english)
        wsCurrentName = (wchar_t*)g_MethodNames[lMethodNum];
        break;
    case 1: // Second language (local)
        wsCurrentName = (wchar_t*)g_MethodNamesRu[lMethodNum];
        break;
    default:
        return 0;
    }

    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);

    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
    }

    return wsMethodName;
}
//---------------------------------------------------------------------------//
long AddInNative::GetNParams(const long lMethodNum)
{ 
    switch (lMethodNum)
    {
    case eMethGetDescription:
    case eMethGetLastError:
    case eMethGetParameters:
    case eMethOpen:
    case eMethClose:
    case eMethGetAdditionalActions:
    case eMethDoAdditionalAction:
        return 1;
    case eMethSetParameter:
    case eMethDeviceTest:
        return 2;
    default:
        return 0;
    }
}
//---------------------------------------------------------------------------//
bool AddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    TV_VT(pvarParamDefValue) = VTYPE_EMPTY;
    return false;
} 
//---------------------------------------------------------------------------//
bool AddInNative::HasRetVal(const long lMethodNum)
{ 
    switch (lMethodNum)
    {
    case eMethGetInterfaceRevision:
    case eMethGetDescription:
    case eMethGetLastError:
    case eMethGetParameters:
    case eMethSetParameter:
    case eMethOpen:
    case eMethClose:
    case eMethDeviceTest:
    case eMethGetAdditionalActions:
    case eMethDoAdditionalAction:
        return true;
    default:
        return false;
    }
}
//---------------------------------------------------------------------------//
bool AddInNative::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool AddInNative::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{ 
    switch (lMethodNum)
    {
    case eMethGetInterfaceRevision:
        TV_VT(pvarRetValue) = VTYPE_I4;
        TV_I4(pvarRetValue) = static_cast<int32_t>(g_InterfaceRev);
        return true;
    case eMethGetDescription:
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    case eMethGetLastError:
        TV_VT(pvarRetValue) = VTYPE_I4;
        TV_I4(pvarRetValue) = m_Scanner.GetLastErrorCode();
        return true;
    case eMethGetParameters:
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    case eMethSetParameter:
        m_Scanner.SetParameter(m_ParamName, m_ParamValue);
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    case eMethOpen:
        m_Scanner.Open();
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    case eMethClose:
        m_Scanner.Close();
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    case eMethDeviceTest:
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = m_Scanner.DeviceTest();
        return true;
    case eMethGetAdditionalActions:
    case eMethDoAdditionalAction:
        TV_VT(pvarRetValue) = VTYPE_BOOL;
        TV_BOOL(pvarRetValue) = true;
        return true;
    default:
        break;
    }
    return false; 
}
//---------------------------------------------------------------------------//
void AddInNative::SetLocale(const WCHAR_T* loc)
{
}
//---------------------------------------------------------------------------//
bool AddInNative::setMemManager(void* mem)
{
    m_iMemory = (IMemoryManager*)mem;
    return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
void AddInNative::addError(uint32_t wcode, const wchar_t* source,
    const wchar_t* descriptor, long code)
{
    if (m_iConnect)
    {
        WCHAR_T* err = 0;
        WCHAR_T* descr = 0;

        convToShortWchar(&err, source);
        convToShortWchar(&descr, descriptor);

        m_iConnect->AddError(wcode, err, descr, code);

        delete[] descr;
        delete[] err;
    }
}
//---------------------------------------------------------------------------//
long AddInNative::findName(const wchar_t* names[], const wchar_t* name,
    const uint32_t size) const
{
    long ret = -1;
    for (uint32_t i = 0; i < size; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}
