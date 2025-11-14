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
#include "Details.h"

 // do not add other dom class dependencies. Use Implementations.cpp for implementations that need total class access.

namespace musx {
namespace dom {

/// @enum KnownShapeDefType
/// @brief Enumerates the shape types we can recognize semantically.
enum class KnownShapeDefType
{
    Blank,                      ///< A blank shape containing no instructions.
    TenutoMark,                 ///< A horizontal tenuto mark, typically used as an articulation symbol.

    // Add more known types here
};

/// @enum ShapeDefInstructionType
/// @brief Defines the instruction types for Shape Designer shapes.
enum class ShapeDefInstructionType
{
    /// catch-all for possible undocumented instruction tags.
    Undocumented,

    /// bracket  
    /// data items: (1) bracket type (piano brace, bracket, etc.)
    /// @note This does not actually draw the bracket. It simply indicates the start of automatically created instructions
    /// that draw the bracket, ending with ClosePath.
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
    /// data items: (3) width, height of graphic, cmper of corresponding @ref ShapeGraphicAssign record
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
    /// - 9,10: unused/undocumented
    StartObject,

    /// stroke path  
    /// data items: none
    Stroke,

    /// pen vertical mode  
    /// data items: (1) 1=center, 2=left, 3=right  
    /// tells line-drawing commands to draw the line center, on the left, or on the right of the drawing coordinates specified.
    VerticalMode,
};

/// @struct ShapeDefInstruction
/// @brief Helper functions and structs for decoding instruction data vectors in @ref others::ShapeDef.
struct ShapeDefInstruction
{
    /// @brief Holds the parsed data for an Undocumented instruction.
    /// @details Catch-all for instructions whose data layout is not fully known.
    struct Undocumented {
        std::vector<int> data; ///< Raw data items for the undocumented instruction.
    };

    /// @brief Holds the parsed data for a Bracket instruction.
    struct Bracket {
        details::Bracket::BracketStyle bracketStyle; ///< The style of bracket being created.
    };

    /// @brief Holds the parsed data for a CloneChar instruction.
    struct CloneChar {
        Evpu dx;            ///< The X coordinate of the endpoint (relative to current point).
        Evpu dy;            ///< The Y coordinate of the endpoint (currently ignored).
        int unused2;        ///< Unused/undocumented field at index 2.
        int  baselineShift; ///< Baseline shift in EMs (1/1000 of current point size).
        char32_t codePoint; ///< The 32-bit codepoint of the character to clone.
    };

    /// @brief Holds the parsed data for a CurveTo instruction.
    struct CurveTo {
        Evpu c1dx; ///< Relative X offset of the first control point.
        Evpu c1dy; ///< Relative Y offset of the first control point.
        Evpu c2dx; ///< Relative X offset of the second control point.
        Evpu c2dy; ///< Relative Y offset of the second control point.
        Evpu edx;  ///< Relative X offset of the end point.
        Evpu edy;  ///< Relative Y offset of the end point.
    };

    /// @brief Holds the parsed data for a DrawChar instruction.
    struct DrawChar {
        char32_t codePoint; ///< The 32-bit codepoint of the character to draw.
    };

    /// @brief Holds the parsed data for an Ellipse instruction.
    struct Ellipse {
        Evpu width;  ///< The width of the bounding rectangle.
        Evpu height; ///< The height of the bounding rectangle.
    };

    /// @brief Holds the parsed data for an ExternalGraphic instruction.
    struct ExternalGraphic {
        Evpu width;   ///< The width of the placed graphic.
        Evpu height;  ///< The height of the placed graphic.
        Cmper cmper;  ///< The cmper of the corresponding ShapeGraphicAssign record.
    };

    /// @brief Holds the parsed data for a LineWidth instruction.
    struct LineWidth {
        Efix efix; ///< The line width in Efix units.
    };

    /// @brief Holds the parsed data for a Rectangle instruction.
    struct Rectangle {
        Evpu width;  ///< The width of the rectangle.
        Evpu height; ///< The height of the rectangle.
    };

    /// @brief Holds the parsed data for an RLineTo instruction.
    struct RLineTo {
        Evpu dx; ///< The relative X coordinate delta in Evpu.
        Evpu dy; ///< The relative Y coordinate delta in Evpu.
    };

    /// @brief Holds the parsed data for an RMoveTo instruction.
    struct RMoveTo {
        Evpu dx; ///< The relative X coordinate delta in Evpu.
        Evpu dy; ///< The relative Y coordinate delta in Evpu.
    };

    /// @brief Holds the parsed data for a SetArrowhead instruction.
    struct SetArrowhead {
        int startArrowId; ///< Identifier of the start arrowhead.
        int endArrowId;   ///< Identifier of the end arrowhead.
        int startFlags;   ///< Flags for the start arrowhead (built-in vs custom, etc.).
        int endFlags;     ///< Flags for the end arrowhead (built-in vs custom, etc.).
    };

    /// @brief Holds the parsed data for a SetDash instruction.
    struct SetDash {
        Efix dashLength;  ///< Length of each dash segment.
        Efix spaceLength; ///< Space between dashes.
    };

    /// @brief Holds the parsed data for a SetFont instruction.
    struct SetFont {
        /// @brief Constructor function
        SetFont(const DocumentWeakPtr& document) : font(document)
        {}
        FontInfo font;  ///< the parsed font
    };

    /// @brief Holds the parsed data for a SetGray instruction.
    struct SetGray {
        int gray; ///< Gray value between 0 (black) and 100 (white).
    };

    /// @brief Holds the parsed data for a Slur instruction.
    /// @details Same data layout as CurveTo, but rendered using the slur thickness settings.
    struct Slur {
        Evpu c1dx; ///< Relative X offset of the first control point.
        Evpu c1dy; ///< Relative Y offset of the first control point.
        Evpu c2dx; ///< Relative X offset of the second control point.
        Evpu c2dy; ///< Relative Y offset of the second control point.
        Evpu edx;  ///< Relative X offset of the end point.
        Evpu edy;  ///< Relative Y offset of the end point.
    };

    /// @brief Holds the parsed data for a StartObject instruction.
    struct StartObject {
        Evpu originX;  ///< X coordinate of the origin point.
        Evpu originY;  ///< Y coordinate of the origin point.
        Evpu left;     ///< Left of the bounding rectangle.
        Evpu top;      ///< Top of the bounding rectangle.
        Evpu right;    ///< Right of the bounding rectangle.
        Evpu bottom;   ///< Bottom of the bounding rectangle.
        int  scaleX;   ///< X scale transform (scale ratio * 1000).
        int  scaleY;   ///< Y scale transform (scale ratio * 1000).
        int  rotation; ///< Rotation transform.
        int  unused9;  ///< Undocumented/unused field at index 9.
        int  unused10; ///< Undocumented/unused field at index 10.
    };

    /// @brief Holds the parsed data for a StartGroup instruction.
    /// @details Uses the same layout as StartObject.
    struct StartGroup {
        Evpu originX;  ///< X coordinate of the origin point.
        Evpu originY;  ///< Y coordinate of the origin point.
        Evpu left;     ///< Left of the bounding rectangle.
        Evpu top;      ///< Top of the bounding rectangle.
        Evpu right;    ///< Right of the bounding rectangle.
        Evpu bottom;   ///< Bottom of the bounding rectangle.
        int  scaleX;   ///< X scale transform (scale ratio * 1000).
        int  scaleY;   ///< Y scale transform (scale ratio * 1000).
        int  rotation; ///< Rotation transform.
        int  unused9;  ///< Undocumented/unused field at index 9.
        int  unused10; ///< Undocumented/unused field at index 10.
    };

    /// @brief Pen vertical alignment modes for the VerticalMode instruction.
    enum class VerticalAlign {
        Center = 1, ///< Draw line centered on the coordinates.
        Left   = 2, ///< Draw line on the "left" side of the coordinates.
        Right  = 3  ///< Draw line on the "right" side of the coordinates.
    };

    /// @brief Holds the parsed data for a VerticalMode instruction.
    struct VerticalMode {
        VerticalAlign mode; ///< The pen vertical alignment mode.
    };


    /// @brief A variant for passing specific data per instruction type.
    using InstructionData = std::variant<
        std::monostate,   // for commands with no data
        Undocumented,
        Bracket,
        CloneChar,
        CurveTo,
        DrawChar,
        Ellipse,
        ExternalGraphic,
        LineWidth,
        Rectangle,
        RLineTo,
        RMoveTo,
        SetArrowhead,
        SetDash,
        SetFont,
        SetGray,
        Slur,
        StartGroup,
        StartObject,
        VerticalMode
    >;

    /// @struct Decoded
    /// @brief Contains the parsed instruction type with its data
    struct Decoded
    {
        ShapeDefInstructionType type{};             ///< Instruction type
        InstructionData data{std::monostate{}};     ///< Data for instruction

        /// @brief Returns true is the data is valid.
        bool valid() const noexcept { return m_valid; }

        /// @brief Sets the payload
        /// @tparam T The instruction-specific type for the data.
        /// @param optData The data supplied from the parse routine.
        template <typename T>
        void setPayload(std::optional<T>&& optData)
        {
            if (optData) {
                // Move the contained T into the variant
                data = std::move(*optData);
                m_valid = true;
            } else {
                // We *tried* to parse data and failed
                m_valid = false;
            }
        }

    private:
        bool m_valid{true};  ///< No-data instructions are valid by default.
    };

    /// @brief Attempts to parse an Undocumented instruction (raw passthrough).
    static std::optional<Undocumented> parseUndocumented(const std::vector<int>& data);

    /// @brief Attempts to parse a Bracket instruction.
    static std::optional<Bracket> parseBracket(const std::vector<int>& data);

    /// @brief Attempts to parse a CloneChar instruction.
    static std::optional<CloneChar> parseCloneChar(const std::vector<int>& data);

    /// @brief Attempts to parse a CurveTo instruction.
    static std::optional<CurveTo> parseCurveTo(const std::vector<int>& data);

    /// @brief Attempts to parse a DrawChar instruction.
    static std::optional<DrawChar> parseDrawChar(const std::vector<int>& data);

    /// @brief Attempts to parse an Ellipse instruction.
    static std::optional<Ellipse> parseEllipse(const std::vector<int>& data);

    /// @brief Attempts to parse an ExternalGraphic instruction.
    static std::optional<ExternalGraphic> parseExternalGraphic(const std::vector<int>& data);

    /// @brief Attempts to parse a LineWidth instruction.
    static std::optional<LineWidth> parseLineWidth(const std::vector<int>& data);

    /// @brief Attempts to parse a Rectangle instruction.
    static std::optional<Rectangle> parseRectangle(const std::vector<int>& data);

    /// @brief Attempts to parse an RLineTo instruction.
    static std::optional<RLineTo> parseRLineTo(const std::vector<int>& data);

    /// @brief Attempts to parse an RMoveTo instruction.
    static std::optional<RMoveTo> parseRMoveTo(const std::vector<int>& data);

    /// @brief Attempts to parse a SetArrowhead instruction.
    static std::optional<SetArrowhead> parseSetArrowhead(const std::vector<int>& data);

    /// @brief Attempts to parse a SetDash instruction.
    static std::optional<SetDash> parseSetDash(const std::vector<int>& data);

    /// @brief Attempts to parse a SetFont instruction.
    static std::optional<SetFont> parseSetFont(const DocumentWeakPtr& document, const std::vector<int>& data);

    /// @brief Attempts to parse a SetGray instruction.
    static std::optional<SetGray> parseSetGray(const std::vector<int>& data);

    /// @brief Attempts to parse a Slur instruction.
    static std::optional<Slur> parseSlur(const std::vector<int>& data);

    /// @brief Attempts to parse a StartGroup instruction.
    static std::optional<StartGroup> parseStartGroup(const std::vector<int>& data);

    /// @brief Attempts to parse a StartObject instruction.
    static std::optional<StartObject> parseStartObject(const std::vector<int>& data);

    /// @brief Attempts to parse a VerticalMode instruction.
    static std::optional<VerticalMode> parseVerticalMode(const std::vector<int>& data);
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

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
class ShapeDefTestAccessor; // used by tests to access private iterator in ShapeDef
#endif

class ShapeInstructionList;
/**
 * @class ShapeDef
 * @brief Represents a shape created in Finale's Shape Designer.
 *
 * This class is identified by the XML node name "shapeDef".
 */
class ShapeDef : public OthersBase
{
private:
    /// @brief Internal iterator function
    bool iterateInstructions(std::function<bool(ShapeDefInstructionType, std::vector<int>)> callback) const;

    friend class ShapeDefTestAccessor; // gives tests access, but users don't see it

public:
    /** @brief Constructor function */
    explicit ShapeDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

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
    /// @return true if all instructions were iterated. False if the callback function exited early.
    bool iterateInstructions(std::function<bool(const ShapeDefInstruction::Decoded&)> callback) const;


    /// @brief Determine if this is a recognized shape type
    /// @return If the shape is recognized, return its known type. Otherwise return std::nullopt.
    std::optional<KnownShapeDefType> recognize() const;

    constexpr static std::string_view XmlNodeName = "shapeDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeDef>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
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
        ShapeDefInstructionType type{};     ///< the type of instruction

        static const xml::XmlElementArray<InstructionInfo>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
    };

    std::vector<std::shared_ptr<InstructionInfo>> instructions; ///< The instructions.

    constexpr static std::string_view XmlNodeName = "shapeList"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeInstructionList>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace musx
