
#include "AddInNative.h"
#include "ConversionWchar.h"
#include <stdio.h>

#if defined(__ANDROID__) || defined(__APPLE__)

#include <wchar.h>

#endif //__ANDROID__ || __APPLE__

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace ADD_IN_NATIVE
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY
    
static const wchar_t g_kClassNames[] = L"AddInNative"; //|OtherClass1|OtherClass2";
static const wchar_t g_ComponentNameType[] = L"AddInNativeExtension";
static WcharWrapper s_kClassNames(g_kClassNames);

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

static const char g_ComponentName[] = "AddInNative";

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

// This component supports 1.0 version
const long g_VersionAddIn = 1000;

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
AddInNative::AddInNative()
{
}
//---------------------------------------------------------------------------//
AddInNative::~AddInNative()
{
}
//---------------------------------------------------------------------------//
bool AddInNative::Init(void* pConnection)
{
    return false;
}
//---------------------------------------------------------------------------//
long AddInNative::GetInfo()
{ 
    return g_VersionAddIn;
}
//---------------------------------------------------------------------------//
void AddInNative::Done()
{
}
//---------------------------------------------------------------------------//
bool AddInNative::RegisterExtensionAs(WCHAR_T** wsLanguageExt)
{ 
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
    return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddInNative::GetPropName(long lPropNum, long lPropAlias)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
bool AddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool AddInNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool AddInNative::IsPropReadable(const long lPropNum)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool AddInNative::IsPropWritable(const long lPropNum)
{
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
    return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddInNative::GetMethodName(const long lMethodNum, 
                            const long lMethodAlias)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
long AddInNative::GetNParams(const long lMethodNum)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
bool AddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    return false;
} 
//---------------------------------------------------------------------------//
bool AddInNative::HasRetVal(const long lMethodNum)
{ 
    return false;
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
    return false; 
}
//---------------------------------------------------------------------------//
void AddInNative::SetLocale(const WCHAR_T* loc)
{
}
//---------------------------------------------------------------------------//
bool AddInNative::setMemManager(void* mem)
{
    return false;
}
//---------------------------------------------------------------------------//
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

};

namespace ADD_IN_NATIVE
{
    
    static LPCVOID addin_exports[] =
    {
        "GetClassObject", (LPCVOID)GetClassObject,
        "DestroyObject", (LPCVOID)DestroyObject,
        "GetClassNames", (LPCVOID)GetClassNames,
        "SetPlatformCapabilities", (LPCVOID)SetPlatformCapabilities,
        NULL
    };

    DECLARE_DLL((const char*)g_ComponentName, addin_exports);
}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY
