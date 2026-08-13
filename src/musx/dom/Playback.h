/*
 * Copyright (C) 2026, Robert Patterson
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

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "BaseClasses.h"

namespace musx {
namespace dom {
namespace others {

/**
 * @class PlaybackRoute
 * @brief Defines the playback destination assigned to one or more staff playback streams.
 *
 * The cmper is the one-based playback route ID. @ref StaffPlayData::PlaybackSettings stores
 * the corresponding route as a zero-based index.
 *
 * This class is identified by the XML node name "playbackRoute".
 */
class PlaybackRoute : public OthersBase
{
public:
    /** @brief Constructor function. */
    explicit PlaybackRoute(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    int virtualChannel{};                       ///< The zero-based virtual MIDI channel. (xml node is `<virtChannel>`)
    std::string patchType;                      ///< The encoding used for the MIDI bank and patch values.
    int patch{};                                ///< The MIDI patch or program value.
    int midiBankSelectMostSignificantByte{};   ///< The MIDI Bank Select most-significant-byte value. (xml node is `<msb>`)
    Cmper percussionMapId{};                    ///< The percussion map reference ID. (xml node is `<percMapRefID>`)
    std::string soundUuid;                      ///< The UUID of the sound assigned to the route. (xml node is `<soundUUID>`)

    constexpr static std::string_view XmlNodeName = "playbackRoute"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PlaybackRoute>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class PlaybackRouteName
 * @brief Contains the display name of a playback route.
 *
 * The cmper is the one-based playback route ID and identifies the corresponding @ref PlaybackRoute.
 * Finale commonly stores the name of the playback device or software instrument, such as an audio-unit
 * or VST player, rather than the name of the musical instrument on the staff.
 *
 * This class is identified by the XML node name "playbackRouteName".
 */
class PlaybackRouteName : public OthersBase
{
public:
    /** @brief Constructor function. */
    explicit PlaybackRouteName(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::string name; ///< The playback route display name.

    constexpr static std::string_view XmlNodeName = "playbackRouteName"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PlaybackRouteName>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffPlayData
 * @brief Defines playback routing, velocity control, and enablement for a staff's playback streams.
 *
 * The cmper is the staff ID. Separate settings are stored for chords, MIDI expressions, and each
 * supported notation layer.
 *
 * This class is identified by the XML node name "staffPlayData".
 */
class StaffPlayData : public OthersBase
{
public:
    /**
     * @enum VelocityControlFunction
     * @brief Specifies how a playback stream's velocity-control level is applied.
     */
    enum class VelocityControlFunction
    {
        Absolute, ///< Replaces the existing playback velocity with the configured level.
        Relative, ///< Adjusts the existing playback velocity by the configured level.
    };

    /**
     * @class VelocityControl
     * @brief Defines whether and how velocity control is applied to a playback stream.
     */
    class VelocityControl : public ContainedClassBase
    {
    public:
        using ContainedClassBase::ContainedClassBase;

        bool controlOn{};                   ///< Whether velocity control is enabled.
        VelocityControlFunction function{}; ///< How the velocity-control level is applied.

        static const xml::XmlElementArray<VelocityControl>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /**
     * @class PlaybackSettings
     * @brief Defines the route and playback behavior for one staff playback stream.
     */
    class PlaybackSettings : public ContainedClassBase
    {
    public:
        using ContainedClassBase::ContainedClassBase;

        int playbackRouteIndex{};                    ///< Zero-based index of the route; the corresponding route cmper is this value plus one. (xml node is `<playbackRouteNum>`)
        int velocityControlLevel{};                  ///< The velocity-control level. (xml node is `<velControlLevel>`)
        std::shared_ptr<VelocityControl> velocityControl; ///< The velocity-control behavior, if present. (xml node is `<velControlSaveFlags>`)
        bool play{};                                 ///< Whether this playback stream is enabled.
        bool solo{};                                 ///< Whether this playback stream is soloed.

        /// @brief Returns the one-based cmper of the playback route referenced by #playbackRouteIndex.
        Cmper getPlaybackRouteId() const
        { return static_cast<Cmper>(playbackRouteIndex + 1); }

        static const xml::XmlElementArray<PlaybackSettings>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /** @brief Constructor function. */
    explicit StaffPlayData(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::shared_ptr<PlaybackSettings> chords;          ///< Playback settings for chord symbols, if present.
    std::shared_ptr<PlaybackSettings> midiExpressions; ///< Playback settings for MIDI expressions, if present.
    std::array<std::shared_ptr<PlaybackSettings>, MAX_LAYERS> layers; ///< Playback settings for each supported notation layer.

    constexpr static std::string_view XmlNodeName = "staffPlayData"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StaffPlayData>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace musx
