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

namespace details {
class Bracket;
}

namespace others {

/**
* @class OssiaBounds
 * @brief Stores additional positioning offsets for ossia passages.
 *
 * The cmper is a unique identifier used to reference and tie together this set of ossia class instances.
 * This class is identified by the XML node name "ossiaBounds".
 */
class OssiaBounds : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit OssiaBounds(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    Evpu topMargin{};       ///< Top margin (negative moves music down) (xml node is `<topAdd>`)
    Evpu leftMargin{};      ///< Left margin (xml node is `<leftAdd>`)
    Evpu rightMargin{};     ///< Right margin (xml node is `<rightAdd>`)
    Evpu centerPull{};      ///< Centering adjustment,

    constexpr static std::string_view XmlNodeName = "ossiaBounds"; ///< The XML node name for this type.
    static const xml::XmlElementArray<OssiaBounds>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class OssiaHeader
 * @brief Header properties for an ossia passage (clef, key, time, grouping).
 *
 * The cmper is a unique identifier used to reference and tie together this set of ossia class instances.
 * This class is identified by the XML node name "ossiaHeader".
 */
class OssiaHeader : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit OssiaHeader(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    ClefIndex clefId{};                         ///< Clef index (xml node is `<clef>`)
    std::shared_ptr<KeySignature> keySig;       ///< Display the music with this key signature. It does not transpose. The factory guarantees this is not null.
    Cmper beats{};                              ///< Time signature numerator or beats per measure (xml node is `<beats>`)
    Cmper divBeat{};                            ///< Time signature denominator division unit (xml node is `<divbeat>`)
    Cmper bracketGroup{};                       ///< Ossia bracket group number (xml node is `<group>`)
    bool compositeNumerator{};                  ///< Indicates a composite numerator for the time signature (xml node is `<altNumTsig>`)
    bool compositeDenominator{};                ///< Indicates a composite denominator for the time signature (xml node is `<altDenTsig>`)
    Measure::BarlineType barlineType{};         ///< Barline type. (xml node is `<barline>`)

    /// @brief Create a shared pointer to an instance of the @ref TimeSignature for this ossia.
    MusxInstance<TimeSignature> createTimeSignature() const;

    /// @brief Gets the brackets associated with this ossia. The are in the order created. The
    /// #details::Bracket::horzAdjLeft member controls the order in which they display.
    MusxInstanceList<details::Bracket> getBrackets() const;

    void integrityCheck(const std::shared_ptr<Base>& ptrToThis) override
    {
        this->OthersBase::integrityCheck(ptrToThis);
        if (!keySig) {
            keySig = std::make_shared<KeySignature>(getDocument());
        }
    }

    constexpr static std::string_view XmlNodeName = "ossiaHeader"; ///< The XML node name for this type.
    static const xml::XmlElementArray<OssiaHeader>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
* @class OssiaMusic
 * @brief Stores display and source options for an ossia passage.
 *
 * The cmper is a unique identifier used to reference and tie together this set of ossia class instances.
 * This class is identified by the XML node name "ossiaMusic".
 */
class OssiaMusic : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit OssiaMusic(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    int percent{};                  ///< Percent size of the ossia relative to normal (100 = 100%)
    StaffCmper sourceStaffId{};     ///< Source staff (xml node is `<qinst>`)
    MeasCmper sourceMeasureId{};    ///< Source measure (xml node is `<qmeasure>`)
    bool hideLeftLine{};            ///< Hide the left bar/line (xml node is `<negLeftLine>`)
    bool hideKey{};                 ///< Hide the key signature (xml node is `<negKey>`)
    bool hideTime{};                ///< Hide the time signature (xml node is `<negTime>`)
    bool hideDynamic{};             ///< Hide dynamics (xml node is `<negDynamic>`)
    bool hideStaff{};               ///< Hide staff lines (xml node is `<negStaff>`)
    bool hideRepeat{};              ///< Hide repeat marks (xml node is `<negRepeat>`)
    bool hideClef{};                ///< Hide clef (xml node is `<negClef>`)
    // posMode: Positioning behavior for the ossia (xml node is `<posMode>`).
    // posMode appears to be an unused legacy spacing feature and is intentionally not stored. The only value ever seen
    // is "ossiaDefault".

    constexpr static std::string_view XmlNodeName = "ossiaMusic"; ///< The XML node name for this type.
    static const xml::XmlElementArray<OssiaMusic>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class PageOssiaAssign
 * @brief Represents the assignment of an ossia passage to a specific page.
 *
 * Each record references an ossia instance and positions it from the upper left corner of the page.
 * This class is identified by the XML node name "pageOssiaAssign".
 */
class PageOssiaAssign : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit PageOssiaAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    Cmper ossiaId{};        ///< Cmper of related @ref OssiaBounds, @ref OssiaHeader, and @ref OssiaMusic classes. (xml node is `<arbnum>`)
    Evpu xOffset{};         ///< Horizontal offset (xml node is `<topAdd>`)
    Evpu yOffset{};         ///< Vertical offset (xml node is `<leftAdd>`)
    Evpu measureWidth{};    ///< Measure width for this ossia (xml node is `<mwidth>`)
    bool hidden{};          ///< Whether the ossia is hidden.

    constexpr static std::string_view XmlNodeName = "pageOssiaAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PageOssiaAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} //namespace others

namespace details {

/**
 * @class MeasureOssiaAssign
 * @brief Assigns an ossia passage to a specific staff/measure location.
 *
 * Cmper1 is the staff @ref Cmper and Cmper2 is the measure @ref Cmper.
 * This class is identified by the XML node name "measOssiaAssign".
 */
class MeasureOssiaAssign : public DetailsBase
{
public:
    /** @brief Constructor function */
    explicit MeasureOssiaAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper staffId, Cmper measureId, Inci inci)
        : DetailsBase(document, partId, shareMode, staffId, measureId, inci)
    {
    }

    Cmper ossiaId{};     ///< Cmper of related @ref others::OssiaBounds, @ref others::OssiaHeader, and @ref others::OssiaMusic classes. (xml node is `<arbnum>`)
    Evpu xOffset{};      ///< Horizontal offset (xml node is `<topAdd>`)
    Evpu yOffset{};      ///< Vertical offset (xml node is `<leftAdd>`)
    bool hidden{};       ///< Whether the ossia is hidden.

    constexpr static std::string_view XmlNodeName = "measOssiaAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MeasureOssiaAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} //namespace details

} //namespace dom
} //namespace musx
