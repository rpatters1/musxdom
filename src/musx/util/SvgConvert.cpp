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
#include <limits>
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
    double strokeWidth{4.0};
    std::optional<std::pair<double, double>> dash;
};

struct EllipseState
{
    Point center;
    double rx{};
    double ry{};
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
    constexpr double kPi = 3.14159265358979323846;
    uint32_t raw = static_cast<uint32_t>(rotationValue);
    uint32_t quadrant = raw >> 30;
    uint32_t fine = raw & 0x3FFFFFFFu;
    double degrees = static_cast<double>(quadrant) * 90.0;
    if (fine != 0) {
        degrees -= static_cast<double>(fine) / 16.0;
    }
    return degrees * (kPi / 180.0);
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

void includeLineJoinBounds(Bounds& bounds,
                           const Point& prevStart,
                           const Point& prevEnd,
                           const Point& nextEnd,
                           const Point& center,
                           double halfWidth)
{
    Point v0{prevEnd.x - prevStart.x, prevEnd.y - prevStart.y};
    Point v1{nextEnd.x - prevEnd.x, nextEnd.y - prevEnd.y};
    double len0 = std::hypot(v0.x, v0.y);
    double len1 = std::hypot(v1.x, v1.y);
    if (len0 == 0.0 || len1 == 0.0) {
        return;
    }
    v0.x /= len0;
    v0.y /= len0;
    v1.x /= len1;
    v1.y /= len1;
    Point n0{-v0.y, v0.x};
    Point n1{-v1.y, v1.x};
    Point miter{n0.x + n1.x, n0.y + n1.y};
    double miterLen = std::hypot(miter.x, miter.y);
    if (miterLen == 0.0) {
        return;
    }
    miter.x /= miterLen;
    miter.y /= miterLen;
    Point toCenter{center.x - prevEnd.x, center.y - prevEnd.y};
    if ((miter.x * toCenter.x + miter.y * toCenter.y) > 0.0) {
        miter.x = -miter.x;
        miter.y = -miter.y;
    }
    double denom = miter.x * n1.x + miter.y * n1.y;
    if (std::abs(denom) < 1e-6) {
        return;
    }
    double length = halfWidth / denom;
    constexpr double kMiterLimit = 14.0;
    double maxLength = halfWidth * kMiterLimit;
    if (length > maxLength) {
        length = maxLength;
    } else if (length < -maxLength) {
        length = -maxLength;
    }
    length = std::abs(length);
    if (length <= halfWidth) {
        return;
    }
    bounds.include({prevEnd.x + miter.x * length, prevEnd.y + miter.y * length});
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
                              GlyphMetricsFn glyphMetrics,
                              ExternalGraphicFn externalGraphicResolver)
{
    if (!shape) {
        return {};
    }
    const int debugShapeId = shape->getCmper();
    const bool debugShape = (debugShapeId == 9 || debugShapeId == 113);

    std::vector<std::string> elements;
    Bounds bounds;
    PaintState paint;
    auto document = shape->getDocument();
    TextState text(document);
    PenState pen;
    bool lineWidthSet = false;

    PathBuilder path;
    std::optional<dom::ShapeDefInstruction::StartObject> lastStartObject;
    std::optional<dom::ShapeDefInstruction::RMoveTo> lastMove;
    std::optional<int> currentRawScaleX;
    std::optional<int> currentRawScaleY;
    Bounds pathBounds;
    Bounds strokeBounds;
    bool pathRendered = false;
    std::optional<Point> firstLineStartWorld;
    std::optional<Point> firstLineEndWorld;
    std::optional<Point> lastLineStartWorld;
    std::optional<Point> lastLineEndWorld;
    std::optional<EllipseState> pendingEllipse;

    Point current{};
    Point start{};
    double textLineStartX = 0.0;
    Point origin{};

    std::vector<Point> originStack;
    std::vector<Point> currentStack;
    std::vector<Point> startStack;
    std::vector<Transform> groupTransformStack;
    Transform groupTransform;
    Transform currentTransform;
    Transform currentGroupTransform;
    std::optional<Bounds> currentObjectBounds;
    double currentRotationRadians = 0.0;

    auto toWorld = [&](const Point& local) {
        Point world = applyTransform(currentTransform, local);
        world.y = -world.y;
        return world;
    };

    auto beginDrawing = [&]() {
        if (pathRendered) {
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            firstLineStartWorld.reset();
            firstLineEndWorld.reset();
            lastLineStartWorld.reset();
            lastLineEndWorld.reset();
        }
    };

    auto resetLineTracking = [&]() {
        firstLineStartWorld.reset();
        firstLineEndWorld.reset();
        lastLineStartWorld.reset();
        lastLineEndWorld.reset();
    };

    auto resetEllipse = [&]() {
        pendingEllipse.reset();
    };

    auto emitPath = [&](bool fill, bool evenOdd, bool stroke) {
        if (path.empty()) {
            return;
        }

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
            element << " stroke-width=\"" << paint.strokeWidth << "\"";
        } else {
            element << " stroke=\"none\"";
        }
        element << "/>";
        elements.push_back(element.str());

        Bounds combined = pathBounds;
        if (stroke && strokeBounds.hasValue) {
            combined.include({strokeBounds.minX, strokeBounds.minY});
            combined.include({strokeBounds.maxX, strokeBounds.maxY});
        }
        if (combined.hasValue) {
            bounds.include({combined.minX, combined.minY});
            bounds.include({combined.maxX, combined.maxY});
        }
        pathRendered = true;
    };

    auto resolveMetrics = [&](const dom::FontInfo& font,
                              char32_t codePoint,
                              double fontSizeEvpu) -> GlyphMetrics {
        if (glyphMetrics) {
            char32_t buffer[1] = { codePoint };
            if (auto metrics = glyphMetrics(font, std::u32string_view(buffer, 1)); metrics) {
                return *metrics;
            }
        }
        GlyphMetrics fallback;
        fallback.advance = fontSizeEvpu * 0.6;
        fallback.ascent = fontSizeEvpu * 0.8;
        fallback.descent = fontSizeEvpu * 0.2;
        return fallback;
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

        if (debugShape) {
            std::cout << "[Shape " << debugShapeId << "] Inst type=" << static_cast<int>(inst.type) << '\n';
        }

        switch (inst.type) {
        case IT::StartObject: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::StartObject>(&inst.data);
            if (data) {
                lastStartObject = *data;
                lastMove.reset();
                currentRawScaleX = data->scaleX;
                currentRawScaleY = data->scaleY;
                origin = {};
                auto isSentinel = [](dom::Evpu value) {
                    return value == (std::numeric_limits<dom::Evpu>::min)()
                        || value == (std::numeric_limits<dom::Evpu>::max)();
                };
                double sx = (data->scaleX == 0) ? 1.0 : (static_cast<double>(data->scaleX) / 1000.0);
                double sy = (data->scaleY == 0) ? 1.0 : (static_cast<double>(data->scaleY) / 1000.0);
                double radians = decodeRotationRadians(data->rotation);
                currentRotationRadians = radians;
                Point translate{toEvpuDouble(data->originX), toEvpuDouble(data->originY)};
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] StartObject origin=(" << data->originX << "," << data->originY
                              << ") bounds=(" << data->left << "," << data->top << "," << data->right << ","
                              << data->bottom << ") scale=(" << data->scaleX << "," << data->scaleY
                              << ") rotation=" << data->rotation << " radians=" << radians << '\n';
                }
                Transform localTransform = makeTranslateRotateScale(translate, sx, sy, radians);
                currentTransform = multiplyTransforms(groupTransform, localTransform);
                currentGroupTransform = groupTransform;
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] StartObject transform"
                              << " a=" << currentTransform.a << " b=" << currentTransform.b
                              << " c=" << currentTransform.c << " d=" << currentTransform.d
                              << " tx=" << currentTransform.tx << " ty=" << currentTransform.ty << '\n';
                }
                if (isSentinel(data->left) || isSentinel(data->right)
                    || isSentinel(data->top) || isSentinel(data->bottom)) {
                    currentObjectBounds.reset();
                } else {
                    Bounds objBounds;
                    objBounds.include({toEvpuDouble(data->left), toEvpuDouble(data->bottom)});
                    objBounds.include({toEvpuDouble(data->right), toEvpuDouble(data->top)});
                    currentObjectBounds = objBounds.hasValue ? std::optional<Bounds>(objBounds) : std::nullopt;
                }
            } else {
                lastStartObject.reset();
                lastMove.reset();
                currentRawScaleX.reset();
                currentRawScaleY.reset();
                origin = {};
                currentTransform = groupTransform;
                currentGroupTransform = groupTransform;
                currentObjectBounds.reset();
            }
            current = {};
            start = current;
            textLineStartX = current.x;
            lineWidthSet = false;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            resetLineTracking();
            resetEllipse();
            break;
        }
        case IT::StartGroup: {
            beginDrawing();
            originStack.push_back(origin);
            currentStack.push_back(current);
            startStack.push_back(start);
            groupTransformStack.push_back(groupTransform);
            lastStartObject.reset();
            lastMove.reset();
            currentRawScaleX.reset();
            currentRawScaleY.reset();
            const auto* data = std::get_if<dom::ShapeDefInstruction::StartGroup>(&inst.data);
            if (data) {
                origin = {};
                double sx = (data->scaleX == 0) ? 1.0 : (static_cast<double>(data->scaleX) / 1000.0);
                double sy = (data->scaleY == 0) ? 1.0 : (static_cast<double>(data->scaleY) / 1000.0);
                double radians = decodeRotationRadians(data->rotation);
                Point translate{toEvpuDouble(data->originX), toEvpuDouble(data->originY)};
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] StartGroup origin=(" << data->originX << "," << data->originY
                              << ") bounds=(" << data->left << "," << data->top << "," << data->right << ","
                              << data->bottom << ") scale=(" << data->scaleX << "," << data->scaleY
                              << ") rotation=" << data->rotation << " radians=" << radians << '\n';
                }
                Transform localTransform = makeTranslateRotateScale(translate, sx, sy, radians);
                groupTransform = multiplyTransforms(groupTransform, localTransform);
                currentTransform = groupTransform;
            } else {
                origin = {};
                currentTransform = groupTransform;
            }
            current = {};
            start = current;
            textLineStartX = current.x;
            lineWidthSet = false;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            resetLineTracking();
            resetEllipse();
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
            textLineStartX = current.x;
            lineWidthSet = false;
            resetLineTracking();
            resetEllipse();
            lastStartObject.reset();
            lastMove.reset();
            currentRawScaleX.reset();
            currentRawScaleY.reset();
            break;
        }
        case IT::GoToOrigin: {
            beginDrawing();
            current = origin;
            start = current;
            textLineStartX = current.x;
            Point world = toWorld(current);
            path.moveTo(world);
            pathBounds.include(world);
            strokeBounds.include(world);
            resetLineTracking();
            resetEllipse();
            lastMove.reset();
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] GoToOrigin world=(" << world.x << "," << world.y << ")\n";
            }
            break;
        }
        case IT::GoToStart: {
            beginDrawing();
            current = start;
            textLineStartX = current.x;
            Point world = toWorld(current);
            path.moveTo(world);
            pathBounds.include(world);
            strokeBounds.include(world);
            resetLineTracking();
            resetEllipse();
            lastMove.reset();
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] GoToStart world=(" << world.x << "," << world.y << ")\n";
            }
            break;
        }
        case IT::RMoveTo: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::RMoveTo>(&inst.data);
            if (data) {
                lastMove = *data;
                current.x += toEvpuDouble(data->dx);
                current.y += toEvpuDouble(data->dy);
                start = current;
                textLineStartX = current.x;
                Point world = toWorld(current);
                path.moveTo(world);
                pathBounds.include(world);
                strokeBounds.include(world);
                resetLineTracking();
                resetEllipse();
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] RMoveTo dx=" << data->dx << " dy=" << data->dy
                              << " world=(" << world.x << "," << world.y << ")\n";
                }
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
                Point worldStart = toWorld(drawStart);
                Point worldEnd = toWorld(drawEnd);
                if (path.empty()) {
                    path.moveTo(worldStart);
                    pathBounds.include(worldStart);
                    strokeBounds.include(worldStart);
                    resetLineTracking();
                }
                path.lineTo(worldEnd);
                pathBounds.include(worldEnd);
                includeLineStrokeBounds(strokeBounds, worldStart, worldEnd, paint.strokeWidth / 2.0);
                double segmentLen = std::hypot(worldEnd.x - worldStart.x, worldEnd.y - worldStart.y);
                if (segmentLen > 0.0) {
                if (pathBounds.hasValue && lastLineStartWorld && lastLineEndWorld) {
                    Point center{(pathBounds.minX + pathBounds.maxX) / 2.0,
                                 (pathBounds.minY + pathBounds.maxY) / 2.0};
                    includeLineJoinBounds(strokeBounds, *lastLineStartWorld, *lastLineEndWorld, worldEnd,
                                          center, paint.strokeWidth / 2.0);
                }
                    if (!firstLineStartWorld || !firstLineEndWorld) {
                        firstLineStartWorld = worldStart;
                        firstLineEndWorld = worldEnd;
                    }
                    lastLineStartWorld = worldStart;
                    lastLineEndWorld = worldEnd;
                }
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] RLineTo dx=" << data->dx << " dy=" << data->dy
                              << " worldStart=(" << worldStart.x << "," << worldStart.y
                              << ") worldEnd=(" << worldEnd.x << "," << worldEnd.y << ")\n";
                }
            }
            break;
        }
        case IT::CurveTo: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::CurveTo>(&inst.data);
            if (data) {
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] Curve raw c1=(" << data->c1dx << "," << data->c1dy
                              << ") c2=(" << data->c2dx << "," << data->c2dy << ") end=("
                              << data->edx << "," << data->edy << ")\n";
                }
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
                Point worldStart = toWorld(startPoint);
                Point worldC1 = toWorld(c1);
                Point worldC2 = toWorld(c2);
                Point worldEnd = toWorld(current);
                if (debugShape) {
                    double dx = worldEnd.x - worldStart.x;
                    double dy = worldEnd.y - worldStart.y;
                    double endAngle = std::atan2(dy, dx);
                    double delta = endAngle + currentRotationRadians;
                    std::cout << "[Shape " << debugShapeId << "] Curve endAngle=" << endAngle
                              << " delta=" << delta << '\n';
                    std::cout << "[Shape " << debugShapeId << "] Curve start=(" << worldStart.x << "," << worldStart.y
                              << ") end=(" << worldEnd.x << "," << worldEnd.y << ") c1=(" << worldC1.x << ","
                              << worldC1.y << ") c2=(" << worldC2.x << "," << worldC2.y << ")\n";
                }
                if (path.empty()) {
                    path.moveTo(worldStart);
                    pathBounds.include(worldStart);
                    strokeBounds.include(worldStart);
                }
                path.curveTo(worldC1, worldC2, worldEnd);
                Bounds curveBounds = computeCubicBounds(worldStart, worldC1, worldC2, worldEnd);
                pathBounds.include({curveBounds.minX, curveBounds.minY});
                pathBounds.include({curveBounds.maxX, curveBounds.maxY});
                constexpr int kCurveSamples = 24;
                Point prev = worldStart;
                for (int i = 1; i <= kCurveSamples; ++i) {
                    double t = static_cast<double>(i) / static_cast<double>(kCurveSamples);
                    Point next = evaluateCubic(worldStart, worldC1, worldC2, worldEnd, t);
                    includeLineStrokeBounds(strokeBounds, prev, next, paint.strokeWidth / 2.0);
                    prev = next;
                }
            }
            break;
        }
        case IT::Slur: {
            beginDrawing();
            const auto* data = std::get_if<dom::ShapeDefInstruction::Slur>(&inst.data);
            if (data) {
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] Slur raw c1=(" << data->c1dx << "," << data->c1dy
                              << ") c2=(" << data->c2dx << "," << data->c2dy << ") end=("
                              << data->edx << "," << data->edy << ")\n";
                }
                Point startPoint = current;
                double c1dx = toEvpuDouble(data->c1dx);
                double c1dy = toEvpuDouble(data->c1dy);
                double c2dx = toEvpuDouble(data->c2dx);
                double c2dy = toEvpuDouble(data->c2dy);
                double edx = toEvpuDouble(data->edx);
                double edy = toEvpuDouble(data->edy);
                if (currentRawScaleX && currentRawScaleY
                    && (currentRawScaleX.value() == 0 || currentRawScaleY.value() == 0)
                    && currentObjectBounds && currentObjectBounds->hasValue) {
                    const auto& obj = *currentObjectBounds;
                    double rawMinX = 0.0;
                    double rawMaxX = c1dx + c2dx + edx;
                    double rawMinY = 0.0;
                    double rawMaxY = c1dy + c2dy + edy;
                    double rawEndX = c1dx + c2dx + edx;
                    double rawEndY = c1dy + c2dy + edy;
                    rawMinX = (std::min)(rawMinX, c1dx);
                    rawMinX = (std::min)(rawMinX, c1dx + c2dx);
                    rawMinX = (std::min)(rawMinX, rawEndX);
                    rawMaxX = (std::max)(rawMaxX, c1dx);
                    rawMaxX = (std::max)(rawMaxX, c1dx + c2dx);
                    rawMaxX = (std::max)(rawMaxX, rawEndX);
                    rawMinY = (std::min)(rawMinY, c1dy);
                    rawMinY = (std::min)(rawMinY, c1dy + c2dy);
                    rawMinY = (std::min)(rawMinY, rawEndY);
                    rawMaxY = (std::max)(rawMaxY, c1dy);
                    rawMaxY = (std::max)(rawMaxY, c1dy + c2dy);
                    rawMaxY = (std::max)(rawMaxY, rawEndY);
                    double rawWidth = rawMaxX - rawMinX;
                    double rawHeight = rawMaxY - rawMinY;
                    double boundsWidth = obj.maxX - obj.minX;
                    double boundsHeight = obj.maxY - obj.minY;
                    double sx = 1.0;
                    double sy = 1.0;
                    if (currentRawScaleX.value() == 0) {
                        sx = (boundsWidth == 0.0) ? 0.0 : ((rawWidth != 0.0) ? (boundsWidth / rawWidth) : 1.0);
                    }
                    if (currentRawScaleY.value() == 0) {
                        sy = (boundsHeight == 0.0) ? 0.0 : ((rawHeight != 0.0) ? (boundsHeight / rawHeight) : 1.0);
                    }
                    c1dx *= sx;
                    c2dx *= sx;
                    edx *= sx;
                    c1dy *= sy;
                    c2dy *= sy;
                    edy *= sy;
                }
                Point c1{current.x + c1dx, current.y + c1dy};
                Point c2{current.x + c1dx + c2dx, current.y + c1dy + c2dy};
                current.x += c1dx + c2dx + edx;
                current.y += c1dy + c2dy + edy;
                Point endPoint = current;

                const auto options = document ? document->getOptions()->get<dom::options::SmartShapeOptions>() : nullptr;
                const double tipWidth = 1.0;
                const double cp1x = options ? toEvpuDouble(options->slurThicknessCp1X) : 0.0;
                const double cp1y = options ? toEvpuDouble(options->slurThicknessCp1Y) : 8.0;
                const double cp2x = options ? toEvpuDouble(options->slurThicknessCp2X) : 0.0;
                const double cp2y = options ? toEvpuDouble(options->slurThicknessCp2Y) : 8.0;
                if (debugShape && options) {
                    std::cout << "[Shape " << debugShapeId << "] Slur options tipWidth=" << options->smartSlurTipWidth
                              << " cp1=(" << options->slurThicknessCp1X << "," << options->slurThicknessCp1Y
                              << ") cp2=(" << options->slurThicknessCp2X << "," << options->slurThicknessCp2Y
                              << ")\n";
                }

                Point worldStart = toWorld(startPoint);
                Point worldC1 = toWorld(c1);
                Point worldC2 = toWorld(c2);
                Point worldEnd = toWorld(endPoint);

                Point chord{worldEnd.x - worldStart.x, worldEnd.y - worldStart.y};
                double chordLen = std::hypot(chord.x, chord.y);
                Point unit{0.0, 0.0};
                Point normal{0.0, 0.0};
                if (chordLen > 0.0) {
                    unit = {chord.x / chordLen, chord.y / chordLen};
                    normal = {-unit.y, unit.x};
                }
                double bulge = (worldC1.y - worldStart.y) + (worldC2.y - worldStart.y);
                double offsetSign = (bulge >= 0.0) ? -1.0 : 1.0;
                constexpr double kInvSqrt2 = 0.7071067811865476;
                double tipScale = tipWidth * kInvSqrt2;

                Point worldOffsetStart{
                    worldStart.x + unit.x * tipScale + normal.x * tipScale * offsetSign,
                    worldStart.y + unit.y * tipScale + normal.y * tipScale * offsetSign
                };
                Point worldOffsetEnd{
                    worldEnd.x - unit.x * tipScale + normal.x * tipScale * offsetSign,
                    worldEnd.y - unit.y * tipScale + normal.y * tipScale * offsetSign
                };
                Point worldOffsetC1{
                    worldC1.x + unit.x * cp1x + normal.x * cp1y * offsetSign,
                    worldC1.y + unit.y * cp1x + normal.y * cp1y * offsetSign
                };
                Point worldOffsetC2{
                    worldC2.x + unit.x * cp2x + normal.x * cp2y * offsetSign,
                    worldC2.y + unit.y * cp2x + normal.y * cp2y * offsetSign
                };

                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] Slur start=(" << worldStart.x << "," << worldStart.y
                              << ") end=(" << worldEnd.x << "," << worldEnd.y << ") c1=(" << worldC1.x << ","
                              << worldC1.y << ") c2=(" << worldC2.x << "," << worldC2.y << ")\n";
                }
                if (path.empty()) {
                    path.moveTo(worldStart);
                    pathBounds.include(worldStart);
                    strokeBounds.include(worldStart);
                }
                path.curveTo(worldC1, worldC2, worldEnd);
                path.lineTo(worldOffsetEnd);
                path.curveTo(worldOffsetC2, worldOffsetC1, worldOffsetStart);
                path.closePath();

                bool suppressBounds = false;
                if (currentRawScaleX && currentRawScaleY
                    && currentRawScaleX.value() == 0
                    && currentRawScaleY.value() == 0
                    && currentObjectBounds && currentObjectBounds->hasValue) {
                    const double objWidth = currentObjectBounds->maxX - currentObjectBounds->minX;
                    const double objHeight = currentObjectBounds->maxY - currentObjectBounds->minY;
                    suppressBounds = (objWidth == 0.0 || objHeight == 0.0);
                }
                if (!suppressBounds) {
                    Bounds curveBounds = computeCubicBounds(worldStart, worldC1, worldC2, worldEnd);
                    pathBounds.include({curveBounds.minX, curveBounds.minY});
                    pathBounds.include({curveBounds.maxX, curveBounds.maxY});
                    Bounds offsetBounds = computeCubicBounds(worldOffsetEnd, worldOffsetC2, worldOffsetC1, worldOffsetStart);
                    pathBounds.include({offsetBounds.minX, offsetBounds.minY});
                    pathBounds.include({offsetBounds.maxX, offsetBounds.maxY});
                }
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
                Point w0 = toWorld(p0);
                Point w1 = toWorld(p1);
                Point w2 = toWorld(p2);
                Point w3 = toWorld(p3);
                path.moveTo(w0);
                path.lineTo(w1);
                path.lineTo(w2);
                path.lineTo(w3);
                path.closePath();
                pathBounds.includeRect(w0, w2);
                includeLineStrokeBounds(strokeBounds, w0, w1, paint.strokeWidth / 2.0);
                includeLineStrokeBounds(strokeBounds, w1, w2, paint.strokeWidth / 2.0);
                includeLineStrokeBounds(strokeBounds, w2, w3, paint.strokeWidth / 2.0);
                includeLineStrokeBounds(strokeBounds, w3, w0, paint.strokeWidth / 2.0);
                current = p0;
                start = p0;
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
                Point worldCenter = toWorld(center);
                double worldRx = rx * std::hypot(currentTransform.a, currentTransform.b);
                double worldRy = ry * std::hypot(currentTransform.c, currentTransform.d);

                auto isSentinel = [](dom::Evpu value) {
                    return value == (std::numeric_limits<dom::Evpu>::min)()
                        || value == (std::numeric_limits<dom::Evpu>::max)();
                };

                if (currentObjectBounds && currentObjectBounds->hasValue
                    && currentGroupTransform.b == 0.0 && currentGroupTransform.c == 0.0) {
                    const auto& obj = *currentObjectBounds;
                    if (!isSentinel(static_cast<dom::Evpu>(obj.minX))
                        && !isSentinel(static_cast<dom::Evpu>(obj.maxX))
                        && !isSentinel(static_cast<dom::Evpu>(obj.minY))
                        && !isSentinel(static_cast<dom::Evpu>(obj.maxY))) {
                        double left = obj.minX;
                        double right = obj.maxX;
                        double bottom = obj.minY;
                        double top = obj.maxY;
                        double objWidth = right - left + 1.0;
                        double objHeight = top - bottom + 1.0;
                        Point objCenter{(left + right) / 2.0, (top + bottom) / 2.0};
                        worldCenter = {objCenter.x, -objCenter.y};
                        worldRx = objWidth / 2.0;
                        worldRy = objHeight / 2.0;
                    }
                }
                pendingEllipse = EllipseState{worldCenter, worldRx, worldRy};

                Point startPt{center.x + rx, center.y};
                current = startPt;
                start = startPt;
            }
            break;
        }
        case IT::ClosePath:
            beginDrawing();
            if (pathBounds.hasValue && lastLineStartWorld && lastLineEndWorld && firstLineStartWorld && firstLineEndWorld) {
                Point center{(pathBounds.minX + pathBounds.maxX) / 2.0,
                             (pathBounds.minY + pathBounds.maxY) / 2.0};
                includeLineStrokeBounds(strokeBounds, *lastLineEndWorld, *firstLineStartWorld,
                                        paint.strokeWidth / 2.0);
                includeLineJoinBounds(strokeBounds, *lastLineStartWorld, *lastLineEndWorld, *firstLineStartWorld,
                                      center, paint.strokeWidth / 2.0);
                includeLineJoinBounds(strokeBounds, *lastLineEndWorld, *firstLineStartWorld, *firstLineEndWorld,
                                      center, paint.strokeWidth / 2.0);
            }
            path.closePath();
            current = start;
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] ClosePath\n";
            }
            break;
        case IT::FillSolid:
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] FillSolid\n";
            }
            if (pendingEllipse) {
                const auto ellipse = *pendingEllipse;
                std::ostringstream element;
                element << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                        << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                element << " fill=\"" << grayToRgb(paint.gray) << "\"";
                element << " stroke=\"none\"";
                element << "/>";
                elements.push_back(element.str());

                Bounds ellipseBounds;
                ellipseBounds.include({ellipse.center.x - ellipse.rx, ellipse.center.y - ellipse.ry});
                ellipseBounds.include({ellipse.center.x + ellipse.rx, ellipse.center.y + ellipse.ry});
                bounds.include({ellipseBounds.minX, ellipseBounds.minY});
                bounds.include({ellipseBounds.maxX, ellipseBounds.maxY});

                if (lineWidthSet && paint.strokeWidth > 0.0) {
                    std::ostringstream strokeElement;
                    strokeElement << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                                  << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                    strokeElement << " fill=\"none\"";
                    strokeElement << " stroke=\"" << grayToRgb(0) << "\"";
                    strokeElement << " stroke-width=\"" << paint.strokeWidth << "\"";
                    strokeElement << "/>";
                    elements.push_back(strokeElement.str());
                }

                pendingEllipse.reset();
            }
            emitPath(true, false, false);
            if (lineWidthSet && paint.strokeWidth > 0.0) {
                int savedGray = paint.gray;
                paint.gray = 0;
                emitPath(false, false, true);
                paint.gray = savedGray;
            }
            break;
        case IT::FillAlt:
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] FillAlt\n";
            }
            if (pendingEllipse) {
                const auto ellipse = *pendingEllipse;
                std::ostringstream element;
                element << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                        << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                element << " fill=\"" << grayToRgb(paint.gray) << "\"";
                element << " stroke=\"none\"";
                element << "/>";
                elements.push_back(element.str());

                Bounds ellipseBounds;
                ellipseBounds.include({ellipse.center.x - ellipse.rx, ellipse.center.y - ellipse.ry});
                ellipseBounds.include({ellipse.center.x + ellipse.rx, ellipse.center.y + ellipse.ry});
                bounds.include({ellipseBounds.minX, ellipseBounds.minY});
                bounds.include({ellipseBounds.maxX, ellipseBounds.maxY});

                if (lineWidthSet && paint.strokeWidth > 0.0) {
                    std::ostringstream strokeElement;
                    strokeElement << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                                  << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                    strokeElement << " fill=\"none\"";
                    strokeElement << " stroke=\"" << grayToRgb(0) << "\"";
                    strokeElement << " stroke-width=\"" << paint.strokeWidth << "\"";
                    strokeElement << "/>";
                    elements.push_back(strokeElement.str());
                }

                pendingEllipse.reset();
            }
            emitPath(true, true, false);
            if (lineWidthSet && paint.strokeWidth > 0.0) {
                int savedGray = paint.gray;
                paint.gray = 0;
                emitPath(false, false, true);
                paint.gray = savedGray;
            }
            break;
        case IT::Stroke:
            if (debugShape) {
                std::cout << "[Shape " << debugShapeId << "] Stroke\n";
            }
            if (pendingEllipse) {
                const auto ellipse = *pendingEllipse;
                std::ostringstream strokeElement;
                strokeElement << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                              << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                strokeElement << " fill=\"none\"";
                strokeElement << " stroke=\"" << grayToRgb(paint.gray) << "\"";
                strokeElement << " stroke-width=\"" << paint.strokeWidth << "\"";
                strokeElement << "/>";
                elements.push_back(strokeElement.str());

                Bounds ellipseBounds;
                ellipseBounds.include({ellipse.center.x - ellipse.rx, ellipse.center.y - ellipse.ry});
                ellipseBounds.include({ellipse.center.x + ellipse.rx, ellipse.center.y + ellipse.ry});
                bounds.include({ellipseBounds.minX, ellipseBounds.minY});
                bounds.include({ellipseBounds.maxX, ellipseBounds.maxY});
                pendingEllipse.reset();
            }
            emitPath(false, false, true);
            break;
        case IT::LineWidth: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::LineWidth>(&inst.data);
            if (data) {
                paint.strokeWidth = toEvpuFromEfix(data->efix);
                lineWidthSet = true;
                if (debugShape) {
                    std::cout << "[Shape " << debugShapeId << "] LineWidth efix=" << data->efix
                              << " strokeWidth=" << paint.strokeWidth << '\n';
                }
            }
            break;
        }
        case IT::SetDash:
            break;
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
            ExternalGraphicInfo info;
            info.width = data->width;
            info.height = data->height;
            info.cmper = data->cmper;
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
            Point worldTopLeft = toWorld({x, y + height});
            std::ostringstream element;
            element << "<image x=\"" << worldTopLeft.x << "\" y=\"" << worldTopLeft.y
                    << "\" width=\"" << width << "\" height=\"" << height
                    << "\" href=\"data:" << payload->mimeType
                    << ";base64," << encoded << "\"/>";
            elements.push_back(element.str());

            Point worldMin = toWorld({x, y});
            Point worldMax = toWorld({x + width, y + height});
            bounds.include({worldMin.x, worldMin.y});
            bounds.include({worldMax.x, worldMax.y});
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
            if (data->codePoint == U'\n' || data->codePoint == U'\r') {
                int fontSizePoints = text.font.fontSize > 0 ? text.font.fontSize : 12;
                double fontSizeEvpu = fontSizePoints * dom::EVPU_PER_POINT;
                current.x = textLineStartX;
                current.y -= fontSizeEvpu;
                start = current;
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
            Point anchor = toWorld(current);
            GlyphMetrics metrics = resolveMetrics(currentFont, data->codePoint, fontSizeEvpu);
            std::ostringstream element;
            element << "<text";
            if (!fontName.empty()) {
                element << " font-family=\"" << fontName << "\"";
            }
            element << " transform=\"translate(" << anchor.x << ',' << anchor.y << ")\"";
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
            element << " xml:space=\"preserve\"";
            element << ">" << utf8FromCodePoint(data->codePoint) << "</text>";
            elements.push_back(element.str());

            Bounds localBounds;
            localBounds.include({anchor.x, anchor.y + metrics.descent});
            localBounds.include({anchor.x + metrics.advance, anchor.y - metrics.ascent});
            bounds.include({localBounds.minX, localBounds.minY});
            bounds.include({localBounds.maxX, localBounds.maxY});
            current.x += metrics.advance;
            break;
        }
        case IT::CloneChar: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::CloneChar>(&inst.data);
            if (!data || (text.hasFont && text.font.hidden)) {
                break;
            }
            if (data->codePoint == U'\n' || data->codePoint == U'\r') {
                int fontSizePoints = text.font.fontSize > 0 ? text.font.fontSize : 12;
                double fontSizeEvpu = fontSizePoints * dom::EVPU_PER_POINT;
                current.x = textLineStartX;
                current.y -= fontSizeEvpu;
                start = current;
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
            GlyphMetrics metrics = resolveMetrics(currentFont, data->codePoint, fontSizeEvpu);
            double step = metrics.advance;
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
                Point anchor = toWorld({current.x + ux * offset, current.y + uy * offset + baselineShift});
                std::ostringstream element;
                element << "<text";
                if (!fontName.empty()) {
                    element << " font-family=\"" << fontName << "\"";
                }
                element << " transform=\"translate(" << anchor.x << ',' << anchor.y << ")\"";
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
                element << " xml:space=\"preserve\"";
                element << ">" << glyph << "</text>";
                elements.push_back(element.str());

                Bounds localBounds;
                localBounds.include({anchor.x, anchor.y + metrics.descent});
                localBounds.include({anchor.x + metrics.advance, anchor.y - metrics.ascent});
                bounds.include({localBounds.minX, localBounds.minY});
                bounds.include({localBounds.maxX, localBounds.maxY});
            }
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
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    svg << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
           "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    svg << "<!-- Generator: MusxDom -->\n";
    if (bounds.hasValue) {
        double minX = bounds.minX;
        double minY = bounds.minY;
        double maxX = bounds.maxX;
        double maxY = bounds.maxY;
        double width = maxX - minX;
        double height = maxY - minY;
        svg << "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" "
               "xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\""
            << minX << ' ' << minY << ' ' << width << ' ' << height << "\"";
        if (width > 0.0 && height > 0.0) {
            svg << " width=\"" << width << "\" height=\"" << height << "\"";
        }
        svg << " xml:space=\"preserve\">\n";
    } else {
        svg << "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" "
               "xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 0 0\" "
               "xml:space=\"preserve\">\n";
    }

    svg << "    <g id=\"MusxDom\">\n";
    for (const auto& element : elements) {
        svg << "        " << element << '\n';
    }
    svg << "    </g>\n";
    svg << "</svg>\n";
    return svg.str();
}



} // namespace util
} // namespace musx
