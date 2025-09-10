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

/**
 * @class PageGraphicAssign
 * @brief Represents a page graphic assignment with positioning and page-range properties.
 *
 * Instances of #PageGraphicAssign use page assignment IDs rather than straightforward page numbers.
 * If the cmper is non-0, #startPage is not used and the cmper specifies a page assignment ID that
 * defines which page this graphic is assigned to. If the cmper is 0, #startPage specifies the first
 * page assignment ID to which the graphic is assigned (and #displayType governs whether it repeats
 * on odd/even/all pages thereafter).
 *
 * The inci value specifies a particular page graphic when more than one exists for a cmper value.
 *
 * This class is identified by the XML node name "pageGraphicAssign".
 */
class PageGraphicAssign : public OthersBase
{
public:

    /**
     * @enum PageAssignType
     * @brief Which pages a multipage assignment appears on
     */
    enum class PageAssignType
    {
        AllPages,
        Even,
        Odd,
        One
    };

    /**
     * @enum PositionFrom
     * @brief Reference frame for positioning.
     */
    enum class PositionFrom
    {
        Margins,    ///< Position is relative to page margins.
        PageEdge    ///< Position is relative to page edge.
    };

    using HorizontalAlignment = options::TextOptions::HorizontalAlignment;  ///< Horizontal alignment options.
    using VerticalAlignment   = options::TextOptions::VerticalAlignment;    ///< Vertical alignment options.

    /** @brief Constructor */
    explicit PageGraphicAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    uint32_t version{};                 ///< Version as written by Finale. (This was intended for tracking changes to the data format, but it was never used.)
    Evpu left{};                        ///< Horizontal position from reference frame.
    Evpu bottom{};                      ///< Vertical position from reference frame.
    Evpu width{};                       ///< Display width of the placed graphic.
    Evpu height{};                      ///< Display height of the placed graphic.
    Cmper fDescId{};                    ///< The Cmper of the assigned @ref FileDescription . (xml tag is `<fDescID>`)
    PageAssignType displayType{};       ///< Whether the assignment appears on all/even/odd pages.
    bool hidden{};                      ///< If true, the graphic does not display: inverse of "Show" context menu option. (xml tag is `<displayHidden>`)
    HorizontalAlignment hAlign{};       ///< Horizontal alignment for left/all pages. (xml tag is `<halign>`)
    VerticalAlignment vAlign{};         ///< Vertical alignment for left/all pages. (xml tag is `<valign>`)
    PositionFrom posFrom{};             ///< Position reference for left/all pages.
    bool fixedPerc{};                   ///< If true, horizontal and vertical scaling is the same.
    PageCmper startPage{};              ///< First page assignment ID where the graphic appears when cmper==0.
    PageCmper endPage{};                ///< Last page assignment ID where the graphic appears when cmper==0.
    bool savedRecord{};                 ///< Indicates a stored/saved record. (Used internally by Finale when a graphic is created.)
    Evpu origWidth{};                   ///< Intrinsic/original width of the graphic.
    Evpu origHeight{};                  ///< Intrinsic/original height of the graphic.
    HorizontalAlignment rightPgHAlign{};///< Horizontal alignment on right pages.
    VerticalAlignment rightPgVAlign{};  ///< Vertical alignment on right pages.
    PositionFrom rightPgPosFrom{};      ///< Position reference for right pages.
    bool rightPgFixedPerc{};            ///< If true, right-page horizontal and vertical scaling is the same.
    Evpu rightPgLeft{};                 ///< Horizontal position for right pages.
    Evpu rightPgBottom{};               ///< Vertical position for right pages.
    Cmper graphicCmper{};               ///< Graphic instance Cmper. A non-zero value indicates that the graphic is embedded in the `musx` file.
                                        ///< Embedded graphics are stored in the @c /graphics/ subdirectory of the musx zip archive. This value
                                        ///< also identifies the filename within that directory. For example, if the @c graphicCmper is 3 and the
                                        ///< embedded file has extendion @c .png then the file is called @c 3.png within the directory.

    /// @brief Return the starting page number, taking into account leading blank pages in all parts.
    /// This calculation mimics observed behavior in Finale.
    /// @return The first page in @p forPartId on which the page graphic appears. If it does not appear on the part,
    /// the function returns std::nullopt.
    std::optional<PageCmper> calcStartPageNumber(Cmper forPartId) const;

    /// @brief Return the ending page number, taking into account leading blank pages in all parts
    /// This calculation mimics observed behavior in Finale.
    /// @return The first page in @p forPartId on which the page graphic appears. If it does not appear on the part,
    /// the function returns std::nullopt.
    std::optional<PageCmper> calcEndPageNumber(Cmper forPartId) const;

    /// @brief Returns true if this is a multi-page assignment.
    bool isMultiPage() const
    { return getCmper() == 0 && startPage != endPage; }

    /// @brief Returns true if this is a multi-page assignment that is assigned to through last page, no matter how many.
    bool isMultiAssignedThruLastPage() const
    { return isMultiPage() && endPage == 0; }

    void integrityCheck(const std::shared_ptr<Base>& ptrToThis) override
    {
        this->OthersBase::integrityCheck(ptrToThis);
        if (getCmper() != 0) {
            if (startPage != getCmper() || endPage != getCmper()) {
                MUSX_INTEGRITY_ERROR("PageGraphicAssign " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value_or(0)) +
                    " has startPage or endPage value that does not match.");
            }
        }
    }

    /// @brief Returns a specific page graphic assignment for a given page number in a given part.
    /// This allows the caller not to have to know the conversion to page assignment IDs.
    /// @param document The document to search.
    /// @param partId The ID of the linked part to search.
    /// @param pageId The page number to search for, or zero for multipage assignments.
    /// @param inci The inci of the specific page text assignment to retrieve.
    static MusxInstance<others::PageGraphicAssign> getForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId, Inci inci);

    /// @brief Returns all the page graphic assignments for a given page number in a given part.
    /// This allows the caller not to have to know the conversion to page assignment IDs.
    /// @param document The document to search.
    /// @param partId The ID of the linked part to search.
    /// @param pageId The page number to search for, or zero for all multipage assignments.
    static MusxInstanceList<others::PageGraphicAssign> getArrayForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId);

    constexpr static std::string_view XmlNodeName = "pageGraphicAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PageGraphicAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} //namespace others

namespace details {


} //namespace details

} //namespace dom
} //namespace musx
