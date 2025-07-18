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

#include "musx/util/Logger.h"
#include "musx/util/Fraction.h"

#include "BaseClasses.h"
#include "CommonClasses.h"

 // do not add other dom class dependencies. Use Implementations.cpp for implementations that need total class access.

namespace musx {
namespace dom {

/// @enum KnownShapeDefType
/// @brief Enumerates the shape types we can recognize semantically.
enum class KnownShapeDefType
{
    TenutoMark,                 ///< A horizontal tenuto mark, typically used as an articulation symbol.

    // Add more known types here
};

namespace others {
    
/**
 * @class ShapeData
 * @brief Represents the data for instruction associated with a @ref ShapeDef.
 *
 * This class is identified by the XML node name "shapeData".
 */
class ShapeData : public OthersArray<int>
{
    std::string_view xmlTag() const override { return XmlNodeName; }

public:
    using OthersArray::OthersArray;

    constexpr static std::string_view XmlNodeName = "shapeData"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeData>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

class ShapeInstructionList;
/**
 * @class ShapeDef
 * @brief Represents a shape created in Finale's Shape Designer.
 *
 * This class is identified by the XML node name "shapeDef".
 */
class ShapeDef : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit ShapeDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    /// @enum InstructionType
    /// @brief Defines the instruction types for Shape Designer shapes.
    enum class InstructionType
    {
        /// catch-all for possible undocumented instruction tags.
        Undocumented,

        /// bracket  
        /// data items: (1) bracket type (piano brace, bracket, etc.)
        Bracket,

        /// cloned character for horizontal lines (e.g. trills)  
        /// data items: (5)
        /// - 0, 1: x and y of endpoint (y ignored)
        /// - 2:    unused
        /// - 3:    baseline shift in EMs (1/1000 of current point size)
        /// - 4:    the 32-bit codepoint of the character to clone
        CloneChar,

        /// close path  
        /// data items: none
        ClosePath,

        /// relative curveto (bezier)  
        /// data items: (6) 2 bezier control points and an end point
        CurveTo,

        /// draw character (current font)  
        /// data items: (1) The 32-bit codepoint of the character to draw
        DrawChar,

        /// ellipse  
        /// data items: (2) width and height of bounding rectangle  
        /// current point is lower left corner
        Ellipse,

        /// marks the end of a group  
        /// data items: none
        EndGroup,

        /// external graphic (eps, pict, tiff, etc)  
        /// data items: (3) width, height of graphic, cmper of graphic
        ExternalGraphic,

        /// fill path (alternating, or even-odd rule)  
        /// data items: none
        FillAlt,

        /// fill path (solid, or non-zero winding rule)  
        /// data items: none
        FillSolid,

        /// go to origin point  
        /// data items: none
        GoToOrigin,

        /// go to start of path, same as close path for filled shapes  
        /// data items: none
        GoToStart,

        /// line width  
        /// data items: (1) the new line width in Efix
        LineWidth,

        /// rectangle  
        /// data items: (2) width and height of rectangle  
        /// current point is lower left corner
        Rectangle,

        /// relative lineto  
        /// data items: (2) X, Y endpoint
        RLineTo,

        /// relative moveto  
        /// data items: (2) X, Y endpoint
        RMoveTo,

        /// set arrowhead  
        /// data items: (4) startArrowID, endArrowID, startFlags, endFlags  
        /// the meaning of the flags is currently untested, but they are likely only used to specify built-in vs. custom arrowhead
        SetArrowhead,

        /// set black: equivalent to `SetGray(0)`  
        /// data items: none
        SetBlack,

        /// set dash  
        /// data items: (2) dash length, space between dashes
        SetDash,

        /// set font  
        /// data items: (3) font id, size, efx
        SetFont,

        /// set gray  
        /// data items: (1) gray value between 1 and 100 where 0=black and 100=white
        SetGray,

        /// set white: equivalent to `SetGray(100)`  
        /// data items: none
        SetWhite,

        /// slur  
        /// data items: (6) 2 bezier control points and an end point  
        /// like curve, except 2 more control points are deduced from a global slur thickness setting
        Slur,

        /// start group  
        /// data items: (11) same as StartObject
        StartGroup,

        /// start object  
        /// data items: (11) with indices as noted:
        /// - 0, 1: origin point (x, y)
        /// - 2..5: bounding rect (left, top, right, bottom)
        /// - 6, 7: x and y transform (scale ratio * 1000)
        /// - 8:    rotation transform
        /// - 9,10: always zero
        StartObject,

        /// stroke path  
        /// data items: none
        Stroke,

        /// pen vertical mode  
        /// data items: (1) 1=center, 2=left, 3=right  
        /// tells line-drawing commands to draw the line center, on the left, or on the right of the drawing coordinates specified.
        VerticalMode,
    };

    /**
     * @enum ShapeType
     * @brief Represents different types of shapes in Finale's Shape Designer.
     */
    enum class ShapeType
    {
        Other = 0,          ///< May Correspond to "other" in XML (but may not appear in XML). All pre-Fin2k shapes use this value.
        Articulation = 1,   ///< Corresponds to "articulation" in XML.
        Barline = 2,        ///< Corresponds to "barline" in XML.
        Executable = 3,     ///< Corresponds to "executable" in XML.
        Expression = 4,     ///< Corresponds to "expression" in XML.
        CustomStem = 5,     ///< Corresponds to "note" in XML.
        Frame = 6,          ///< Corresponds to "frame" in XML.
        Arrowhead = 7,      ///< Corresponds to "arrowhead" in XML.
        Fretboard = 8,      ///< Corresponds to "fretboard" in XML.
        Clef = 9            ///< Corresponds to "clef" in XML.
    };

    Cmper instructionList;  ///< Instruction list @ref Cmper. (xml node is `<instList>`)
    Cmper dataList;         ///< Instruction data list @ref Cmper.
    ShapeType shapeType;    ///< Shape type (specifies which type of entity this shape pertains to)

    /// @brief Returns true if this shape does not draw anything.
    bool isBlank() const
    { return instructionList == 0; }

    /// @brief Iterates through the instructions in the shape
    /// @param callback The callback function. Returning `false` from this function aborts the iteration loop.
    void iterateInstructions(std::function<bool(InstructionType, std::vector<int>)> callback) const;


    /// @brief Determine if this is a recognized shape type
    /// @return If the shape is recognized, return its known type. Otherwise return std::nullopt.
    std::optional<KnownShapeDefType> recognize() const;

    constexpr static std::string_view XmlNodeName = "shapeDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeDef>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
/// @struct ShapeInstruction
/// @brief Helper functions and structs for decoding instruction data vectors in @ref ShapeDef.
struct ShapeInstruction
{
    /// @brief Holds the parsed data for a LineWidth instruction.
    struct LineWidth {
        Efix efix;  ///< The line width in Efix units.
    };

    /// @brief Holds the parsed data for an RLineTo instruction.
    struct RLineTo {
        Evpu dx;  ///< The relative X coordinate delta in Evpu.
        Evpu dy;  ///< The relative Y coordinate delta in Evpu.
    };

    // add more data structs as needed...

    /// @brief Attempts to parse a LineWidth instruction.
    /// @param[in] type The instruction type; must be InstructionType::LineWidth.
    /// @param[in] data The instruction's associated data vector.
    /// @return A LineWidth struct if successfully parsed, otherwise std::nullopt.
    static std::optional<LineWidth> parseLineWidth(ShapeDef::InstructionType type, const std::vector<int>& data);

    /// @brief Attempts to parse an RLineTo instruction.
    /// @param[in] type The instruction type; must be InstructionType::RLineTo.
    /// @param[in] data The instruction's associated data vector.
    /// @return An RLineTo struct if successfully parsed, otherwise std::nullopt.
    static std::optional<RLineTo> parseRLineTo(ShapeDef::InstructionType type, const std::vector<int>& data);

    /// @brief Attempts to parse a SetFont instruction into a FontInfo object.
    /// @param[in] document The owning document used to construct the FontInfo object.
    /// @param[in] type The instruction type; must be InstructionType::SetFont.
    /// @param[in] data The instruction's associated data vector.
    /// @return A FontInfo object if successfully parsed, otherwise std::nullopt.
    static std::optional<FontInfo> parseSetFont(const DocumentWeakPtr& document, ShapeDef::InstructionType type, const std::vector<int>& data);

    // Add more parsers as needed...
};

/**
 * @class ShapeInstructionList
 * @brief Represents the data for instruction associated with a @ref ShapeDef.
 *
 * This class is identified by the XML node name "shapeList".
 */
class ShapeInstructionList : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit ShapeInstructionList(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }
    
    /// @brief An instruction in the shape
    class InstructionInfo
    {
    public:

        int numData{};                      ///< the number of data items consumed by this instruction (See @ref ShapeData.)
        ShapeDef::InstructionType type{};   ///< the type of instruction

        static const xml::XmlElementArray<InstructionInfo>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
    };

    std::vector<std::shared_ptr<InstructionInfo>> instructions; ///< The instructions.

    constexpr static std::string_view XmlNodeName = "shapeList"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeInstructionList>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace musx
