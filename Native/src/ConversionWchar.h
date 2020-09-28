
#ifndef __CONVERSIONWCHAR_H__
#define __CONVERSIONWCHAR_H__

#include "../include/types.h"

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace ADD_IN_NATIVE
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);

class WcharWrapper
{
public:

#if defined(__APPLE__) || defined(__ANDROID__)

    WcharWrapper(const WCHAR_T* str);
    
#endif //__APPLE__ || __ANDROID__

    WcharWrapper(const wchar_t* str);
    ~WcharWrapper();
#if defined(__APPLE__) || defined(__ANDROID__)

    operator const WCHAR_T*(){ return m_str_WCHAR; }
    operator WCHAR_T*(){ return m_str_WCHAR; }
    
#endif //__APPLE__ || __ANDROID__

    operator const wchar_t*(){ return m_str_wchar; }
    operator wchar_t*(){ return m_str_wchar; }
private:
    WcharWrapper& operator = (const WcharWrapper& other) { return *this; }
    WcharWrapper(const WcharWrapper& other) { }
private:

#if defined(__APPLE__) || defined(__ANDROID__)

    WCHAR_T* m_str_WCHAR;
    
#endif //__APPLE__ || __ANDROID__

    wchar_t* m_str_wchar;
};

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

#endif //__CONVERSIONWCHAR_H__
