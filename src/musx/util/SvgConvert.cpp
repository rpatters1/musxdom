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
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
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
        minX = (std::min)(minX, pt.x);
        maxX = (std::max)(maxX, pt.x);
        minY = (std::min)(minY, pt.y);
        maxY = (std::max)(maxY, pt.y);
    }

    void includeRect(const Point& a, const Point& b)
    {
        include(a);
        include(b);
        include({a.x, b.y});
        include({b.x, a.y});
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
    int packedKindCodes{};
    int startId{};
    int endId{};
    int extra{};

    int startKindCode() const noexcept
    { return static_cast<int>(static_cast<unsigned int>(packedKindCodes) & 0xFFFFu); }

    int endKindCode() const noexcept
    { return static_cast<int>((static_cast<unsigned int>(packedKindCodes) >> 16) & 0xFFFFu); }
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

double toEvpuDouble(dom::Evpu value)
{
    return static_cast<double>(value);
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

Transform makeTranslateRotateScaleWithPivot(const Point& translate, double sx, double sy, double radians, const Point& pivot)
{
    Transform scale = makeScale(sx, sy);
    Transform rotate = makeRotate(radians);
    Point pivotScaled{pivot.x * sx, pivot.y * sy};
    Transform toPivot = makeTranslate(pivotScaled.x, pivotScaled.y);
    Transform fromPivot = makeTranslate(-pivotScaled.x, -pivotScaled.y);
    Transform rotateAboutPivot = multiplyTransforms(toPivot, multiplyTransforms(rotate, fromPivot));
    Transform translateTransform = makeTranslate(translate.x, translate.y);
    return multiplyTransforms(translateTransform, multiplyTransforms(rotateAboutPivot, scale));
}

Point applyTransform(const Transform& transform, const Point& pt)
{
    return {
        transform.a * pt.x + transform.c * pt.y + transform.tx,
        transform.b * pt.x + transform.d * pt.y + transform.ty
    };
}

/// @brief Decode packed rotation data to radians.
/// @details Supports both encodings observed in shape data:
///          1) Sign+magnitude trig encoding:
///             bit 31 (0x80000000) = sign of cos(theta),
///             bit 30 (0x40000000) = sign of sin(theta),
///             bits 0..10 = |sin(theta)| scaled to 0..0x400.
///          2) Angle-marker encoding:
///             bit 29 (0x20000000) marks a fixed-scale angle payload,
///             interpreted as floor(radians * 100000).
///          In both encodings, 0 means no rotation.
/// @return Angle in the range [0, 2*pi).
double decodeRotationRadians(int rotationValue)
{
    constexpr double kPi = 3.14159265358979323846;

    const uint32_t raw = static_cast<uint32_t>(rotationValue);
    constexpr uint32_t kAngleEncodingFlag = 0x20000000u;

    if ((raw & kAngleEncodingFlag) != 0u) {
        const double fullTurn = 2.0 * kPi;
        const double radians = static_cast<double>(raw & ~kAngleEncodingFlag) / 100000.0;
        const double wrapped = std::fmod(radians, fullTurn);
        return wrapped >= 0.0 ? wrapped : wrapped + fullTurn;
    }

    // Canonical boundary aliases observed in the legacy encoding:
    //   0°   -> 0x00000000
    //   90°  -> 0x00000400
    //   180° -> 0x80000000
    //   270° -> 0x40000400  (preferred alias; 0xC0000400 also appears)
    switch (raw) {
        case 0x00000000u: return 0.0;
        case 0x00000400u: return 0.5 * kPi;
        case 0x80000400u: return 0.5 * kPi;
        case 0x80000000u: return 1.0 * kPi;
        case 0x40000400u: return 1.5 * kPi;
        case 0xC0000400u: return 1.5 * kPi;
        default: break;
    }

    const bool bit31CosNegative = (raw & 0x80000000u) != 0;
    const bool bit30SinNegative = (raw & 0x40000000u) != 0;

    // The low 11 bits encode |sin(theta)| scaled to 0..0x400.
    const uint32_t ticks = raw & 0x7FFu;
    const double ratio = (std::min)(1.0, (std::max)(0.0, static_cast<double>(ticks) / 1024.0));
    const double a = std::asin(ratio);

    // Quadrant selection based on sign bits (bit31 = cos sign, bit30 = sin sign).
    // Returned angle is in [0, 2pi).
    if (!bit31CosNegative && !bit30SinNegative) {
        // q0: 0 + a
        return a;
    }
    if (bit31CosNegative && !bit30SinNegative) {
        // q1: pi - a
        return kPi - a;
    }
    if (bit31CosNegative && bit30SinNegative) {
        // q2: pi + a
        return kPi + a;
    }

    // q3: 2pi - a
    return (2.0 * kPi) - a;
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

Point normalize(const Point& vector)
{
    double length = std::hypot(vector.x, vector.y);
    if (length == 0.0) {
        return {};
    }
    return {vector.x / length, vector.y / length};
}

[[maybe_unused]] double arrowheadSizeForPreset(dom::ArrowheadPreset preset)
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

struct [[maybe_unused]] ArrowheadGeometry
{
    std::string path;
    Bounds bounds;
};

struct SvgFragment
{
    std::string content;
    Bounds bounds;
    bool hasBounds{};
};

[[maybe_unused]] ArrowheadGeometry makeArrowheadPath(const Point& tip, const Point& direction, double size, bool curved)
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

std::optional<Bounds> parseSvgViewBoxBounds(std::string_view svg)
{
    const std::string_view key = "viewBox=\"";
    const auto pos = svg.find(key);
    if (pos == std::string_view::npos) {
        return std::nullopt;
    }
    const auto start = pos + key.size();
    const auto end = svg.find('"', start);
    if (end == std::string_view::npos) {
        return std::nullopt;
    }
    std::istringstream input(std::string(svg.substr(start, end - start)));
    double minX{};
    double minY{};
    double width{};
    double height{};
    if (!(input >> minX >> minY >> width >> height)) {
        return std::nullopt;
    }
    Bounds bounds;
    bounds.include({minX, minY});
    bounds.include({minX + width, minY + height});
    return bounds;
}

std::optional<std::string> extractSvgGroupContents(std::string_view svg, std::string_view groupId)
{
    std::string marker = "<g id=\"";
    marker += std::string(groupId);
    marker += "\">";
    const auto openPos = svg.find(marker);
    if (openPos == std::string_view::npos) {
        return std::nullopt;
    }
    const auto contentStart = openPos + marker.size();
    const auto closePos = svg.find("</g>", contentStart);
    if (closePos == std::string_view::npos) {
        return std::nullopt;
    }
    return std::string(svg.substr(contentStart, closePos - contentStart));
}

std::optional<std::string> extractXmlAttribute(std::string_view tag, std::string_view name)
{
    std::string needle = std::string(name) + "=\"";
    const auto pos = tag.find(needle);
    if (pos == std::string_view::npos) {
        return std::nullopt;
    }
    const auto start = pos + needle.size();
    const auto end = tag.find('"', start);
    if (end == std::string_view::npos) {
        return std::nullopt;
    }
    return std::string(tag.substr(start, end - start));
}

bool computeTransformedBoundsFromGeneratedPath(std::string_view d,
                                              const Transform& transform,
                                              double strokeWidth,
                                              bool hasStroke,
                                              Bounds& outBounds)
{
    size_t pos = 0;
    char command = '\0';
    Point current{};
    Point subpathStart{};
    bool hasCurrent = false;
    Bounds geomBounds;
    bool hasPrevLineSegment = false;
    Point prevLineStart{};
    Point prevLineEnd{};

    auto skipSeparators = [&]() {
        while (pos < d.size()) {
            const char ch = d[pos];
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == ',') {
                ++pos;
                continue;
            }
            break;
        }
    };

    auto readNumber = [&](double& value) -> bool {
        skipSeparators();
        if (pos >= d.size()) {
            return false;
        }
        const size_t start = pos;
        bool sawDigit = false;
        if (d[pos] == '+' || d[pos] == '-') {
            ++pos;
        }
        while (pos < d.size() && std::isdigit(static_cast<unsigned char>(d[pos]))) {
            sawDigit = true;
            ++pos;
        }
        if (pos < d.size() && d[pos] == '.') {
            ++pos;
            while (pos < d.size() && std::isdigit(static_cast<unsigned char>(d[pos]))) {
                sawDigit = true;
                ++pos;
            }
        }
        if (pos < d.size() && (d[pos] == 'e' || d[pos] == 'E')) {
            size_t expPos = pos + 1;
            if (expPos < d.size() && (d[expPos] == '+' || d[expPos] == '-')) {
                ++expPos;
            }
            bool expDigits = false;
            while (expPos < d.size() && std::isdigit(static_cast<unsigned char>(d[expPos]))) {
                expDigits = true;
                ++expPos;
            }
            if (expDigits) {
                pos = expPos;
            }
        }
        if (!sawDigit) {
            pos = start;
            return false;
        }
        try {
            value = std::stod(std::string(d.substr(start, pos - start)));
        } catch (...) {
            pos = start;
            return false;
        }
        return true;
    };

    auto includeTransformedLineSegment = [&](const Point& a, const Point& b) {
        geomBounds.include(a);
        geomBounds.include(b);
        outBounds.include(a);
        outBounds.include(b);
        if (hasStroke && strokeWidth > 0.0) {
            includeLineStrokeBounds(outBounds, a, b, strokeWidth / 2.0);
            if (hasPrevLineSegment && geomBounds.hasValue) {
                Point center{
                    (geomBounds.minX + geomBounds.maxX) / 2.0,
                    (geomBounds.minY + geomBounds.maxY) / 2.0
                };
                includeLineJoinBounds(outBounds, prevLineStart, prevLineEnd, b, center, strokeWidth / 2.0);
            }
        }
        prevLineStart = a;
        prevLineEnd = b;
        hasPrevLineSegment = true;
    };

    while (true) {
        skipSeparators();
        if (pos >= d.size()) {
            break;
        }
        const char ch = d[pos];
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            command = ch;
            ++pos;
        } else if (command == '\0') {
            return false;
        }

        switch (command) {
        case 'M': {
            double x{};
            double y{};
            if (!readNumber(x) || !readNumber(y)) {
                return false;
            }
            current = {x, y};
            subpathStart = current;
            hasCurrent = true;
            const Point p = applyTransform(transform, current);
            geomBounds.include(p);
            outBounds.include(p);
            hasPrevLineSegment = false;
            command = 'L'; // SVG implicit lineto for subsequent coordinate pairs.
            break;
        }
        case 'L': {
            if (!hasCurrent) {
                return false;
            }
            double x{};
            double y{};
            if (!readNumber(x) || !readNumber(y)) {
                return false;
            }
            Point next{x, y};
            includeTransformedLineSegment(applyTransform(transform, current), applyTransform(transform, next));
            current = next;
            break;
        }
        case 'C': {
            if (!hasCurrent) {
                return false;
            }
            double x1{}, y1{}, x2{}, y2{}, x{}, y{};
            if (!readNumber(x1) || !readNumber(y1)
                || !readNumber(x2) || !readNumber(y2)
                || !readNumber(x) || !readNumber(y)) {
                return false;
            }
            const Point p0 = applyTransform(transform, current);
            const Point p1 = applyTransform(transform, {x1, y1});
            const Point p2 = applyTransform(transform, {x2, y2});
            const Point p3 = applyTransform(transform, {x, y});
            const Bounds cubicBounds = computeCubicBounds(p0, p1, p2, p3);
            geomBounds.include({cubicBounds.minX, cubicBounds.minY});
            geomBounds.include({cubicBounds.maxX, cubicBounds.maxY});
            outBounds.include({cubicBounds.minX, cubicBounds.minY});
            outBounds.include({cubicBounds.maxX, cubicBounds.maxY});
            if (hasStroke && strokeWidth > 0.0) {
                constexpr int kCurveSamples = 24;
                Point prev = p0;
                for (int i = 1; i <= kCurveSamples; ++i) {
                    const double t = static_cast<double>(i) / static_cast<double>(kCurveSamples);
                    Point next = evaluateCubic(p0, p1, p2, p3, t);
                    includeLineStrokeBounds(outBounds, prev, next, strokeWidth / 2.0);
                    prev = next;
                }
            }
            hasPrevLineSegment = false;
            current = {x, y};
            break;
        }
        case 'Z':
        case 'z': {
            if (hasCurrent) {
                includeTransformedLineSegment(applyTransform(transform, current), applyTransform(transform, subpathStart));
                current = subpathStart;
            }
            hasPrevLineSegment = false;
            break;
        }
        default:
            return false;
        }
    }

    return outBounds.hasValue;
}

std::optional<Bounds> computeTransformedBoundsFromSvgContent(std::string_view content, const Transform& transform)
{
    Bounds result;
    bool foundPath = false;
    size_t pos = 0;
    while (true) {
        const auto tagStart = content.find("<path", pos);
        if (tagStart == std::string_view::npos) {
            break;
        }
        const auto tagEnd = content.find('>', tagStart);
        if (tagEnd == std::string_view::npos) {
            break;
        }
        const std::string_view tag = content.substr(tagStart, tagEnd - tagStart + 1);
        pos = tagEnd + 1;

        const auto dAttr = extractXmlAttribute(tag, "d");
        if (!dAttr) {
            continue;
        }

        bool hasStroke = true;
        if (const auto strokeAttr = extractXmlAttribute(tag, "stroke")) {
            hasStroke = (*strokeAttr != "none");
        }
        double strokeWidth = 0.0;
        if (hasStroke) {
            if (const auto strokeWidthAttr = extractXmlAttribute(tag, "stroke-width")) {
                try {
                    strokeWidth = std::stod(*strokeWidthAttr);
                } catch (...) {
                    strokeWidth = 0.0;
                }
            }
        }

        Bounds pathBounds;
        if (!computeTransformedBoundsFromGeneratedPath(*dAttr, transform, strokeWidth, hasStroke, pathBounds)) {
            return std::nullopt;
        }
        if (pathBounds.hasValue) {
            result.include({pathBounds.minX, pathBounds.minY});
            result.include({pathBounds.maxX, pathBounds.maxY});
            foundPath = true;
        }
    }

    if (!foundPath || !result.hasValue) {
        return std::nullopt;
    }
    return result;
}

Bounds transformBoundsRect(const Bounds& source, const Transform& transform)
{
    Bounds result;
    if (!source.hasValue) {
        return result;
    }
    result.include(applyTransform(transform, {source.minX, source.minY}));
    result.include(applyTransform(transform, {source.minX, source.maxY}));
    result.include(applyTransform(transform, {source.maxX, source.minY}));
    result.include(applyTransform(transform, {source.maxX, source.maxY}));
    return result;
}

} // namespace

std::string SvgConvert::toSvg(const dom::others::ShapeDef& shape)
{
    return toSvg(shape, 1.0, SvgUnit::None, nullptr);
}

std::string SvgConvert::toSvg(const dom::others::ShapeDef& shape,
                              GlyphMetricsFn glyphMetrics)
{
    return toSvg(shape, 1.0, SvgUnit::None, std::move(glyphMetrics));
}

std::string SvgConvert::toSvg(const dom::others::ShapeDef& shape,
                              double scaling,
                              SvgUnit unit)
{
    return toSvg(shape, scaling, unit, nullptr);
}

static constexpr std::string_view unitSuffixFor(musx::util::SvgConvert::SvgUnit unit)
{
    switch (unit) {
    case musx::util::SvgConvert::SvgUnit::None:        return {};
    case musx::util::SvgConvert::SvgUnit::Pixels:      return "px";
    case musx::util::SvgConvert::SvgUnit::Points:      return "pt";
    case musx::util::SvgConvert::SvgUnit::Picas:       return "pc";
    case musx::util::SvgConvert::SvgUnit::Centimeters: return "cm";
    case musx::util::SvgConvert::SvgUnit::Millimeters: return "mm";
    case musx::util::SvgConvert::SvgUnit::Inches:      return "in";
    }
    return {};
}

static constexpr double evpuPerUnit(musx::util::SvgConvert::SvgUnit unit)
{
    switch (unit) {
    case musx::util::SvgConvert::SvgUnit::None:
        return 1.0;
    case musx::util::SvgConvert::SvgUnit::Pixels:
        return dom::EVPU_PER_PX;
    case musx::util::SvgConvert::SvgUnit::Points:
        return dom::EVPU_PER_POINT;
    case musx::util::SvgConvert::SvgUnit::Picas:
        return dom::EVPU_PER_PICA;
    case musx::util::SvgConvert::SvgUnit::Centimeters:
        return dom::EVPU_PER_CM;
    case musx::util::SvgConvert::SvgUnit::Millimeters:
        return dom::EVPU_PER_MM;
    case musx::util::SvgConvert::SvgUnit::Inches:
        return dom::EVPU_PER_INCH;
    }
    return 1.0;
}

std::string SvgConvert::toSvg(const dom::others::ShapeDef& shape,
                              double scaling,
                              SvgUnit unit,
                              GlyphMetricsFn glyphMetrics)
{
    struct ExternalGraphicPayload {
        std::string mimeType;
        std::vector<std::uint8_t> bytes;
    };

    std::vector<std::string> elements;
    Bounds bounds;
    PaintState paint;
    auto document = shape.getDocument();
    TextState text(document);
    PenState pen;
    [[maybe_unused]] ArrowheadState arrowheads;
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
    int lineSegmentCount = 0;
    bool pathHasNonLineGeometry = false;
    bool pathClosed = false;
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
    Point currentObjectTranslate{};
    double currentObjectScaleX = 1.0;
    double currentObjectScaleY = 1.0;
    bool hasCurrentObjectTransform = false;

    const double unitScale = evpuPerUnit(unit);
    const double outputScale = (unitScale != 0.0) ? (scaling / unitScale) : scaling;
    auto scalePoint = [&](const Point& value) {
        return Point{value.x * outputScale, value.y * outputScale};
    };
    auto scaleValue = [&](double value) {
        return value * outputScale;
    };

    auto toWorld = [&](const Point& local) {
        Point world = applyTransform(currentTransform, local);
        world.y = -world.y;
        return scalePoint(world);
    };

    auto toWorldCurve = [&](const Point& local, const Point& pivotLocal) {
        if (!hasCurrentObjectTransform) {
            return toWorld(local);
        }
        Transform localTransform = makeTranslateRotateScaleWithPivot(
            currentObjectTranslate,
            currentObjectScaleX,
            currentObjectScaleY,
            currentRotationRadians,
            pivotLocal);
        Transform curveTransform = multiplyTransforms(groupTransform, localTransform);
        Point world = applyTransform(curveTransform, local);
        world.y = -world.y;
        return scalePoint(world);
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
            lineSegmentCount = 0;
            pathHasNonLineGeometry = false;
            pathClosed = false;
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

        const std::string basePathData = path.str();
        std::string strokePathData = basePathData;
        std::optional<Bounds> strokeBoundsOverride;
        Bounds arrowBounds;
        bool hasArrowBounds = false;
        std::vector<std::string> arrowElements;

        auto appendPresetArrowhead = [&](const Point& tip,
                                         const Point& direction,
                                         dom::ArrowheadPreset preset) -> bool {
            const bool curved = preset == dom::ArrowheadPreset::SmallCurved
                || preset == dom::ArrowheadPreset::MediumCurved
                || preset == dom::ArrowheadPreset::LargeCurved;
            const ArrowheadGeometry geometry = makeArrowheadPath(
                tip, direction, scaleValue(arrowheadSizeForPreset(preset)), curved);
            if (geometry.path.empty() || !geometry.bounds.hasValue) {
                return false;
            }

            const std::string strokeColor = grayToRgb(paint.gray);
            if (preset == dom::ArrowheadPreset::SmallOutline) {
                std::ostringstream fillElement;
                fillElement << "<path d=\"" << geometry.path
                            << "\" stroke=\"none\" fill=\"" << grayToRgb(100) << "\"/>";
                arrowElements.push_back(fillElement.str());

                std::ostringstream outlineElement;
                outlineElement << "<path d=\"" << geometry.path
                               << "\" stroke=\"" << strokeColor
                               << "\" fill=\"none\" stroke-width=\"" << scaleValue(1.8) << "\"/>";
                arrowElements.push_back(outlineElement.str());
            } else {
                std::ostringstream element;
                element << "<path d=\"" << geometry.path
                        << "\" stroke=\"none\" fill=\"" << strokeColor << "\"/>";
                arrowElements.push_back(element.str());
            }

            arrowBounds.include({geometry.bounds.minX, geometry.bounds.minY});
            arrowBounds.include({geometry.bounds.maxX, geometry.bounds.maxY});
            hasArrowBounds = true;
            return true;
        };

        auto appendCustomArrowhead = [&](const Point& tip,
                                         const Point& direction,
                                         int arrowShapeId) -> bool {
            if (arrowShapeId <= 0 || arrowShapeId == shape.getCmper() || !document) {
                return false;
            }
            auto others = document->getOthers();
            if (!others) {
                return false;
            }
            auto arrowShape = others->get<dom::others::ShapeDef>(shape.getRequestedPartId(), dom::Cmper(arrowShapeId));
            if (!arrowShape) {
                return false;
            }
            Point dir = normalize(direction);
            if (dir.x == 0.0 && dir.y == 0.0) {
                return false;
            }
            const std::string arrowSvg = SvgConvert::toSvg(*arrowShape, scaling, unit, glyphMetrics);
            if (arrowSvg.empty()) {
                return false;
            }
            auto content = extractSvgGroupContents(arrowSvg, "MusxDom");
            if (!content || content->empty()) {
                return false;
            }
            const double angle = std::atan2(dir.y, dir.x);
            const Transform xf = makeTranslateRotateScale(tip, 1.0, 1.0, angle);

            std::ostringstream group;
            group << "<g transform=\"matrix(" << xf.a << ' ' << xf.b << ' ' << xf.c << ' ' << xf.d << ' '
                  << xf.tx << ' ' << xf.ty << ")\">" << *content << "</g>";
            arrowElements.push_back(group.str());

            if (const auto contentBounds = computeTransformedBoundsFromSvgContent(*content, xf);
                contentBounds && contentBounds->hasValue) {
                arrowBounds.include({contentBounds->minX, contentBounds->minY});
                arrowBounds.include({contentBounds->maxX, contentBounds->maxY});
                hasArrowBounds = true;
            } else if (const auto nestedBounds = parseSvgViewBoxBounds(arrowSvg);
                       nestedBounds && nestedBounds->hasValue) {
                const Bounds transformed = transformBoundsRect(*nestedBounds, xf);
                if (transformed.hasValue) {
                    arrowBounds.include({transformed.minX, transformed.minY});
                    arrowBounds.include({transformed.maxX, transformed.maxY});
                    hasArrowBounds = true;
                }
            }
            return true;
        };

        bool renderedStartArrow = false;
        bool renderedEndArrow = false;
        if (stroke && firstLineStartWorld && firstLineEndWorld && lastLineStartWorld && lastLineEndWorld) {
            auto presetForId = [](int id) -> std::optional<dom::ArrowheadPreset> {
                switch (id) {
                case 1: return dom::ArrowheadPreset::SmallFilled;
                case 2: return dom::ArrowheadPreset::SmallOutline;
                case 3: return dom::ArrowheadPreset::SmallCurved;
                case 4: return dom::ArrowheadPreset::LargeCurved;
                case 5: return dom::ArrowheadPreset::MediumCurved;
                default:
                    return std::nullopt;
                }
            };

            Point startTangent{
                firstLineEndWorld->x - firstLineStartWorld->x,
                firstLineEndWorld->y - firstLineStartWorld->y
            };
            Point endTangent{
                lastLineEndWorld->x - lastLineStartWorld->x,
                lastLineEndWorld->y - lastLineStartWorld->y
            };

            if (arrowheads.startKindCode() == 1) {
                if (auto preset = presetForId(arrowheads.startId)) {
                    renderedStartArrow = appendPresetArrowhead(
                        *firstLineStartWorld,
                        Point{-startTangent.x, -startTangent.y},
                        *preset);
                }
            } else if (arrowheads.startKindCode() == 2) {
                renderedStartArrow = appendCustomArrowhead(
                    *firstLineStartWorld,
                    Point{-startTangent.x, -startTangent.y},
                    arrowheads.startId);
            }
            if (arrowheads.endKindCode() == 1) {
                if (auto preset = presetForId(arrowheads.endId)) {
                    renderedEndArrow = appendPresetArrowhead(
                        *lastLineEndWorld,
                        endTangent,
                        *preset);
                }
            } else if (arrowheads.endKindCode() == 2) {
                renderedEndArrow = appendCustomArrowhead(
                    *lastLineEndWorld,
                    endTangent,
                    arrowheads.endId);
            }

            const bool canTrimSingleLineStroke = lineSegmentCount == 1
                && !pathHasNonLineGeometry
                && !pathClosed;
            if (canTrimSingleLineStroke && (renderedStartArrow || renderedEndArrow)) {
                Point lineStart = *firstLineStartWorld;
                Point lineEnd = *lastLineEndWorld;
                Point dir = normalize({lineEnd.x - lineStart.x, lineEnd.y - lineStart.y});
                if (!(dir.x == 0.0 && dir.y == 0.0)) {
                    const double trimAmount = scaleValue(12.0); // Finale default backoff observed in exported examples.
                    if (renderedStartArrow) {
                        lineStart.x += dir.x * trimAmount;
                        lineStart.y += dir.y * trimAmount;
                    }
                    if (renderedEndArrow) {
                        lineEnd.x -= dir.x * trimAmount;
                        lineEnd.y -= dir.y * trimAmount;
                    }
                    Point trimmedVec{lineEnd.x - lineStart.x, lineEnd.y - lineStart.y};
                    if (std::hypot(trimmedVec.x, trimmedVec.y) > 1e-6) {
                        std::ostringstream trimmed;
                        trimmed << "M " << lineStart.x << ' ' << lineStart.y
                                << " L " << lineEnd.x << ' ' << lineEnd.y;
                        strokePathData = trimmed.str();

                        Bounds trimmedStrokeBounds;
                        trimmedStrokeBounds.include(lineStart);
                        trimmedStrokeBounds.include(lineEnd);
                        includeLineStrokeBounds(trimmedStrokeBounds, lineStart, lineEnd, scaleValue(paint.strokeWidth) / 2.0);
                        strokeBoundsOverride = trimmedStrokeBounds;
                    }
                }
            }
        }

        std::ostringstream element;
        element << "<path d=\"" << (stroke ? strokePathData : basePathData) << "\"";
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
            const double strokeWidthOut = scaleValue(paint.strokeWidth);
            element << " stroke-width=\"" << strokeWidthOut << "\"";
            if (paint.dash && paint.dash->first > 0.0 && paint.dash->second > 0.0) {
                element << " stroke-dasharray=\"" << scaleValue(paint.dash->first) << ' '
                        << scaleValue(paint.dash->second) << "\"";
            }
        } else {
            element << " stroke=\"none\"";
        }
        element << "/>";
        elements.push_back(element.str());
        for (const auto& arrowElement : arrowElements) {
            elements.push_back(arrowElement);
        }

        Bounds combined = pathBounds;
        if (stroke) {
            const Bounds& effectiveStrokeBounds = strokeBoundsOverride ? *strokeBoundsOverride : strokeBounds;
            if (effectiveStrokeBounds.hasValue) {
                combined.include({effectiveStrokeBounds.minX, effectiveStrokeBounds.minY});
                combined.include({effectiveStrokeBounds.maxX, effectiveStrokeBounds.maxY});
            }
            if (hasArrowBounds) {
                combined.include({arrowBounds.minX, arrowBounds.minY});
                combined.include({arrowBounds.maxX, arrowBounds.maxY});
            }
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

    // Finale-supported external graphic types:
    // TIFF, Metafile, EPS, BMP, GIF, JPEG, PNG, PDF, SVG (export-only), EPUB (export-only).
    // We only map formats that are commonly supported in SVG <image> data URIs.
    auto mimeTypeFromExtension = [](const std::filesystem::path& extensionPath) -> std::string {
        auto extension = extensionPath.u8string();
        if (!extension.empty() && extension.front() == '.') {
            extension.erase(extension.begin());
        }
        if (extension.empty()) {
            return {};
        }
        std::string lower;
        lower.reserve(extension.size());
        for (unsigned char ch : extension) {
            lower.push_back(static_cast<char>(std::tolower(ch)));
        }
        if (lower == "png") {
            return "image/png";
        }
        if (lower == "jpg" || lower == "jpeg") {
            return "image/jpeg";
        }
        if (lower == "gif") {
            return "image/gif";
        }
        if (lower == "bmp") {
            return "image/bmp";
        }
        if (lower == "svg") {
            return "image/svg+xml";
        }
        return {};
    };

    auto readFileBytes = [](const std::filesystem::path& path) -> std::vector<std::uint8_t> {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return {};
        }
        return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    };

    shape.iterateInstructions([&](const dom::ShapeDefInstruction::Decoded& inst) -> bool {
        using IT = dom::ShapeDefInstructionType;

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
                currentObjectTranslate = translate;
                currentObjectScaleX = sx;
                currentObjectScaleY = sy;
                hasCurrentObjectTransform = true;
                Transform localTransform = makeTranslateRotateScale(translate, sx, sy, radians);
                currentTransform = multiplyTransforms(groupTransform, localTransform);
                currentGroupTransform = groupTransform;
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
                currentRotationRadians = 0.0;
                hasCurrentObjectTransform = false;
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
            lineSegmentCount = 0;
            pathHasNonLineGeometry = false;
            pathClosed = false;
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
            currentRotationRadians = 0.0;
            hasCurrentObjectTransform = false;
            const auto* data = std::get_if<dom::ShapeDefInstruction::StartGroup>(&inst.data);
            if (data) {
                origin = {};
                double sx = (data->scaleX == 0) ? 1.0 : (static_cast<double>(data->scaleX) / 1000.0);
                double sy = (data->scaleY == 0) ? 1.0 : (static_cast<double>(data->scaleY) / 1000.0);
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
            textLineStartX = current.x;
            lineWidthSet = false;
            path.clear();
            pathBounds.clear();
            strokeBounds.clear();
            pathRendered = false;
            resetLineTracking();
            lineSegmentCount = 0;
            pathHasNonLineGeometry = false;
            pathClosed = false;
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
            lineSegmentCount = 0;
            pathHasNonLineGeometry = false;
            pathClosed = false;
            resetEllipse();
            lastStartObject.reset();
            lastMove.reset();
            currentRawScaleX.reset();
            currentRawScaleY.reset();
            currentRotationRadians = 0.0;
            hasCurrentObjectTransform = false;
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
                ++lineSegmentCount;
                pathBounds.include(worldEnd);
                const double strokeWidthOut = scaleValue(paint.strokeWidth);
                includeLineStrokeBounds(strokeBounds, worldStart, worldEnd, strokeWidthOut / 2.0);
                double segmentLen = std::hypot(worldEnd.x - worldStart.x, worldEnd.y - worldStart.y);
                if (segmentLen > 0.0) {
                if (pathBounds.hasValue && lastLineStartWorld && lastLineEndWorld) {
                    Point center{(pathBounds.minX + pathBounds.maxX) / 2.0,
                                 (pathBounds.minY + pathBounds.maxY) / 2.0};
                    includeLineJoinBounds(strokeBounds, *lastLineStartWorld, *lastLineEndWorld, worldEnd,
                                          center, strokeWidthOut / 2.0);
                }
                    if (!firstLineStartWorld || !firstLineEndWorld) {
                        firstLineStartWorld = worldStart;
                        firstLineEndWorld = worldEnd;
                    }
                    lastLineStartWorld = worldStart;
                    lastLineEndWorld = worldEnd;
                }
            }
            break;
        }
        case IT::CurveTo: {
            beginDrawing();
            pathHasNonLineGeometry = true;
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
                Point worldStart = toWorldCurve(startPoint, startPoint);
                Point worldC1 = toWorldCurve(c1, startPoint);
                Point worldC2 = toWorldCurve(c2, startPoint);
                Point worldEnd = toWorldCurve(current, startPoint);
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
                    const double strokeWidthOut = scaleValue(paint.strokeWidth);
                    includeLineStrokeBounds(strokeBounds, prev, next, strokeWidthOut / 2.0);
                    prev = next;
                }
            }
            break;
        }
        case IT::Slur: {
            beginDrawing();
            pathHasNonLineGeometry = true;
            const auto* data = std::get_if<dom::ShapeDefInstruction::Slur>(&inst.data);
            if (data) {
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
                const double tipWidthScaled = scaleValue(tipWidth);
                const double cp1xScaled = scaleValue(cp1x);
                const double cp1yScaled = scaleValue(cp1y);
                const double cp2xScaled = scaleValue(cp2x);
                const double cp2yScaled = scaleValue(cp2y);

                Point worldStart = toWorldCurve(startPoint, startPoint);
                Point worldC1 = toWorldCurve(c1, startPoint);
                Point worldC2 = toWorldCurve(c2, startPoint);
                Point worldEnd = toWorldCurve(endPoint, startPoint);

                Point chord{worldEnd.x - worldStart.x, worldEnd.y - worldStart.y};
                double chordLen = std::hypot(chord.x, chord.y);
                Point unit{0.0, 0.0};
                Point normal{0.0, 0.0};
                if (chordLen > 0.0) {
                    unit = {chord.x / chordLen, chord.y / chordLen};
                    normal = {-unit.y, unit.x};
                }
                // Determine bulge in local coordinates so the default slur direction
                // (up to the right, down to the left) is preserved across rotations.
                double bulge = 0.0;
                {
                    Point localChord{endPoint.x - startPoint.x, endPoint.y - startPoint.y};
                    double localChordLen = std::hypot(localChord.x, localChord.y);
                    if (localChordLen > 0.0) {
                        Point localUnit{localChord.x / localChordLen, localChord.y / localChordLen};
                        Point localNormal{-localUnit.y, localUnit.x};
                        const Point c1Vec{c1.x - startPoint.x, c1.y - startPoint.y};
                        const Point c2Vec{c2.x - startPoint.x, c2.y - startPoint.y};
                        bulge = (c1Vec.x * localNormal.x + c1Vec.y * localNormal.y)
                            + (c2Vec.x * localNormal.x + c2Vec.y * localNormal.y);
                    } else {
                        bulge = (c1.y - startPoint.y) + (c2.y - startPoint.y);
                    }
                }
                double offsetSign = (bulge >= 0.0) ? 1.0 : -1.0;
                constexpr double kInvSqrt2 = 0.7071067811865476;
                double tipScale = tipWidthScaled * kInvSqrt2;

                Point worldOffsetStart{
                    worldStart.x + unit.x * tipScale + normal.x * tipScale * offsetSign,
                    worldStart.y + unit.y * tipScale + normal.y * tipScale * offsetSign
                };
                Point worldOffsetEnd{
                    worldEnd.x - unit.x * tipScale + normal.x * tipScale * offsetSign,
                    worldEnd.y - unit.y * tipScale + normal.y * tipScale * offsetSign
                };
                Point worldOffsetC1{
                    worldC1.x + unit.x * cp1xScaled + normal.x * cp1yScaled * offsetSign,
                    worldC1.y + unit.y * cp1xScaled + normal.y * cp1yScaled * offsetSign
                };
                Point worldOffsetC2{
                    worldC2.x + unit.x * cp2xScaled + normal.x * cp2yScaled * offsetSign,
                    worldC2.y + unit.y * cp2xScaled + normal.y * cp2yScaled * offsetSign
                };

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
            pathHasNonLineGeometry = true;
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
                const double strokeWidthOut = scaleValue(paint.strokeWidth);
                includeLineStrokeBounds(strokeBounds, w0, w1, strokeWidthOut / 2.0);
                includeLineStrokeBounds(strokeBounds, w1, w2, strokeWidthOut / 2.0);
                includeLineStrokeBounds(strokeBounds, w2, w3, strokeWidthOut / 2.0);
                includeLineStrokeBounds(strokeBounds, w3, w0, strokeWidthOut / 2.0);
                current = p0;
                start = p0;
            }
            break;
        }
        case IT::Ellipse: {
            beginDrawing();
            pathHasNonLineGeometry = true;
            const auto* data = std::get_if<dom::ShapeDefInstruction::Ellipse>(&inst.data);
            if (data) {
                double width = toEvpuDouble(data->width);
                double height = toEvpuDouble(data->height);
                Point center{current.x + (width / 2.0), current.y + (height / 2.0)};
                double rx = std::abs(width / 2.0);
                double ry = std::abs(height / 2.0);
                Point worldCenter = toWorld(center);
                double worldRx = scaleValue(rx * std::hypot(currentTransform.a, currentTransform.b));
                double worldRy = scaleValue(ry * std::hypot(currentTransform.c, currentTransform.d));
                

                auto isSentinel = [](dom::Evpu value) {
                    return value == (std::numeric_limits<dom::Evpu>::min)()
                        || value == (std::numeric_limits<dom::Evpu>::max)();
                };

                if (currentObjectBounds && currentObjectBounds->hasValue
                    && currentRawScaleX && currentRawScaleY
                    && (currentRawScaleX.value() == 0 || currentRawScaleY.value() == 0)
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
                        double objWidth = right - left;
                        double objHeight = top - bottom;
                        Point objCenter{(left + right) / 2.0, (top + bottom) / 2.0};
                        worldCenter = scalePoint({objCenter.x, -objCenter.y});
                        worldRx = scaleValue(objWidth / 2.0);
                        worldRy = scaleValue(objHeight / 2.0);
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
            pathClosed = true;
            if (pathBounds.hasValue && lastLineStartWorld && lastLineEndWorld && firstLineStartWorld && firstLineEndWorld) {
                Point center{(pathBounds.minX + pathBounds.maxX) / 2.0,
                             (pathBounds.minY + pathBounds.maxY) / 2.0};
                const double strokeWidthOut = scaleValue(paint.strokeWidth);
                includeLineStrokeBounds(strokeBounds, *lastLineEndWorld, *firstLineStartWorld,
                                        strokeWidthOut / 2.0);
                includeLineJoinBounds(strokeBounds, *lastLineStartWorld, *lastLineEndWorld, *firstLineStartWorld,
                                      center, strokeWidthOut / 2.0);
                includeLineJoinBounds(strokeBounds, *lastLineEndWorld, *firstLineStartWorld, *firstLineEndWorld,
                                      center, strokeWidthOut / 2.0);
            }
            path.closePath();
            current = start;
            break;
        case IT::FillSolid:
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
                    strokeElement << " stroke-width=\"" << scaleValue(paint.strokeWidth) << "\"";
                    if (paint.dash && paint.dash->first > 0.0 && paint.dash->second > 0.0) {
                        strokeElement << " stroke-dasharray=\"" << paint.dash->first << ' ' << paint.dash->second
                                      << "\"";
                    }
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
                    strokeElement << " stroke-width=\"" << scaleValue(paint.strokeWidth) << "\"";
                    if (paint.dash && paint.dash->first > 0.0 && paint.dash->second > 0.0) {
                        strokeElement << " stroke-dasharray=\"" << paint.dash->first << ' ' << paint.dash->second
                                      << "\"";
                    }
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
            if (pendingEllipse) {
                const auto ellipse = *pendingEllipse;
                std::ostringstream strokeElement;
                strokeElement << "<ellipse cx=\"" << ellipse.center.x << "\" cy=\"" << ellipse.center.y
                              << "\" rx=\"" << ellipse.rx << "\" ry=\"" << ellipse.ry << "\"";
                strokeElement << " fill=\"none\"";
                strokeElement << " stroke=\"" << grayToRgb(paint.gray) << "\"";
                strokeElement << " stroke-width=\"" << scaleValue(paint.strokeWidth) << "\"";
                if (paint.dash && paint.dash->first > 0.0 && paint.dash->second > 0.0) {
                    strokeElement << " stroke-dasharray=\"" << paint.dash->first << ' ' << paint.dash->second
                                  << "\"";
                }
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
            }
            break;
        }
        case IT::SetDash:
            if (const auto* data = std::get_if<dom::ShapeDefInstruction::SetDash>(&inst.data)) {
                double dash = toEvpuDouble(data->dashLength);
                double space = toEvpuDouble(data->spaceLength);
                if (dash > 0.0 && space > 0.0) {
                    paint.dash = std::make_pair(dash, space);
                } else {
                    paint.dash.reset();
                }
            }
            break;
        case IT::VerticalMode: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::VerticalMode>(&inst.data);
            if (data) {
                pen.verticalAlign = data->mode;
            }
            break;
        }
        case IT::SetArrowhead: {
            if (const auto* data = std::get_if<dom::ShapeDefInstruction::SetArrowhead>(&inst.data)) {
                arrowheads.packedKindCodes = data->packedKindCodes;
                arrowheads.startId = data->startArrowId;
                arrowheads.endId = data->endArrowId;
                arrowheads.extra = data->extra;
                /// @todo Render start/end arrowhead caps for line/path instructions.
            }
            break;
        }
        case IT::ExternalGraphic: {
            const auto* data = std::get_if<dom::ShapeDefInstruction::ExternalGraphic>(&inst.data);
            if (!data) {
                unresolvedExternalGraphic = true;
                return false;
            }
            const auto graphicCmper = data->cmper;
            dom::MusxInstance<dom::others::ShapeGraphicAssign> assignment;

            if (auto doc = shape.getDocument()) {
                if (auto others = doc->getOthers()) {
                    assignment = others->get<dom::others::ShapeGraphicAssign>(shape.getRequestedPartId(), graphicCmper);
                    if (!assignment) {
                        auto assigns = others->getArray<dom::others::ShapeGraphicAssign>(shape.getRequestedPartId());
                        for (const auto& nextAssign : assigns) {
                            if (nextAssign->graphicCmper == graphicCmper) {
                                assignment = nextAssign;
                                break;
                            }
                        }
                    }
                }
            }

            std::optional<ExternalGraphicPayload> payload;
            if (auto doc = shape.getDocument()) {
                const auto& embedded = doc->getEmbeddedGraphics();
                auto it = embedded.find(graphicCmper);
                if (it != embedded.end() && !it->second.bytes.empty()) {
                    ExternalGraphicPayload embeddedPayload;
                    embeddedPayload.bytes = it->second.bytes;
                    embeddedPayload.mimeType = mimeTypeFromExtension(it->second.extension);
                    if (!embeddedPayload.mimeType.empty()) {
                        payload = std::move(embeddedPayload);
                    }
                }
            }

            if (!payload) {
                if (auto doc = shape.getDocument()) {
                    if (assignment) {
                        if (auto path = doc->resolveExternalGraphicPath(assignment->fDescId)) {
                            ExternalGraphicPayload filePayload;
                            filePayload.bytes = readFileBytes(*path);
                            if (!filePayload.bytes.empty()) {
                                filePayload.mimeType = mimeTypeFromExtension(path->extension());
                                if (!filePayload.mimeType.empty()) {
                                    payload = std::move(filePayload);
                                }
                            }
                        }
                    }
                }
            }

            if (!payload || payload->bytes.empty() || payload->mimeType.empty()) {
                unresolvedExternalGraphic = true;
                return false;
            }
            double width = toEvpuDouble(data->width);
            double height = toEvpuDouble(data->height);
            double x = current.x;
            double y = current.y;
            bool useBoundsPlacement = false;
            if (lastStartObject) {
                auto isSentinel = [](dom::Evpu value) {
                    return value == (std::numeric_limits<dom::Evpu>::min)()
                        || value == (std::numeric_limits<dom::Evpu>::max)();
                };
                if (!isSentinel(lastStartObject->left) && !isSentinel(lastStartObject->right)
                    && !isSentinel(lastStartObject->top) && !isSentinel(lastStartObject->bottom)) {
                    const double left = toEvpuDouble(lastStartObject->left);
                    const double right = toEvpuDouble(lastStartObject->right);
                    const double top = toEvpuDouble(lastStartObject->top);
                    const double bottom = toEvpuDouble(lastStartObject->bottom);
                    width = right - left;
                    height = top - bottom;
                    x = left;
                    y = -top;
                    useBoundsPlacement = true;
                }
            }
            if (assignment) {
                if (!useBoundsPlacement) {
                    width = toEvpuDouble(assignment->width);
                    height = toEvpuDouble(assignment->height);
                }
            }
            std::string encoded = base64Encode(payload->bytes);
            const double widthOut = scaleValue(width);
            const double heightOut = scaleValue(height);
            Point worldTopLeft = useBoundsPlacement ? scalePoint({x, y}) : toWorld({x, y + height});
            std::ostringstream element;
            element << "<image x=\"" << worldTopLeft.x << "\" y=\"" << worldTopLeft.y
                    << "\" width=\"" << widthOut << "\" height=\"" << heightOut
                    << "\" href=\"data:" << payload->mimeType
                    << ";base64," << encoded << "\"/>";
            elements.push_back(element.str());

            Point worldMin = useBoundsPlacement ? scalePoint({x, y}) : toWorld({x, y});
            Point worldMax = useBoundsPlacement ? scalePoint({x + width, y + height}) : toWorld({x + width, y + height});
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
            GlyphMetrics metricsOut{
                scaleValue(metrics.advance),
                scaleValue(metrics.ascent),
                scaleValue(metrics.descent)
            };
            std::ostringstream element;
            element << "<text";
            if (!fontName.empty()) {
                element << " font-family=\"" << fontName << "\"";
            }
            element << " transform=\"translate(" << anchor.x << ',' << anchor.y << ")\"";
            element << " font-size=\"" << scaleValue(fontSizeEvpu) << "\"";
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
            localBounds.include({anchor.x, anchor.y + metricsOut.descent});
            localBounds.include({anchor.x + metricsOut.advance, anchor.y - metricsOut.ascent});
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
            GlyphMetrics metricsOut{
                scaleValue(metrics.advance),
                scaleValue(metrics.ascent),
                scaleValue(metrics.descent)
            };
            double step = metrics.advance;
            if (step <= 0.0) {
                step = length;
            }
            int repeats = (step > 0.0) ? (std::max)(1, static_cast<int>(std::floor(length / step)) + 1) : 1;
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
                element << " font-size=\"" << scaleValue(fontSizeEvpu) << "\"";
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
                localBounds.include({anchor.x, anchor.y + metricsOut.descent});
                localBounds.include({anchor.x + metricsOut.advance, anchor.y - metricsOut.ascent});
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
    const std::string_view unitSuffix = unitSuffixFor(unit);
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
            svg << " width=\"" << width;
            if (!unitSuffix.empty()) {
                svg << unitSuffix;
            }
            svg << "\" height=\"" << height;
            if (!unitSuffix.empty()) {
                svg << unitSuffix;
            }
            svg << "\"";
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

std::string SvgConvert::toSvgWithPageFormatScaling(const dom::others::ShapeDef& shape,
                                                   SvgUnit unit)
{
    return toSvgWithPageFormatScaling(shape, unit, nullptr);
}

std::string SvgConvert::toSvgWithPageFormatScaling(const dom::others::ShapeDef& shape,
                                                   SvgUnit unit,
                                                   GlyphMetricsFn glyphMetrics)
{
    const auto options = shape.getDocument()->getOptions()->get<dom::options::PageFormatOptions>();
    MUSX_ASSERT_IF(!options) {
        throw std::invalid_argument("PageFormatOptions are not available on this Document.");
    }
    auto pageFormat = options->pageFormatScore;
    MUSX_ASSERT_IF(!pageFormat) {
        throw std::invalid_argument("PageFormatOptions has no score page format.");
    }
    const double scaling = pageFormat->calcCombinedSystemScaling().toDouble();
    return toSvg(shape, scaling, unit, std::move(glyphMetrics));
}



} // namespace util
} // namespace musx
