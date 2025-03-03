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
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "BaseClasses.h"
#include "CommonClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {

/**
 * @namespace musx::dom::texts
 * @brief Classes in the @ref TextsPool.
 */
namespace texts {

/**
 * @class FileInfoText
 * @brief Contains File Info text (from Finale's Score Manager)
 */
class FileInfoText : public TextsBase
{
public:
    /**
     * @brief Constructs a `FileInfoText` object.
     * 
     * @param document A weak pointer to the parent document
     * @param shareMode Always `ShareMode::All`, but this parameeter is needed for the generic factory routine.
     * @param partId Always 0, but this parameter is needed for the generic factory routine.
     * @param textNumber The text number (`Cmper`).
     */
    explicit FileInfoText(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper textNumber)
        : TextsBase(document, partId, shareMode, textNumber)
    {
        if (textNumber <= 0 || textNumber > Cmper(TextType::Subtitle)) {
            throw std::invalid_argument("invalid text type value provided to FileInfoText constructor");
        }
    }

    /**
     * @brief Constructs a `FileInfoText` object.
     * 
     * @param document A weak pointer to the parent document
     * @param textNumber The text number (`Cmper`).
     */
    explicit FileInfoText(const DocumentWeakPtr& document, Cmper textNumber)
        : TextsBase(document, 0, ShareMode::All, textNumber)
    {
        if (textNumber <= 0 || textNumber > Cmper(TextType::Subtitle)) {
            throw std::invalid_argument("invalid text type value provided to FileInfoText constructor");
        }
    }

    /**
     * @enum TextType
     * @brief Represents various text types for the file header.
     *
     * Use these values to search the @ref TextsPool for file info text.
     */
    enum class TextType : Cmper
    {
        Title = 1,        ///< Title of the piece.
        Composer = 2,     ///< Composer of the piece.
        Copyright = 3,    ///< Copyright information for the piece.
        Description = 4,  ///< Description of the piece.
        Lyricist = 5,     ///< Lyricist  the piece.
        Arranger = 6,     ///< Arranger of the piece.
        Subtitle = 7      ///< Subtitle of the piece.
    };

    /**
     * @brief Returns the @ref TextType of the current instance.
     */
    TextType getTextType() const
    {
        const Cmper textNumber = getTextNumber();
        assert(textNumber > 0 && textNumber <= Cmper(TextType::Subtitle));
        return TextType(textNumber);
    }

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "fileInfo";
};

/**
 * @class LyricsTextBase
 * @brief Base class for lyrics text
 */
class LyricsTextBase : public TextsBase
{
public:
    using TextsBase::TextsBase;

    std::vector<std::shared_ptr<LyricsSyllableInfo>> syllables; ///< the syllable info for the lyric text, constructed by the factory

    /// @brief Creates the syllables array. Used by the factory but available at any time.
    void createSyllableInfo();
};

/**
 * @class LyricsVerse
 * @brief Contains verse text for lyrics
 */
class LyricsVerse : public LyricsTextBase
{
public:
    using LyricsTextBase::LyricsTextBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "verse";
};

/**
 * @class LyricsChorus
 * @brief Contains chorus text for lyrics
 */
class LyricsChorus : public LyricsTextBase
{
public:
    using LyricsTextBase::LyricsTextBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "chorus";
};

/**
 * @class LyricsSection
 * @brief Contains section text for lyrics
 */
class LyricsSection : public LyricsTextBase
{
public:
    using LyricsTextBase::LyricsTextBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "section";
};

/**
 * @class BlockText
 * @brief Contains block text (Finale Text Tool, both page- and measure-attached)
 */
class BlockText : public TextsBase
{
public:
    using TextsBase::TextsBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "blockText";
};

/**
 * @class SmartShapeText
 * @brief Contains text blocks associated with custom line smart shapes
 */
class SmartShapeText : public TextsBase
{
public:
    using TextsBase::TextsBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "smartShapeText";
};

/**
 * @class ExpressionText
 * @brief Contains text blocks associated with text expressions
 */
class ExpressionText : public TextsBase
{
public:
    using TextsBase::TextsBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "expression";
};

/**
 * @class BookmarkText
 * @brief Contains bookmark description text
 */
class BookmarkText : public TextsBase
{
public:
    using TextsBase::TextsBase;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "bookmarkText";
};

} // namespace texts
} // namespace dom
} // namespace musx