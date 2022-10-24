/*
    ×Ö·û´®±àÂë×ª»»Æ÷¡£
*/

#include "ToyGraphCommon/EngineCompileOptions.h"

#include <Windows.h>

#include <string>

#include "ToyGraphCommon/CharEncodingConverter.h"

using namespace std;

#if TG_OS_IS_WINDOWS

string CharEncodingConverter::utf8ToGb(const string& utf8) {
    if (utf8.length() < 1) {
        return "";
    }

    const char* utf8c = utf8.c_str();

    auto len = MultiByteToWideChar(CP_UTF8, 0, utf8c, -1, nullptr, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8c, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);

    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, nullptr, nullptr);

    string result = str;

    delete[] wstr;
    delete[] str;

    return result;
}

#endif
