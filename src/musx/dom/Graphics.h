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

#include "BaseClasses.h"
#include "CommonClasses.h"
#include "Others.h"
 // do not add other dom class dependencies. Use .cpp files for implementations that need total class access.

namespace musx {
namespace dom {

namespace others {

/**
 * @class FileAlias
 * @brief Opaque storage for a platform-specific alias handle associated with a graphic.
 *
 * The DOM does not interpret aliasHandle bytes; callers may decode/resolve them if desired.
 */
class FileAlias : public OthersBase
{
public:
    /// @brief Constructor
    explicit FileAlias(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    size_t length{};                            ///< Declared byte length.
    std::vector<std::uint8_t> aliasHandle{};    ///< Opaque blob (decoded from hex text)

    void integrityCheck(const std::shared_ptr<Base>& ptrToThis) override
    {
        OthersBase::integrityCheck(ptrToThis);
        if (length != aliasHandle.size()) {
            MUSX_INTEGRITY_ERROR("Stated length does not match actual size of FileAlias " + std::to_string(getCmper()) + ".");
        }
    }

    constexpr static std::string_view XmlNodeName = "fileAlias"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FileAlias>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FileDescription
 * @brief Describes the locator flavor Finale used for the associated file/graphic.
 *
 * The DOM does not enforce/interpret pathType semantics. The pathType is preserved as-is.
 */
class FileDescription : public OthersBase
{
public:
    /// @brief Constructor
    explicit FileDescription(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /// @enum PathType
    /// @brief Defines the possible path types.
    enum class PathType
    {
        DosPath,                ///< Path is a DOS filepath.
        MacFsSpec,              ///< Path is a Mac FS Spec.
        MacAlias,               ///< Path is defined by mac alias. (See @ref FileAlias.)
        MacPosixPath,           ///< Path is a POSIX filepath.
        MacUrlBookmark          ///< Path is a Mac URL bookmark. (See @ref FileUrlBookmark.)
    };

    uint32_t version{};         ///< Version as written by Finale. (This was intended for tracking changes to the data format, but it was never used.)
    int volRefNum{};            ///< Classic Mac OS volume reference number, used with legacy MacFsSpec path.
    int dirId{};                ///< Classic Mac OS directory ID, used with legacy MacFsSpec path. (xml node is `<dirID>`)
    PathType pathType;          ///< Path type. (Determines which of @ref FileAlias and @ref FileUrlBookmark to use as well as how to interpret #FilePath::path.)
    Cmper pathId{};             ///< Cmper of @ref FilePath? It is unknown if this value ever differs from the cmper of #FileDescription. (xml node is `<pathID>`)

    constexpr static std::string_view XmlNodeName = "fileDesc"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FileDescription>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FilePath
 * @brief Stores a human-readable path/leaf name recorded by Finale.
 *
 * @note This value is not authoritative when graphics are embedded.
 * Embedded graphics files are stored in the `/graphics/` directory of the musx zip file.
 * For how to interpret the path value, see @ref FileDescription.
 */
class FilePath : public OthersBase
{
public:
    /// @brief Constructor
    explicit FilePath(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::string path;   ///< Display/path string exactly as in XML. For MacAlias paths, this is only the source file name.

    constexpr static std::string_view XmlNodeName = "filePath"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FilePath>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FileUrlBookmark
 * @brief Opaque storage for a platform-specific URL/bookmark blob (e.g., macOS CFURL bookmark).
 *
 * The DOM does not interpret urlBookmarkData bytes; callers may decode/resolve them if desired.
 */
class FileUrlBookmark : public OthersBase
{
public:
    /// @brief Constructor
    explicit FileUrlBookmark(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    size_t length{};                                ///< Declared byte length.
    std::vector<std::uint8_t> urlBookmarkData{};    ///< Opaque blob decoded from hex text.

    void integrityCheck(const std::shared_ptr<Base>& ptrToThis) override
    {
        OthersBase::integrityCheck(ptrToThis);
        if (length != urlBookmarkData.size()) {
            MUSX_INTEGRITY_ERROR("Stated length does not match actual size of FileUrlBookmark " + std::to_string(getCmper()) + ".");
        }
    }

    constexpr static std::string_view XmlNodeName = "fileURLBookmark"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FileUrlBookmark>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} //namespace others

namespace details {


} //namespace details

} //namespace dom
} //namespace musx
