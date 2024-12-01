/*
 * Copyright (C) 2024, Robert Patterson
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
#include <optional>

namespace musx {
namespace dom {

/**
 * @namespace musx::dom::header
 * @brief Document header classes are in this namespace.
 */
namespace header {

/**
 * @brief Enum representing the word order used in the document.
 */
enum class WordOrder
{
    LittleEndian, ///< Little-endian word order
    BigEndian     ///< Big-endian word order
};

/**
 * @brief Enum representing the text encoding used in the document.
 */
enum class TextEncoding
{
    Mac,        ///< Mac text encoding
    Windows,    ///< Windows text encoding
    Other       ///< Other/unknown text encoding
};

/**
 * @brief Enum representing the platform on which the document was created.
 */
enum class Platform
{
    Mac,        ///< macOS platform
    Windows,    ///< Windows platform
    Other       ///< Other/unknown platform
};

/**
 * @brief Struct representing version information.
 */
struct FinaleVersion
{
    int major;                         ///< Major version number
    int minor;                         ///< Minor version number
    std::optional<int> maint;          ///< Optional maintenance version number
    std::string devStatus;             ///< Development status (e.g., "dev", "release")
    std::optional<int> build;          ///< Optional build number
};

/**
 * @brief Struct representing a date with associated metadata.
 */
struct FileInfo
{
    int year;                       ///< Year (4 digits)
    int month;                      ///< Month (1-12)
    int day;                        ///< Day (1-31)
    std::string modifiedBy;         ///< Initials of person who modified the file
    FinaleVersion finaleVersion;    ///< Version of the Finale software
    std::string application;        ///< Application that created or modified the file
    Platform platform;              ///< Platform on which the file was created
    FinaleVersion appVersion;       ///< Version of the application
    FinaleVersion fileVersion;      ///< Version of the file format
    std::string appRegion;          ///< Region information (e.g., "US")
};

/**
 * @brief Struct representing the header of a document.
 */
struct Header
{
    WordOrder wordOrder;            ///< Word order used in the document
    TextEncoding textEncoding;      ///< Text encoding used in the document
    FileInfo created;               ///< Creation date and associated metadata
    FileInfo modified;              ///< Modification date and associated metadata
};

/** @brief Shared `Header` pointer */
using HeaderPtr = std::shared_ptr<Header>;

} // namespace header
} // namespace dom
} // namespace musx
