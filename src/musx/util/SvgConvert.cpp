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
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "musx/musx.h"

namespace musx {
namespace util {
namespace {

struct Point
{
    double x{};
    double y{};
};

struct Bounds
{
    void clear()
    {
        hasValue = false;
        minX = minY = maxX = maxY = 0.0;
    }

    void include(const Point& pt)
    {
        if (!hasValue) {
            minX = maxX = pt.x;
            minY = maxY = pt.y;
            hasValue = true;
            return;
        }
        minX = std::min(minX, pt.x);
        maxX = std::max(maxX, pt.x);
        minY = std::min(minY, pt.y);
        maxY = std::max(maxY, pt.y);
    }

    void includeRect(const Point& a, const Point& b)
    {
        include(a);
        include(b);
        include({a.x, b.y});
        include({b.x, a.y});
    }

    void expand(double delta)
    {
        if (!hasValue) {
            return;
        }
        minX -= delta;
        minY -= delta;
        maxX += delta;
        maxY += delta;
    }

    bool hasValue{};
    double minX{};
    double minY{};
    double maxX{};
    double maxY{};
};

class PathBuilder
{
public:
    void clear()
    {
        m_stream.str(std::string{});
        m_stream.clear();
        m_hasData = false;
    }

    bool empty() const
    {
        return !m_hasData;
    }

    void moveTo(const Point& pt)
    {
        appendCommand("M", pt.x, pt.y);
    }

    void lineTo(const Point& pt)
    {
        appendCommand("L", pt.x, pt.y);
    }

    void curveTo(const Point& c1, const Point& c2, const Point& pt)
    {
        appendCommand("C", c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
    }

    void arcTo(double rx, double ry, double xAxisRotation, bool largeArc, bool sweep, const Point& pt)
    {
        if (m_hasData) {
            m_stream << ' ';
        }
        m_stream << 'A' << ' ' << rx << ' ' << ry << ' ' << xAxisRotation << ' '
                 << (largeArc ? 1 : 0) << ' ' << (sweep ? 1 : 0) << ' '
                 << pt.x << ' ' << pt.y;
        m_hasData = true;
    }

    void closePath()
    {
        if (!m_hasData) {
            return;
        }
        m_stream << " Z";
    }

    std::string str() const
    {
        return m_stream.str();
    }

private:
    template <typename... Args>
    void appendCommand(const char* command, Args... args)
    {
        if (m_hasData) {
            m_stream << ' ';
        }
        m_stream << command;
        (appendArg(args), ...);
        m_hasData = true;
    }

    void appendArg(double value)
    {
        m_stream << ' ' << value;
    }

    std::ostringstream m_stream;
    bool m_hasData{};
};

struct PaintState
{
    int gray{0};
    double strokeWidth{1.0};
    std::optional<std::pair<double, double>> dash;
};

struct TextState
{
    dom::FontInfo font;
    bool hasFont{};

    explicit TextState(const dom::DocumentWeakPtr& document)
        : font(document)
    {
    }
};

struct PenState
{
    dom::ShapeDefInstruction::VerticalAlign verticalAlign{dom::ShapeDefInstruction::VerticalAlign::Center};
};

struct ArrowheadState
{
    int startId{};
    int endId{};
    int startFlags{};
    int endFlags{};
};

struct PathDirectionState
{
    bool hasStartDir{};
    bool hasEndDir{};
    Point startDir{};
    Point endDir{};
    Point startPoint{};
    Point endPoint{};
    bool hasSubpath{};
    bool closed{};
};

struct Transform
{
    double a{1.0};
    double b{};
    double c{};
    double d{1.0};
    double tx{};
    double ty{};

    bool isIdentity() const
    {
        return a == 1.0 && b == 0.0 && c == 0.0 && d == 1.0 && tx == 0.0 && ty == 0.0;
    }
};

const Transform kFlipY{1.0, 0.0, 0.0, -1.0, 0.0, 0.0};

double toEvpuDouble(dom::Evpu value)
{
    return static_cast<double>(value);
}

double toEvpuDouble16ths(dom::Evpu16ths value)
{
    return static_cast<double>(value) / 16.0;
}

double toEvpuFromEfix(dom::Efix value)
{
    return static_cast<double>(value) / dom::EFIX_PER_EVPU;
}

std::string grayToRgb(int gray)
{
    int clamped = std::clamp(gray, 0, 100);
    int channel = static_cast<int>(std::lround(255.0 * (static_cast<double>(clamped) / 100.0)));
    return "rgb(" + std::to_string(channel) + "," + std::to_string(channel) + "," + std::to_string(channel) + ")";
}

Transform multiplyTransforms(const Transform& lhs, const Transform& rhs)
{
    Transform result;
    result.a = lhs.a * rhs.a + lhs.c * rhs.b;
    result.b = lhs.b * rhs.a + lhs.d * rhs.b;
    result.c = lhs.a * rhs.c + lhs.c * rhs.d;
    result.d = lhs.b * rhs.c + lhs.d * rhs.d;
    result.tx = lhs.a * rhs.tx + lhs.c * rhs.ty + lhs.tx;
    result.ty = lhs.b * rhs.tx + lhs.d * rhs.ty + lhs.ty;
    return result;
}

Transform makeTranslate(double tx, double ty)
{
    Transform t;
    t.tx = tx;
    t.ty = ty;
    return t;
}

Transform makeScale(double sx, double sy)
{
    Transform t;
    t.a = sx;
    t.d = sy;
    return t;
}

Transform makeRotate(double radians)
{
    Transform t;
    double c = std::cos(radians);
    double s = std::sin(radians);
    t.a = c;
    t.b = s;
    t.c = -s;
    t.d = c;
    return t;
}

Transform makeTranslateRotateScale(const Point& translate, double sx, double sy, double radians)
{
    Transform scale = makeScale(sx, sy);
    Transform rotate = makeRotate(radians);
    Transform translateTransform = makeTranslate(translate.x, translate.y);
    return multiplyTransforms(translateTransform, multiplyTransforms(rotate, scale));
}

Transform makePivotTransform(const Point& pivot, double sx, double sy, double radians)
{
    Transform translateToOrigin = makeTranslate(-pivot.x, -pivot.y);
    Transform scale = makeScale(sx, sy);
    Transform rotate = makeRotate(radians);
    Transform translateBack = makeTranslate(pivot.x, pivot.y);
    return multiplyTransforms(translateBack, multiplyTransforms(rotate, multiplyTransforms(scale, translateToOrigin)));
}

Point applyTransform(const Transform& transform, const Point& pt)
{
    return {
        transform.a * pt.x + transform.c * pt.y + transform.tx,
        transform.b * pt.x + transform.d * pt.y + transform.ty
    };
}

Bounds transformBounds(const Bounds& bounds, const Transform& transform)
{
    Bounds result;
    if (!bounds.hasValue) {
        return result;
    }
    Point p1{bounds.minX, bounds.minY};
    Point p2{bounds.maxX, bounds.minY};
    Point p3{bounds.maxX, bounds.maxY};
    Point p4{bounds.minX, bounds.maxY};
    result.include(applyTransform(transform, p1));
    result.include(applyTransform(transform, p2));
    result.include(applyTransform(transform, p3));
    result.include(applyTransform(transform, p4));
    return result;
}

double decodeRotationRadians(int rotationValue)
{
    constexpr double kTwoPi = 6.28318530717958647692;
    constexpr double kAngleUnits = 4294967296.0;
    uint32_t raw = static_cast<uint32_t>(rotationValue);
    return (static_cast<double>(raw) * kTwoPi) / kAngleUnits;
}

std::string utf8FromCodePoint(char32_t codePoint)
{
    std::string result;
    if (codePoint <= 0x7F) {
        result.push_back(static_cast<char>(codePoint));
    } else if (codePoint <= 0x7FF) {
        result.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
        result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    } else if (codePoint <= 0xFFFF) {
        result.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
        result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    } else {
        result.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
        result.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    return result;
}

Bounds makeTextBounds(const Point& anchor, double fontSizeEvpu, double advanceEvpu)
{
    Bounds result;
    double width = advanceEvpu;
    double top = anchor.y + fontSizeEvpu * 0.8;
    double bottom = anchor.y - fontSizeEvpu * 0.2;
    result.include({anchor.x, bottom});
    result.include({anchor.x + width, top});
    return result;
}

Point computeLineOffset(const Point& start, const Point& end,
                        dom::ShapeDefInstruction::VerticalAlign align,
                        double strokeWidth)
{
    if (align == dom::ShapeDefInstruction::VerticalAlign::Center || strokeWidth == 0.0) {
        return {};
    }
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double length = std::hypot(dx, dy);
    if (length == 0.0) {
        return {};
    }
    double nx = -dy / length;
    double ny = dx / length;
    double halfWidth = strokeWidth / 2.0;
    double sign = (align == dom::ShapeDefInstruction::VerticalAlign::Left) ? 1.0 : -1.0;
    return {nx * halfWidth * sign, ny * halfWidth * sign};
}

void includeLineStrokeBounds(Bounds& bounds, const Point& start, const Point& end, double halfWidth)
{
    if (halfWidth <= 0.0) {
        bounds.include(start);
        bounds.include(end);
        return;
    }
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double length = std::hypot(dx, dy);
    if (length == 0.0) {
        bounds.include({start.x - halfWidth, start.y - halfWidth});
        bounds.include({start.x + halfWidth, start.y + halfWidth});
        return;
    }
    double nx = -dy / length;
    double ny = dx / length;
    Point offset{nx * halfWidth, ny * halfWidth};
    bounds.include({start.x + offset.x, start.y + offset.y});
    bounds.include({start.x - offset.x, start.y - offset.y});
    bounds.include({end.x + offset.x, end.y + offset.y});
    bounds.include({end.x - offset.x, end.y - offset.y});
}

Bounds computeCubicBounds(const Point& p0, const Point& p1, const Point& p2, const Point& p3)
{
    auto evaluate = [](double p0v, double p1v, double p2v, double p3v, double t) {
        double mt = 1.0 - t;
        return mt * mt * mt * p0v
            + 3.0 * mt * mt * t * p1v
            + 3.0 * mt * t * t * p2v
            + t * t * t * p3v;
    };

    auto extrema = [&](double p0v, double p1v, double p2v, double p3v) {
        std::vector<double> values;
        values.reserve(4);
        values.push_back(p0v);
        values.push_back(p3v);

        double a = -p0v + 3.0 * p1v - 3.0 * p2v + p3v;
        double b = 3.0 * p0v - 6.0 * p1v + 3.0 * p2v;
        double c = -3.0 * p0v + 3.0 * p1v;
        constexpr double kEpsilon = 1e-9;
        if (std::abs(a) < kEpsilon) {
            if (std::abs(b) >= kEpsilon) {
                double t = -c / (2.0 * b);
                if (t > 0.0 && t < 1.0) {
                    values.push_back(evaluate(p0v, p1v, p2v, p3v, t));
                }
            }
        } else {
            double disc = b * b - 3.0 * a * c;
            if (disc >= 0.0) {
                double sqrtDisc = std::sqrt(disc);
                double t1 = (-b + sqrtDisc) / (3.0 * a);
                double t2 = (-b - sqrtDisc) / (3.0 * a);
                if (t1 > 0.0 && t1 < 1.0) {
                    values.push_back(evaluate(p0v, p1v, p2v, p3v, t1));
                }
                if (t2 > 0.0 && t2 < 1.0) {
                    values.push_back(evaluate(p0v, p1v, p2v, p3v, t2));
                }
            }
        }
        auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());
        return std::pair<double, double>(*minIt, *maxIt);
    };

    Bounds result;
    auto [minX, maxX] = extrema(p0.x, p1.x, p2.x, p3.x);
    auto [minY, maxY] = extrema(p0.y, p1.y, p2.y, p3.y);
    result.include({minX, minY});
    result.include({maxX, maxY});
    return result;
}

Point evaluateCubic(const Point& p0, const Point& p1, const Point& p2, const Point& p3, double t)
{
    double mt = 1.0 - t;
    double mt2 = mt * mt;
    double t2 = t * t;
    double a = mt2 * mt;
    double b = 3.0 * mt2 * t;
    double c = 3.0 * mt * t2;
    double d = t2 * t;
    return {
        a * p0.x + b * p1.x + c * p2.x + d * p3.x,
        a * p0.y + b * p1.y + c * p2.y + d * p3.y
    };
}

double averageScaleForStroke(const Transform& transform)
{
    double sx = std::hypot(transform.a, transform.b);
    double sy = std::hypot(transform.c, transform.d);
    constexpr double kEpsilon = 1e-9;
    if (sx < kEpsilon && sy < kEpsilon) {
        return 1.0;
    }
    if (sx < kEpsilon) {
        return sy;
    }
    if (sy < kEpsilon) {
        return sx;
    }
    return 0.5 * (sx + sy);
}

Point normalize(const Point& vector)
{
    double length = std::hypot(vector.x, vector.y);
    if (length == 0.0) {
        return {};
    }
    return {vector.x / length, vector.y / length};
}

double arrowheadSizeForPreset(dom::ArrowheadPreset preset)
{
    switch (preset) {
    case dom::ArrowheadPreset::SmallFilled:
    case dom::ArrowheadPreset::SmallOutline:
    case dom::ArrowheadPreset::SmallCurved:
        return dom::EVPU_PER_SPACE * 0.9;
    case dom::ArrowheadPreset::MediumCurved:
        return dom::EVPU_PER_SPACE * 2.5;
    case dom::ArrowheadPreset::LargeCurved:
        return dom::EVPU_PER_SPACE * 4.0;
    }
    return dom::EVPU_PER_SPACE;
}

struct ArrowheadGeometry
{
    std::string path;
    Bounds bounds;
};

ArrowheadGeometry makeArrowheadPath(const Point& tip, const Point& direction, double size, bool curved)
{
    ArrowheadGeometry result;
    Point dir = normalize(direction);
    if (dir.x == 0.0 && dir.y == 0.0) {
        return result;
    }
    Point perp{-dir.y, dir.x};
    Point baseCenter{tip.x - dir.x * size, tip.y - dir.y * size};
    double halfWidth = size / 2.0;
    Point left{baseCenter.x + perp.x * halfWidth, baseCenter.y + perp.y * halfWidth};
    Point right{baseCenter.x - perp.x * halfWidth, baseCenter.y - perp.y * halfWidth};

    std::ostringstream path;
    path << "M " << tip.x << ' ' << tip.y
         << " L " << left.x << ' ' << left.y;
    if (curved) {
        Point control{baseCenter.x + dir.x * (size * 0.35), baseCenter.y + dir.y * (size * 0.35)};
        path << " Q " << control.x << ' ' << control.y << ' ' << right.x << ' ' << right.y;
    } else {
        path << " L " << right.x << ' ' << right.y;
    }
    path << " Z";
    result.path = path.str();
    result.bounds.include(tip);
    result.bounds.include(left);
    result.bounds.include(right);
    return result;
}

} // namespace

std::string SvgConvert::toSvg(const dom::MusxInstance<dom::others::ShapeDef>& shape,
                              GlyphAdvanceFn glyphAdvance,
                              ExternalGraphicFn externalGraphicResolver)
{
    if (!shape) {
        return {};
    }

    std::vector<std::string> elements;
    Bounds bounds;
    PathBuilder path;
    Bounds pathBounds;
    Bounds strokeBounds;
    PaintState paint;
    TextState text(shape->getDocument());
    PenState pen;
    ArrowheadState arrowheads;
    PathDirectionState pathDirection;

    Point current{};
    Point start{};
    Point origin{};
    bool pathRendered = false;
    bool hasDrawCurrent = false;
    Point drawCurrent{};

    std::vector<Point> originStack;
    std::vector<Point> currentStack;
    std::vector<Point> startStack;
    std::vector<Transform> groupTransformStack;
    Transform groupTransform;
    Transform currentTransform;

    auto beginDrawing = [&]() {
        if (pathRendered) {
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            hasDrawCurrent = false;
            pathDirection = {};
        }
    };

    auto emitArrowhead = [&](const dom::ArrowheadPreset preset, const Point& tip, const Point& direction, bool outline) {
        double size = arrowheadSizeForPreset(preset);
        bool curved = (preset == dom::ArrowheadPreset::SmallCurved
            || preset == dom::ArrowheadPreset::MediumCurved
            || preset == dom::ArrowheadPreset::LargeCurved);
        ArrowheadGeometry geometry = makeArrowheadPath(tip, direction, size, curved);
        if (geometry.path.empty()) {
            return;
        }
        Transform outputTransform = multiplyTransforms(kFlipY, currentTransform);
        std::ostringstream element;
        element << "<path d=\"" << geometry.path << "\"";
        if (outline) {
            element << " fill=\"none\"";
            element << " stroke=\"" << grayToRgb(paint.gray) << "\"";
            double strokeWidth = paint.strokeWidth;
            if (!outputTransform.isIdentity()) {
                double scale = averageScaleForStroke(outputTransform);
                if (scale > 1e-9) {
                    strokeWidth = paint.strokeWidth / scale;
                }
            }
            element << " stroke-width=\"" << strokeWidth << "\"";
        } else {
            element << " fill=\"" << grayToRgb(paint.gray) << "\"";
            element << " stroke=\"none\"";
        }
        if (!outputTransform.isIdentity()) {
            element << " transform=\"matrix(" << outputTransform.a << ' ' << outputTransform.b << ' '
                    << outputTransform.c << ' ' << outputTransform.d << ' ' << outputTransform.tx << ' '
                    << outputTransform.ty << ")\"";
        }
        element << "/>";
        elements.push_back(element.str());

        Bounds transformed = transformBounds(geometry.bounds, outputTransform);
        if (outline) {
            transformed.expand(paint.strokeWidth / 2.0);
        }
        if (transformed.hasValue) {
            bounds.include({transformed.minX, transformed.minY});
            bounds.include({transformed.maxX, transformed.maxY});
        }
    };

    auto emitPath = [&](bool fill, bool evenOdd, bool stroke) {
        if (path.empty()) {
            return;
        }

        Transform outputTransform = multiplyTransforms(kFlipY, currentTransform);
        std::ostringstream element;
        element << "<path d=\"" << path.str() << "\"";
        if (fill) {
            element << " fill=\"" << grayToRgb(paint.gray) << "\"";
            if (evenOdd) {
                element << " fill-rule=\"evenodd\"";
            }
        } else {
            element << " fill=\"none\"";
        }

        if (stroke) {
            element << " stroke=\"" << grayToRgb(paint.gray) << "\"";
            double strokeWidth = paint.strokeWidth;
            double dashScale = 1.0;
            if (!outputTransform.isIdentity()) {
                double scale = averageScaleForStroke(outputTransform);
                if (scale > 1e-9) {
                    strokeWidth = paint.strokeWidth / scale;
                    dashScale = scale;
                }
            }
            element << " stroke-width=\"" << strokeWidth << "\"";
            if (paint.dash && (paint.dash->first > 0.0 || paint.dash->second > 0.0)) {
                element << " stroke-dasharray=\"" << (paint.dash->first / dashScale)
                        << ' ' << (paint.dash->second / dashScale) << "\"";
            }
        } else {
            element << " stroke=\"none\"";
        }

        if (!outputTransform.isIdentity()) {
            element << " transform=\"matrix(" << outputTransform.a << ' ' << outputTransform.b << ' '
                    << outputTransform.c << ' ' << outputTransform.d << ' ' << outputTransform.tx << ' '
                    << outputTransform.ty << ")\"";
        }

        element << "/>";
        elements.push_back(element.str());
        Bounds transformed = transformBounds(pathBounds, outputTransform);
        if (stroke) {
            Bounds stroked = transformBounds(strokeBounds, outputTransform);
            if (stroked.hasValue) {
                transformed.include({stroked.minX, stroked.minY});
                transformed.include({stroked.maxX, stroked.maxY});
            }
        }
        if (transformed.hasValue) {
            bounds.include({transformed.minX, transformed.minY});
            bounds.include({transformed.maxX, transformed.maxY});
        }
        if (stroke && pathDirection.hasSubpath && !pathDirection.closed) {
            if (arrowheads.startId >= static_cast<int>(dom::ArrowheadPreset::SmallFilled)
                && arrowheads.startId <= static_cast<int>(dom::ArrowheadPreset::MediumCurved)
                && pathDirection.hasStartDir) {
                auto preset = static_cast<dom::ArrowheadPreset>(arrowheads.startId);
                bool outline = (preset == dom::ArrowheadPreset::SmallOutline);
                Point dir{-pathDirection.startDir.x, -pathDirection.startDir.y};
                emitArrowhead(preset, pathDirection.startPoint, dir, outline);
            }
            if (arrowheads.endId >= static_cast<int>(dom::ArrowheadPreset::SmallFilled)
                && arrowheads.endId <= static_cast<int>(dom::ArrowheadPreset::MediumCurved)
                && pathDirection.hasEndDir) {
                auto preset = static_cast<dom::ArrowheadPreset>(arrowheads.endId);
                bool outline = (preset == dom::ArrowheadPreset::SmallOutline);
                emitArrowhead(preset, pathDirection.endPoint, pathDirection.endDir, outline);
            }
        }
        pathRendered = true;
    };

    auto resolveAdvance = [&](const dom::FontInfo& font, char32_t codePoint, double fontSizeEvpu) -> double {
        if (glyphAdvance) {
            char32_t buffer[1] = { codePoint };
            double advance = glyphAdvance(font, std::u32string_view(buffer, 1));
            if (advance > 0.0) {
                return advance;
            }
        }
        return fontSizeEvpu * 0.6;
    };

    bool unresolvedExternalGraphic = false;

    auto base64Encode = [](const std::vector<std::uint8_t>& data) -> std::string {
        static constexpr char table[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string output;
        output.reserve(((data.size() + 2) / 3) * 4);
        size_t index = 0;
        while (index + 2 < data.size()) {
            uint32_t triple = (static_cast<uint32_t>(data[index]) << 16)
                | (static_cast<uint32_t>(data[index + 1]) << 8)
                | static_cast<uint32_t>(data[index + 2]);
            output.push_back(table[(triple >> 18) & 0x3F]);
            output.push_back(table[(triple >> 12) & 0x3F]);
            output.push_back(table[(triple >> 6) & 0x3F]);
            output.push_back(table[triple & 0x3F]);
            index += 3;
        }
        if (index < data.size()) {
            uint32_t triple = static_cast<uint32_t>(data[index]) << 16;
            bool hasSecond = false;
            if (index + 1 < data.size()) {
                triple |= static_cast<uint32_t>(data[index + 1]) << 8;
                hasSecond = true;
            }
            output.push_back(table[(triple >> 18) & 0x3F]);
            output.push_back(table[(triple >> 12) & 0x3F]);
            output.push_back(hasSecond ? table[(triple >> 6) & 0x3F] : '=');
            output.push_back('=');
        }
        return output;
    };

    shape->iterateInstructions([&](const dom::ShapeDefInstruction::Decoded& inst) -> bool {
        using IT = dom::ShapeDefInstructionType;

        switch (inst.type) {
        case IT::StartObject: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::StartObject>(&inst.data);
            if (data) {
                origin = {};
                double sx = static_cast<double>(data->scaleX) / 1000.0;
                double sy = static_cast<double>(data->scaleY) / 1000.0;
                double radians = decodeRotationRadians(data->rotation);
                Point translate{toEvpuDouble(data->originX), toEvpuDouble(data->originY)};
                Transform localTransform = makeTranslateRotateScale(translate, sx, sy, radians);
                currentTransform = multiplyTransforms(groupTransform, localTransform);
            } else {
                origin = {};
                currentTransform = groupTransform;
            }
            current = {};
            start = current;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            pathDirection = {};
            break;
        }
        case IT::StartGroup: {
            beginDrawing();
            originStack.push_back(origin);
            currentStack.push_back(current);
            startStack.push_back(start);
            groupTransformStack.push_back(groupTransform);
            const auto* data = std::get_if<dom::ShapeDefInstruction::StartGroup>(&inst.data);
            if (data) {
                origin = {};
                double sx = static_cast<double>(data->scaleX) / 1000.0;
                double sy = static_cast<double>(data->scaleY) / 1000.0;
                double radians = decodeRotationRadians(data->rotation);
                Point translate{toEvpuDouble(data->originX), toEvpuDouble(data->originY)};
                Transform localTransform = makeTranslateRotateScale(translate, sx, sy, radians);
                groupTransform = multiplyTransforms(groupTransform, localTransform);
                currentTransform = groupTransform;
            } else {
                origin = {};
                currentTransform = groupTransform;
            }
            current = {};
            start = current;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            pathDirection = {};
            break;
        }
        case IT::EndGroup: {
            beginDrawing();
            if (!originStack.empty()) {
                origin = originStack.back();
                originStack.pop_back();
            }
            if (!currentStack.empty()) {
                current = currentStack.back();
                currentStack.pop_back();
            }
            if (!startStack.empty()) {
                start = startStack.back();
                startStack.pop_back();
            }
            if (!groupTransformStack.empty()) {
                groupTransform = groupTransformStack.back();
                groupTransformStack.pop_back();
            }
            currentTransform = groupTransform;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            pathDirection = {};
            break;
        }
        case IT::GoToOrigin: {
            beginDrawing();
            current = origin;
            start = current;
            path.moveTo(current);
            pathBounds.include(current);
            strokeBounds.include(current);
            drawCurrent = current;
            hasDrawCurrent = true;
            pathDirection = {};
            pathDirection.hasSubpath = true;
            pathDirection.startPoint = current;
            pathDirection.endPoint = current;
            break;
        }
        case IT::GoToStart: {
            beginDrawing();
            current = start;
            path.moveTo(current);
            pathBounds.include(current);
            strokeBounds.include(current);
            drawCurrent = current;
            hasDrawCurrent = true;
            pathDirection = {};
            pathDirection.hasSubpath = true;
            pathDirection.startPoint = current;
            pathDirection.endPoint = current;
            break;
        }
        case IT::RMoveTo: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::RMoveTo>(&inst.data);
            if (data) {
                current.x += toEvpuDouble(data->dx);
                current.y += toEvpuDouble(data->dy);
                start = current;
                path.moveTo(current);
                pathBounds.include(current);
                strokeBounds.include(current);
                drawCurrent = current;
                hasDrawCurrent = true;
                pathDirection = {};
                pathDirection.hasSubpath = true;
                pathDirection.startPoint = current;
                pathDirection.endPoint = current;
            }
            break;
        }
        case IT::RLineTo: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::RLineTo>(&inst.data);
            if (data) {
                Point startPoint = current;
                current.x += toEvpuDouble(data->dx);
                current.y += toEvpuDouble(data->dy);
                Point endPoint = current;
                Point offset = computeLineOffset(startPoint, endPoint, pen.verticalAlign, paint.strokeWidth);
                Point drawStart{startPoint.x + offset.x, startPoint.y + offset.y};
                Point drawEnd{endPoint.x + offset.x, endPoint.y + offset.y};
                if (path.empty() || !hasDrawCurrent) {
                    path.moveTo(drawStart);
                    pathBounds.include(drawStart);
                    strokeBounds.include(drawStart);
                    drawCurrent = drawStart;
                    hasDrawCurrent = true;
                } else {
                    double delta = std::hypot(drawCurrent.x - drawStart.x, drawCurrent.y - drawStart.y);
                    if (delta > 1e-6) {
                        path.moveTo(drawStart);
                        pathBounds.include(drawStart);
                        strokeBounds.include(drawStart);
                        drawCurrent = drawStart;
                    }
                }
                path.lineTo(drawEnd);
                pathBounds.include(drawEnd);
                includeLineStrokeBounds(strokeBounds, drawStart, drawEnd, paint.strokeWidth / 2.0);
                drawCurrent = drawEnd;
                pathDirection.hasSubpath = true;
                if (!pathDirection.hasStartDir) {
                    pathDirection.startDir = normalize({drawEnd.x - drawStart.x, drawEnd.y - drawStart.y});
                    pathDirection.hasStartDir = true;
                    pathDirection.startPoint = drawStart;
                }
                pathDirection.endDir = normalize({drawEnd.x - drawStart.x, drawEnd.y - drawStart.y});
                pathDirection.hasEndDir = true;
                pathDirection.endPoint = drawEnd;
            }
            break;
        }
        case IT::CurveTo: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::CurveTo>(&inst.data);
            if (data) {
                Point startPoint = current;
                double c1dx = toEvpuDouble(data->c1dx);
                double c1dy = toEvpuDouble(data->c1dy);
                double c2dx = toEvpuDouble(data->c2dx);
                double c2dy = toEvpuDouble(data->c2dy);
                double edx = toEvpuDouble(data->edx);
                double edy = toEvpuDouble(data->edy);
                Point c1{current.x + c1dx, current.y + c1dy};
                Point c2{current.x + c1dx + c2dx, current.y + c1dy + c2dy};
                current.x += c1dx + c2dx + edx;
                current.y += c1dy + c2dy + edy;
                if (path.empty()) {
                    path.moveTo(startPoint);
                    pathBounds.include(startPoint);
                    strokeBounds.include(startPoint);
                }
                path.curveTo(c1, c2, current);
                Bounds curveBounds = computeCubicBounds(startPoint, c1, c2, current);
                pathBounds.include({curveBounds.minX, curveBounds.minY});
                pathBounds.include({curveBounds.maxX, curveBounds.maxY});
                {
                    constexpr int kCurveSamples = 24;
                    Point prev = startPoint;
                    for (int i = 1; i <= kCurveSamples; ++i) {
                        double t = static_cast<double>(i) / static_cast<double>(kCurveSamples);
                        Point next = evaluateCubic(startPoint, c1, c2, current, t);
                        includeLineStrokeBounds(strokeBounds, prev, next, paint.strokeWidth / 2.0);
                        prev = next;
                    }
                }
                drawCurrent = current;
                hasDrawCurrent = true;
                pathDirection.hasSubpath = true;
                if (!pathDirection.hasStartDir) {
                    pathDirection.startDir = normalize({c1.x - startPoint.x, c1.y - startPoint.y});
                    pathDirection.hasStartDir = true;
                    pathDirection.startPoint = startPoint;
                }
                pathDirection.endDir = normalize({current.x - c2.x, current.y - c2.y});
                pathDirection.hasEndDir = true;
                pathDirection.endPoint = current;
            }
            break;
        }
        case IT::Slur: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::Slur>(&inst.data);
            if (data) {
                Point startPoint = current;
                double c1dx = toEvpuDouble16ths(data->c1dx);
                double c1dy = toEvpuDouble16ths(data->c1dy);
                double c2dx = toEvpuDouble16ths(data->c2dx);
                double c2dy = toEvpuDouble16ths(data->c2dy);
                double edx = toEvpuDouble16ths(data->edx);
                double edy = toEvpuDouble16ths(data->edy);
                Point c1{current.x + c1dx, current.y + c1dy};
                Point c2{current.x + c1dx + c2dx, current.y + c1dy + c2dy};
                current.x += c1dx + c2dx + edx;
                current.y += c1dy + c2dy + edy;
                if (path.empty()) {
                    path.moveTo(startPoint);
                    pathBounds.include(startPoint);
                    strokeBounds.include(startPoint);
                }
                path.curveTo(c1, c2, current);
                Bounds curveBounds = computeCubicBounds(startPoint, c1, c2, current);
                pathBounds.include({curveBounds.minX, curveBounds.minY});
                pathBounds.include({curveBounds.maxX, curveBounds.maxY});
                {
                    constexpr int kCurveSamples = 24;
                    Point prev = startPoint;
                    for (int i = 1; i <= kCurveSamples; ++i) {
                        double t = static_cast<double>(i) / static_cast<double>(kCurveSamples);
                        Point next = evaluateCubic(startPoint, c1, c2, current, t);
                        includeLineStrokeBounds(strokeBounds, prev, next, paint.strokeWidth / 2.0);
                        prev = next;
                    }
                }
                drawCurrent = current;
                hasDrawCurrent = true;
                pathDirection.hasSubpath = true;
                if (!pathDirection.hasStartDir) {
                    pathDirection.startDir = normalize({c1.x - startPoint.x, c1.y - startPoint.y});
                    pathDirection.hasStartDir = true;
                    pathDirection.startPoint = startPoint;
                }
                pathDirection.endDir = normalize({current.x - c2.x, current.y - c2.y});
                pathDirection.hasEndDir = true;
                pathDirection.endPoint = current;
            }
            break;
        }
        case IT::Rectangle: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::Rectangle>(&inst.data);
            if (data) {
                Point p0 = current;
                Point p1{current.x + toEvpuDouble(data->width), current.y};
                Point p2{current.x + toEvpuDouble(data->width), current.y + toEvpuDouble(data->height)};
                Point p3{current.x, current.y + toEvpuDouble(data->height)};
                path.moveTo(p0);
                path.lineTo(p1);
                path.lineTo(p2);
                path.lineTo(p3);
                path.closePath();
                pathBounds.includeRect(p0, p2);
                {
                    Bounds rectBounds;
                    rectBounds.includeRect(p0, p2);
                    rectBounds.expand(paint.strokeWidth / 2.0);
                    strokeBounds.include({rectBounds.minX, rectBounds.minY});
                    strokeBounds.include({rectBounds.maxX, rectBounds.maxY});
                }
                current = p0;
                start = p0;
                drawCurrent = p0;
                hasDrawCurrent = true;
                pathDirection.hasSubpath = true;
                pathDirection.closed = true;
            }
            break;
        }
        case IT::Ellipse: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::Ellipse>(&inst.data);
            if (data) {
                double width = toEvpuDouble(data->width);
                double height = toEvpuDouble(data->height);
                Point center{current.x + (width / 2.0), current.y + (height / 2.0)};
                double rx = std::abs(width / 2.0);
                double ry = std::abs(height / 2.0);
                Point startPt{center.x + rx, center.y};
                Point midPt{center.x - rx, center.y};
                path.moveTo(startPt);
                path.arcTo(rx, ry, 0.0, true, false, midPt);
                path.arcTo(rx, ry, 0.0, true, false, startPt);
                path.closePath();
                pathBounds.include({center.x - rx, center.y - ry});
                pathBounds.include({center.x + rx, center.y + ry});
                {
                    Bounds ellipseBounds;
                    ellipseBounds.include({center.x - rx, center.y - ry});
                    ellipseBounds.include({center.x + rx, center.y + ry});
                    ellipseBounds.expand(paint.strokeWidth / 2.0);
                    strokeBounds.include({ellipseBounds.minX, ellipseBounds.minY});
                    strokeBounds.include({ellipseBounds.maxX, ellipseBounds.maxY});
                }
                current = startPt;
                start = startPt;
                drawCurrent = startPt;
                hasDrawCurrent = true;
                pathDirection.hasSubpath = true;
                pathDirection.closed = true;
            }
            break;
        }
        case IT::ClosePath:
            beginDrawing();
            path.closePath();
            current = start;
            drawCurrent = current;
            hasDrawCurrent = true;
            pathDirection.closed = true;
            break;
        case IT::FillSolid:
            emitPath(true, false, false);
            break;
        case IT::FillAlt:
            emitPath(true, true, false);
            break;
        case IT::Stroke:
            emitPath(false, false, true);
            break;
        case IT::LineWidth: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::LineWidth>(&inst.data);
            if (data) {
                paint.strokeWidth = toEvpuFromEfix(data->efix);
            }
            break;
        }
        case IT::SetDash: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::SetDash>(&inst.data);
            if (data) {
                paint.dash = {toEvpuFromEfix(data->dashLength), toEvpuFromEfix(data->spaceLength)};
            }
            break;
        }
        case IT::SetArrowhead: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::SetArrowhead>(&inst.data);
            if (data) {
                arrowheads.startId = data->startArrowId;
                arrowheads.endId = data->endArrowId;
                arrowheads.startFlags = data->startFlags;
                arrowheads.endFlags = data->endFlags;
            }
            break;
        }
        case IT::VerticalMode: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::VerticalMode>(&inst.data);
            if (data) {
                pen.verticalAlign = data->mode;
            }
            break;
        }
        case IT::ExternalGraphic: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::ExternalGraphic>(&inst.data);
            if (!data || !externalGraphicResolver) {
                unresolvedExternalGraphic = true;
                return false;
            }
            ExternalGraphicInfo info{data->width, data->height, data->cmper};
            auto payload = externalGraphicResolver(info);
            if (!payload || payload->bytes.empty() || payload->mimeType.empty()) {
                unresolvedExternalGraphic = true;
                return false;
            }
            double width = toEvpuDouble(data->width);
            double height = toEvpuDouble(data->height);
            double x = current.x;
            double y = current.y - height;
            std::string encoded = base64Encode(payload->bytes);
            Transform outputTransform = multiplyTransforms(kFlipY, currentTransform);
            std::ostringstream element;
            element << "<image x=\"" << x << "\" y=\"" << y << "\" width=\"" << width
                    << "\" height=\"" << height << "\" href=\"data:" << payload->mimeType
                    << ";base64," << encoded << "\"";
            if (!outputTransform.isIdentity()) {
                element << " transform=\"matrix(" << outputTransform.a << ' ' << outputTransform.b << ' '
                        << outputTransform.c << ' ' << outputTransform.d << ' ' << outputTransform.tx << ' '
                        << outputTransform.ty << ")\"";
            }
            element << "/>";
            elements.push_back(element.str());

            Bounds localBounds;
            localBounds.includeRect({x, y}, {x + width, y + height});
            Bounds transformed = transformBounds(localBounds, outputTransform);
            if (transformed.hasValue) {
                bounds.include({transformed.minX, transformed.minY});
                bounds.include({transformed.maxX, transformed.maxY});
            }
            break;
        }
        case IT::SetFont: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::SetFont>(&inst.data);
            if (data) {
                text.font = data->font;
                text.hasFont = true;
            }
            break;
        }
        case IT::DrawChar: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::DrawChar>(&inst.data);
            if (!data || (text.hasFont && text.font.hidden)) {
                break;
            }
            dom::FontInfo currentFont = text.font;
            std::string fontName;
            try {
                fontName = currentFont.getName();
            } catch (...) {
                fontName.clear();
            }
            int fontSizePoints = currentFont.fontSize > 0 ? currentFont.fontSize : 12;
            double fontSizeEvpu = fontSizePoints * dom::EVPU_PER_POINT;
            Point anchor = current;
            double advanceEvpu = resolveAdvance(currentFont, data->codePoint, fontSizeEvpu);
            std::ostringstream element;
            element << "<text x=\"" << anchor.x << "\" y=\"" << anchor.y << "\"";
            if (!fontName.empty()) {
                element << " font-family=\"" << fontName << "\"";
            }
            element << " font-size=\"" << fontSizeEvpu << "\"";
            element << " font-weight=\"" << (currentFont.bold ? "bold" : "normal") << "\"";
            element << " font-style=\"" << (currentFont.italic ? "italic" : "normal") << "\"";
            if (currentFont.underline || currentFont.strikeout) {
                element << " text-decoration=\"";
                if (currentFont.underline) {
                    element << "underline";
                }
                if (currentFont.underline && currentFont.strikeout) {
                    element << ' ';
                }
                if (currentFont.strikeout) {
                    element << "line-through";
                }
                element << "\"";
            } else {
                element << " text-decoration=\"none\"";
            }
            element << " fill=\"" << grayToRgb(paint.gray) << "\"";
            Transform outputTransform = multiplyTransforms(kFlipY, currentTransform);
            if (!outputTransform.isIdentity()) {
                element << " transform=\"matrix(" << outputTransform.a << ' ' << outputTransform.b << ' '
                        << outputTransform.c << ' ' << outputTransform.d << ' ' << outputTransform.tx << ' '
                        << outputTransform.ty << ")\"";
            }
            element << ">";
            element << utf8FromCodePoint(data->codePoint);
            element << "</text>";
            elements.push_back(element.str());

            Bounds localBounds = makeTextBounds(anchor, fontSizeEvpu, advanceEvpu);
            Bounds transformed = transformBounds(localBounds, outputTransform);
            if (transformed.hasValue) {
                bounds.include({transformed.minX, transformed.minY});
                bounds.include({transformed.maxX, transformed.maxY});
            }
            break;
        }
        case IT::CloneChar: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::CloneChar>(&inst.data);
            if (!data || (text.hasFont && text.font.hidden)) {
                break;
            }
            dom::FontInfo currentFont = text.font;
            std::string fontName;
            try {
                fontName = currentFont.getName();
            } catch (...) {
                fontName.clear();
            }
            int fontSizePoints = currentFont.fontSize > 0 ? currentFont.fontSize : 12;
            double fontSizeEvpu = fontSizePoints * dom::EVPU_PER_POINT;
            double baselineShift = (static_cast<double>(data->baselineShift) / 1000.0) * fontSizeEvpu;
            std::string glyph = utf8FromCodePoint(data->codePoint);
            double dx = toEvpuDouble(data->dx);
            double dy = toEvpuDouble(data->dy);
            double length = std::hypot(dx, dy);
            double advanceEvpu = resolveAdvance(currentFont, data->codePoint, fontSizeEvpu);
            double step = advanceEvpu;
            if (step <= 0.0) {
                step = length;
            }
            int repeats = (step > 0.0) ? std::max(1, static_cast<int>(std::floor(length / step)) + 1) : 1;
            double ux = length > 0.0 ? (dx / length) : 0.0;
            double uy = length > 0.0 ? (dy / length) : 0.0;

            for (int i = 0; i < repeats; ++i) {
                double offset = (step > 0.0) ? (i * step) : 0.0;
                if (offset > length && i > 0) {
                    break;
                }
                Point anchor{current.x + ux * offset, current.y + uy * offset + baselineShift};
                std::ostringstream element;
                element << "<text x=\"" << anchor.x << "\" y=\"" << anchor.y << "\"";
                if (!fontName.empty()) {
                    element << " font-family=\"" << fontName << "\"";
                }
                element << " font-size=\"" << fontSizeEvpu << "\"";
            element << " font-weight=\"" << (currentFont.bold ? "bold" : "normal") << "\"";
            element << " font-style=\"" << (currentFont.italic ? "italic" : "normal") << "\"";
            if (currentFont.underline || currentFont.strikeout) {
                element << " text-decoration=\"";
                if (currentFont.underline) {
                    element << "underline";
                }
                if (currentFont.underline && currentFont.strikeout) {
                    element << ' ';
                }
                if (currentFont.strikeout) {
                    element << "line-through";
                }
                element << "\"";
            } else {
                element << " text-decoration=\"none\"";
            }
            element << " fill=\"" << grayToRgb(paint.gray) << "\"";
            Transform outputTransform = multiplyTransforms(kFlipY, currentTransform);
            if (!outputTransform.isIdentity()) {
                element << " transform=\"matrix(" << outputTransform.a << ' ' << outputTransform.b << ' '
                        << outputTransform.c << ' ' << outputTransform.d << ' ' << outputTransform.tx << ' '
                        << outputTransform.ty << ")\"";
            }
            element << ">" << glyph << "</text>";
            elements.push_back(element.str());

            Bounds localBounds = makeTextBounds(anchor, fontSizeEvpu, advanceEvpu);
            Bounds transformed = transformBounds(localBounds, outputTransform);
            if (transformed.hasValue) {
                bounds.include({transformed.minX, transformed.minY});
                bounds.include({transformed.maxX, transformed.maxY});
            }
            }

            current.x += toEvpuDouble(data->dx);
            current.y += toEvpuDouble(data->dy);
            break;
        }
        case IT::SetGray: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::SetGray>(&inst.data);
            if (data) {
                paint.gray = data->gray;
            }
            break;
        }
        case IT::SetBlack:
            paint.gray = 0;
            break;
        case IT::SetWhite:
            paint.gray = 100;
            break;
        default:
            break;
        }

        return true;
    });

    if (unresolvedExternalGraphic) {
        return {};
    }

    std::ostringstream svg;
    if (bounds.hasValue) {
        double minX = std::round(bounds.minX);
        double minY = std::floor(bounds.minY);
        double maxX = std::round(bounds.maxX);
        double maxY = (bounds.maxY < 0.0) ? std::floor(bounds.maxY) : std::ceil(bounds.maxY);
        double width = maxX - minX;
        double height = maxY - minY;
        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\""
            << minX << ' ' << minY << ' ' << width << ' ' << height << "\"";
        if (width > 0.0 && height > 0.0) {
            svg << " width=\"" << width << "\" height=\"" << height << "\"";
        }
        svg << '>';
    } else {
        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 0 0\">";
    }

    for (const auto& element : elements) {
        svg << element;
    }
    svg << "</svg>";
    return svg.str();
}

} // namespace util
} // namespace musx
