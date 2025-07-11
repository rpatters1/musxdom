/*
 * Copyright (C) 2025, Robert Patterson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <string>
#include <cstring>
#include <ctime>

#ifdef MUSX_RUNNING_ON_WINDOWS
#include <windows.h>
#else
#include <locale>
#include <sstream>
#include <iomanip>
#include <langinfo.h>
#endif

namespace musx {
namespace util {

/**
 * @struct DateTime
 * @brief Static class to provide utility functions for formatting date and time.
 */
struct DateTime
{

    /**
     * @enum DateFormatStyle
     * @brief Defines the date format styles supported by formatDate.
     */
    enum class DateFormatStyle : int
    {
        Short = 0,              ///< Short date format (e.g., 7/10/25)
        Long = 1,               ///< Long date format (e.g., July 10, 2025)
        LongAbbreviated = 2     ///< Long abbreviated date format (e.g., Jul 10, 2025)
    };

    /**
     * @brief Creates a std::time_t from integer year, month, and day.
     *
     * @param year Full year (e.g., 2025).
     * @param month Month of year (1-12).
     * @param day Day of month (1-31).
     * @return std::time_t representing the local time at midnight of that date.
     */
    inline static std::time_t makeTimeT(int year, int month, int day)
    {
        std::tm tm = {};
        tm.tm_year = year - 1900; // tm_year is years since 1900
        tm.tm_mon = month - 1;    // tm_mon is 0-based
        tm.tm_mday = day;
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        tm.tm_isdst = -1; // Let system determine daylight saving time

        return std::mktime(&tm);
    }

#ifdef MUSX_RUNNING_ON_WINDOWS
    /**
     * @brief Formats a date according to the current locale on POSIX systems.
     *
     * Uses strftime to produce a locale-specific date string.
     *
     * @param t The time value to format.
     * @param style The desired date format style.
     * @return A formatted date string, or an empty string on failure.
     */
    inline static std::string formatDate(std::time_t t, DateFormatStyle style)
    {
        SYSTEMTIME st;
        {
            struct tm tm;
            localtime_s(&tm, &t);
            st.wYear = WORD(tm.tm_year + 1900);
            st.wMonth = WORD(tm.tm_mon + 1);
            st.wDay = WORD(tm.tm_mday);
            st.wHour = WORD(tm.tm_hour);
            st.wMinute = WORD(tm.tm_min);
            st.wSecond = WORD(tm.tm_sec);
            st.wMilliseconds = 0;
        }

        // Determine flags and custom format string
        DWORD flags = 0;
        const wchar_t* customFormat = nullptr;

        switch (style)
        {
        case DateFormatStyle::Short:
            flags = DATE_SHORTDATE;
            break;

        case DateFormatStyle::Long:
            flags = DATE_LONGDATE;
            break;

        case DateFormatStyle::LongAbbreviated:
            // Windows does not provide a built-in long abbreviated date, so use custom format string
            customFormat = L"MMM d, yyyy";
            break;
        }

        // First call to get required size
        int requiredSize = GetDateFormatEx(
            nullptr, flags, &st, customFormat, nullptr, 0, nullptr);

        if (requiredSize == 0) {
            throw std::system_error(GetLastError(), std::system_category());
        }

        // Allocate WCHAR buffer with exact required size
        std::wstring wbuf(requiredSize, L'\0');

        // Second call to actually format
        int result = GetDateFormatEx(
            nullptr, flags, &st, customFormat, wbuf.data(), requiredSize, nullptr);

        if (result == 0) {
            throw std::system_error(GetLastError(), std::system_category());
        }

        // Convert WCHAR (UTF-16) to UTF-8
        int utf8len = WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string output(utf8len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, &output[0], utf8len, nullptr, nullptr);
        output.resize(utf8len - 1); // remove null terminator

        return output;
    }
#endif // MUSX_RUNNING_ON_WINDOWS

#ifndef MUSX_RUNNING_ON_WINDOWS
    /**
     * @brief Formats a date according to the current locale on POSIX systems.
     *
     * Uses strftime to produce a locale-specific date string.
     *
     * @param t The time value to format.
     * @param style The desired date format style.
     * @return A formatted date string, or an empty string on failure.
     */
    inline static std::string formatDate(std::time_t t, DateFormatStyle style)
    {
        struct tm tm;

        localtime_r(&t, &tm);

        const char* fmt = nullptr;
        switch (style) {
        case DateFormatStyle::Short:
            fmt = "%x";
            break;
        case DateFormatStyle::Long:
            fmt = "%B %d, %Y";
            break;
        case DateFormatStyle::LongAbbreviated:
            fmt = "%b %d, %Y";
            break;
        }

        std::ostringstream oss;
        oss.imbue(std::locale("")); // Use user’s current locale

        oss << std::put_time(&tm, fmt);
        return oss.str();
    }
#endif // !MUSX_RUNNING_ON_WINDOWS
    
    /**
     * @brief Formats a time according to the current locale.
     *
     * If includeSeconds is true, uses locale-based full time formatting.
     * If includeSeconds is false, formats as hh:mm, choosing 12h or 24h based on system or locale preference.
     *
     * @param t The time value to format.
     * @param includeSeconds If true, includes seconds in the formatted time.
     * @return A formatted time string.
     */
    inline static std::string formatTime(std::time_t t, bool includeSeconds)
    {
#ifdef MUSX_RUNNING_ON_WINDOWS
        SYSTEMTIME st;
        {
            struct tm tm;
            localtime_s(&tm, &t);
            st.wYear = WORD(tm.tm_year + 1900);
            st.wMonth = WORD(tm.tm_mon + 1);
            st.wDay = WORD(tm.tm_mday);
            st.wHour = WORD(tm.tm_hour);
            st.wMinute = WORD(tm.tm_min);
            st.wSecond = WORD(tm.tm_sec);
            st.wMilliseconds = 0;
        }

        DWORD flags = 0;
        if (!includeSeconds) {
            flags = TIME_NOSECONDS;
        }

        // First call to get required size
        int requiredSize = GetTimeFormatEx(
            nullptr, flags, &st, nullptr, nullptr, 0);

        if (requiredSize == 0) {
            throw std::system_error(GetLastError(), std::system_category());
        }

        // Allocate WCHAR buffer
        std::wstring wbuf(requiredSize, L'\0');

        // Second call to actually format
        int result = GetTimeFormatEx(
            nullptr, flags, &st, nullptr, wbuf.data(), requiredSize);

        if (result == 0) {
            throw std::system_error(GetLastError(), std::system_category());
        }

        // Convert WCHAR (UTF-16) to UTF-8
        int utf8len = WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string output(utf8len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, &output[0], utf8len, nullptr, nullptr);
        output.resize(utf8len - 1); // remove null terminator

        return output;

#else // POSIX

        struct tm tm;
        localtime_r(&t, &tm);

        const char* fmt = nullptr;

        if (includeSeconds) {
            fmt = "%X"; // Locale-specific full time with seconds
        } else {
            // Determine 24h vs 12h heuristic using nl_langinfo
            bool is24h = false;
            const char* tfmt = nl_langinfo(T_FMT);
            if (tfmt && std::strchr(tfmt, 'H')) {
                is24h = true;
            }

            fmt = is24h ? "%H:%M" : "%I:%M %p";
        }

        std::ostringstream oss;
        oss.imbue(std::locale("")); // Use user's current locale
        oss << std::put_time(&tm, fmt);
        return oss.str();

#endif
    }
    
};

} // namespace util
} // namespace musx
