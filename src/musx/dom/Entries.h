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

#include <tuple>
#include <map>

#include "musx/util/Fraction.h"
#include "BaseClasses.h"
#include "CommonClasses.h"
 // do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace music_theory {
enum class NoteName : int;
class Transposer;
} // namespace music_theory
    
namespace musx {
namespace dom {

namespace others {
class Frame;
class PercussionNoteInfo;
class Staff;
class StaffComposite;
class StaffUsed;
} // namespace others

namespace details {
class TupletDef;
class GFrameHold;

/**
 * @class GFrameHoldContext
 * @brief A context wrapper for @ref GFrameHold associated with a specific part and location.
 *
 * This class retrieves the appropriate @ref GFrameHold from a Document using part, instrument, and measure IDs,
 * and enables part-aware operations like iterating over EntryFrame objects.
 */
class GFrameHoldContext {
public:
    /**
     * @brief Constructs a context-aware @ref GFrameHold wrapper.
     * 
     * @param document Weak pointer to the owning Document.
     * @param partId The requested part ID.
     * @param staffId The instrument ID for.
     * @param measureId The measure ID for.
     * @param timeOffset Subtract this amount from elapsed durations in created entry frames. A common usage might be to pass in here the
     * value returned by #others::Measure::calcMinLegacyPickupSpacer.
     */
    GFrameHoldContext(const DocumentPtr& document, Cmper partId, Cmper staffId, Cmper measureId, util::Fraction timeOffset = 0);

    /**
     * @brief Returns the requested part ID associated with this context.
     * 
     * @return The requested part ID.
     */
    Cmper getRequestedPartId() const { return m_requestedPartId; }

    /**
     * @brief Provides const pointer-style access to the underlying @ref GFrameHold.
     * 
     * @return A const pointer to @ref GFrameHold.
     */
    const GFrameHold* operator->() const { return m_hold.get(); }

    /**
     * @brief Returns true if the internal @ref GFrameHold is valid.
     * 
     * @return True if the @ref GFrameHold was successfully retrieved; false otherwise.
     */
    explicit operator bool() const noexcept { return static_cast<bool>(m_hold); }

    /// @brief Returns the clef index in effect for at the specified @ref Edu position.
    /// This function does not take into account transposing clefs. Those are addressed in #createEntryFrame.
    /// @param position The Edu position of the clef *in staff-level Edus*. (The staff-level matters for Independent Key Signature staves.)
    ClefIndex calcClefIndexAt(Edu position) const;

    /// @brief Returns the clef index in effect for at the specified @ref util::Fraction position (as a fraction of whole notes).
    /// This function does not take into account transposing clefs. Those are addressed in #createEntryFrame.
    /// @param position The *staff-level* position of the clef. (The staff-level matters for Independent Key Signature staves.)
    ClefIndex calcClefIndexAt(util::Fraction position) const
    { return calcClefIndexAt(position.calcEduDuration()); }

    /** @brief Returns the @ref EntryFrame for all entries in the given layer.
     *
     * @param layerIndex The layer index (0..3) to iterate.
     * @return EntryFrame for layer or nullptr if none.
     */
    std::shared_ptr<const EntryFrame> createEntryFrame(LayerIndex layerIndex) const;
    
    /**
     * @brief iterates the entries for the specified layer in this @ref GFrameHold from left to right
     * @param layerIndex The layer index (0..3) to iterate.
     * @param iterator The callback function for each iteration.
     * @return true if the caller should continue iteration; false to stop early.
     * @throws std::invalid_argument if the layer index is out of range
     */
    bool iterateEntries(LayerIndex layerIndex, std::function<bool(const EntryInfoPtr&)> iterator) const;

    /**
     * @brief iterates the entries for this @ref GFrameHold from left to right for each layer in order
     * @param iterator The callback function for each iteration.
     * @return true if the caller should continue iteration; false to stop early.
     */
    bool iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator) const;

    /// @brief Calculates the number of voices used by the GFrameHold instance.
    /// @return A list of each layer that contains entries and the number voice2 entries in that layer.
    std::map<LayerIndex, int> calcVoices() const;

    /// @brief Calculates if this staff in this measure contains only a cue layer and full-measure rest layers.
    /// @param includeVisibleInScore If true, include cues that are visible in the score.
    bool calcIsCuesOnly(bool includeVisibleInScore = false) const;

    /// @brief Calculates the nearest non-grace-note entry at the given @p position.
    /// @param position The measure position to find.
    /// @param findExact If true, only find an entry that matches to within 1 evpu. Otherwise find the closest entry in the measure.
    /// @param matchLayer If specified, only find entries in this 0-based layer index. (Values 0..3)
    /// @param matchVoice2 If specified, the value of #Entry::voice2 must match the specified value.
    /// @param atGraceNoteDuration Match on this grace note duration. When it is zero, grace notes are skipped.
    /// @return The entry if found, otherwise `nullptr`.
    EntryInfoPtr calcNearestEntry(util::Fraction position, bool findExact = true, std::optional<LayerIndex> matchLayer = std::nullopt,
        std::optional<bool> matchVoice2 = std::nullopt, util::Fraction atGraceNoteDuration = 0) const;

    /// @brief Calculates the minimum legacy pickup spacer, if any.
    ///
    /// Legacy pickup spacers, created by the legacy Mirror Tool, can differ for each staff and layer. The spacer takes away
    /// time from the beginning of the layer, leaving the time at the end for the pickup to the next measure.
    /// @return The smallest legacy pickup spacer encountered in a layer for this measure and staff. Zero if none.
    util::Fraction calcMinLegacyPickupSpacer() const;

private:
    MusxInstance<GFrameHold> m_hold;        ///< The resolved GFrameHold object, or null if not found.
    Cmper m_requestedPartId;                ///< The requested part context.
    util::Fraction m_timeOffset;            ///< The time offset to apply to entry frames. 
};

} // namespace details

/**
 * @brief Calculates the @ref NoteType and number of dots in an @ref Edu value.
 *
 * @param duration The Edu duration to check.
 * @return A std::pair containing:
 *         - NoteType: The note type (Whole, Quarter, Eighth, etc.)
 *         - unsigned: The number of augmentation dots
 * @throws std::invalid_argument if the duration is out of valid range (> 1 and < 0x10000).
 */
std::pair<NoteType, unsigned> calcNoteInfoFromEdu(Edu duration);

/// @brief Calculates the number of beams or flags in the @ref Edu value.
unsigned calcNumberOfBeamsInEdu(Edu duration);

/**
 * @class Note
 * @brief Represents a single note element in an entry.
 *
 * This class is identified by the XML node name "note".
 */
class Note : public Base
{
public:
    /** @brief Constructor function */
    explicit Note(const DocumentWeakPtr& document, NoteNumber noteId)
        : Base(document, 0, ShareMode::All), m_noteId(noteId)
    {
    }

    /// @brief Non floating rests have a note with this noteId that defines their staff positions.
    static constexpr NoteNumber RESTID = 31;

    /**
     * @brief Note properites. A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    using NoteProperties = std::tuple<music_theory::NoteName, int, int, int>;
    
    int harmLev{};      ///< Diatonic displacement relative to middle C or to the tonic in the middle C octave (if the key signature tonic is not C).
    int harmAlt{};      ///< Chromatic alteration relative to the key signature. Never has a magnitude greater than +/-7.
    bool isValid{};     ///< Should always be true but otherwise appears to be used internally by Finale.
    bool tieStart{};    ///< Indicates a tie starts on this note.
    bool tieEnd{};      ///< Indicates a tie ends on this note.
    bool crossStaff{};  ///< Signifies that the note has a @ref details::CrossStaff note detail.
    bool upStemSecond{}; ///< Indicates that this note is the upper note of a second.
                        ///< When the entry is upstem, it is drawn on the "wrong" side of the stem.
    bool downStemSecond{}; ///< Indicates that this note is the lower note of a second.
                        ///< When the entry is downstem, it is drawn on the "wrong" side of the stem.
    bool upSplitStem{}; ///< True if the stem splits on this note. To split a chord in the normal way, every note from this higher
                        ///< should have #upSplitStem set to `true`. Only takes effect if #Entry::splitStem is `true`.
    bool showAcci{};    ///< True if the note has an accidental. (Dynamically changed by Finale unless `freezeAcci` is set.)
    bool parenAcci{};   ///< True if the accidental has parentheses.
    bool noPlayback{};  ///< Indicates that this note should not be played back.
    bool noSpacing{};   ///< Indicates that this note should ignored when calculating spacing.
    bool freezeAcci{};  ///< True if the accidental should be forced on or off (based on `showAcci`.)
    bool playDisabledByHP{}; ///< Used by Finale's smart playback engine.

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

    /// @brief Calculates the default enharmonic equivalent of this note. This is the value that Finale uses when
    /// #details::NoteAlterations::enharmonic is true.
    ///
    /// Normally you do not have to call this function directly. It is called inside #calcNoteProperties. But the function
    /// is available if you need it.
    ///
    /// @param key The key signature to use for transposition.
    /// @return A std::pair containing
    ///         - int: the enharmonic equivalent's displacement value relative to the tonic.
    ///         - int: the enharmonic equivalent's alteration value relative to the key signature.
    std::pair<int, int> calcDefaultEnharmonic(const MusxInstance<KeySignature>& key) const;

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position. This function does
     * not take into account percussion notes and their staff position override.
     *
     * Finale does not transpose correctly with microtonal key signatures. This function transposes
     * mostly correctly for them, which means that microtonal key signatures may have different
     * results than in Finale. Of particular note is Chromatic Transposition, which produces nonsense
     * results in Finale but here produces correct results, provided that the alteration value
     * is understood to be a chromatic half-step alteration rather than a step alteration in EDO divisions.
     *
     * @note In Finale, the default whole rest staff position is the middle staff line. Other music notation systems
     * frequently expect the standard whole rest staff position to be the second line from the top. You may need to interpolate
     * the staff position returned by #calcNoteProperties for whole rests.
     *
     * See #KeySignature::setTransposition for information about differences in key signature transposition.
     *
     * @param key The key signature in effect.
     * @param ctx The key context (concert or written pitch).
     * @param clefIndex The index of the clef in effect.
     * @param percNoteInfo If non-null, the percussion note info is used for the staff position.
     * @param staff If provided, the notes are transposed by any Chromatic Transposition specified for the staff. If
     * calling #calcNoteProperties for Concert Pitch (sounding pitch) values, omit this parameter.
     * @param respellEnharmonic If true, the notes are enharmonically respelled using the default enharmonic spelling.
     * @return #NoteProperties
     */
    NoteProperties calcNoteProperties(const MusxInstance<KeySignature>& key, KeySignature::KeyContext ctx, ClefIndex clefIndex,
        const MusxInstance<others::PercussionNoteInfo>& percNoteInfo, const MusxInstance<others::Staff>& staff = nullptr, bool respellEnharmonic = false) const;

    static const xml::XmlElementArray<Note>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.

private:
    NoteNumber m_noteId{}; ///< Unique identifier for the note.
};

/**
 * @class Entry
 * @brief Represents an entry containing metadata and notes.
 *
 * This class is identified by the XML node name "entry".
 */
class Entry : public Base
{
public:
    /** @brief Constructor function
     *
     * The partId and shareMode values should always be 0 and ShareMode::All, but they are required by the factory function.
    */
    explicit Entry(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, EntryNumber prev, EntryNumber next)
        : Base(document, partId, shareMode), m_entnum(entnum), m_prev(prev), m_next(next)
    {
    }

    /**
     * @brief Duration of the entry, not taking into account tuplets.
     *
     * This is effectively the type of note (quarter, eight, whole etc.) plus the augmentation dots, if any.
     * The most significant bit determines the note type and each set bit to the right is an augmentation dot.
     *
     * (xml node is `<dura>`)
    */
    Edu duration{};
    int numNotes{};          ///< Number of notes in the entry. There is an error if this is not the same as notes.size().
    Evpu hOffsetScore{};     ///< Manual offset created with the Note Position Tool in the score. (xml node is `<posi>`.)
    bool isValid{};          ///< Should always be true but otherwise appears to be used internally by Finale.
    bool isNote{};           ///< If this value is false, the entry is a rest.
    bool v2Launch{};         ///< Indicates if this entry (which is voice1) launches a voice2 sequence. (xml node is `<controller>`)
    bool voice2{};           ///< This is a V2 note. (xml node `<v2>`)
    bool createdByHP{};      ///< Indicates the entry was created by Finale's smart playback engine.
    bool playDisabledByHP{}; ///< Used by Finale's smart playback engine.
    bool graceNote{};        ///< Indicate the entry is a grace note.
    bool noteDetail{};       ///< Indicates there is a note detail or EntrySize record for the entry.
    bool articDetail{};      ///< Indicates there is an articulation on the entry
    bool lyricDetail{};      ///< Indicates there is a lyric assignment on the entry.
    bool tupletStart{};      ///< Indicates that a tuplet start on the entry.
    bool splitRest{};        ///< Indicates that rests in different layers are not combined on this entry.
    bool performanceData{};  ///< Indicates there is performance data on the entry.
    bool floatRest{};        ///< Is floating rest. If false, the first note element gives the staff position of the rest.
    bool isHidden{};         ///< Indicates the entry is hidden, (xml node is `<ignore>`)
    bool beamExt{};          ///< Indicates that there is a beam extension on the entry.
    bool flipTie{};          ///< Indicates the existence of a flipped tie, either in Speedy Entry or Layer Attributes.
    bool dotTieAlt{};        ///< Indicates dot or tie alterations are present.
    bool beam{};             ///< Signifies the start of a beam or singleton entry. (That is, any beam breaks at this entry.)
    bool secBeam{};          ///< Signifies a secondary beam break occurs on the entry.
    bool freezeStemScore{};  ///< Freeze stem flag in the score. (#upStemScore gives the direction.)
    bool stemDetail{};       ///< Indicates there are stem modifications.
    bool crossStaff{};       ///< Signifies that at least one note in the entry has been cross staffed.
    bool reverseUpStem{};    ///< Indicates that a stem normally up is reversed.
    bool reverseDownStem{};  ///< Indicates that a stem normally down is reversed.
    bool doubleStem{};       ///< Creates a double stem on the entry. (Appears to be exclusive with #splitStem.)
    bool splitStem{};        ///< Indicates the presence of a note with #Note::upSplitStem set.
                             ///< If no note has a split stem, it shows as a double stem. (Appears to be exclusive with #doubleStem.)
    bool upStemScore{};      ///< Whether a stem is up or down as set in the score. (Only reliable when #freezeStemScore is true.)
    bool checkAccis{};       ///< Used by Finale to convert pre-2014 `.mus` files. May never be saved in `.musx`.
    bool dummy{};            ///< An entry (usually a rest) inserted for alignment. It may not be meaningful outside the Finale runtime environment
                             ///< and is probably safe to ignore.
    bool smartShapeDetail{}; ///< Indicates this entry has a smart shape assignment.
    bool noLeger{};          ///< Hide ledger lines.
    bool sorted{};           ///< Sorted flag.
    bool slashGrace{};       ///< Indicates that a non-beamed grace note with flags (8th note or smaller) should have a slash on the stem.
                             ///< If #options::GraceNoteOptions::slashFlaggedGraceNotes is true, this options has no effect. The stem
                             ///< always has a slash in that case.
    bool flatBeam{};         ///< Forces any beam that starts on this entry to be flat by default.
    bool noPlayback{};       ///< Indicates that the entry should not be played back.
    bool noSpacing{};        ///< Indicates that the entry should be ignored when calculating music spacing.
    bool freezeBeam{};       ///< Freeze beam flag (Derived from the presence of `<freezeBeam>` node.)

    /** @brief Collection of notes that comprise the entry. These are in order from lowest to highest. */
    std::vector<std::shared_ptr<Note>> notes;

    /** @brief The location(s) of this entry calculated by #calcLocations, which is called by the factory.
     * An entry can have multiple locations if it is mirrored with the Mirror Tool. Finale 27 flattens out
     * all mirrors so musx files created by Finale 27 never should have more than one location. But if a musx
     * from an earlier version is read, the entry might have multiple locations.
     */
    std::vector<std::tuple<StaffCmper, MeasCmper, LayerIndex>> locations;

    /// @brief Gets the entry number for this entry
    EntryNumber getEntryNumber() const { return m_entnum; }

    /// @brief Gets the next entry in this list or nullptr if none.
    ///
    /// Note that the entry list may contain entries that aren't in any frame. These should be ignored.
    MusxInstance<Entry> getNext() const;

    /// @brief Gets the previous entry in this list or nullptr if none
    ///
    /// Note that the entry list may contain entries that aren't in any frame. These should be ignored.
    MusxInstance<Entry> getPrevious() const;

    /**
     * @brief Calculates the NoteType and number of augmentation dots. (See #calcNoteInfoFromEdu.)
     */
    std::pair<NoteType, unsigned> calcNoteInfo() const { return calcNoteInfoFromEdu(duration); }

    /**
     * @brief Calculates the duration as a @ref util::Fraction of a whole note
     */
    util::Fraction calcFraction() const { return util::Fraction::fromEdu(duration); }

    /// @brief Returns true if the entry's duration has a stem.
    /// @return True if the entry's duration is less than a whole note, irrespective of whether it is a rest or a note.
    bool hasStem() const { return duration < Edu(NoteType::Whole); }

    /// @brief Returns true if the entry could be a full-measure rest.
    /// @note Finale recognizes only whole rests as possible full-measure rests. Any other rest types (specifically
    /// breve rests in 4/2 and larger time signatures) are implemented by users as workarounds. These workarouds typically
    /// involve suppressing Finale's full-measure rest display and replacing them with a text expression.
    bool isPossibleFullMeasureRest() const
    { return !isNote && !isHidden && duration == Edu(NoteType::Whole); }

    void integrityCheck(const std::shared_ptr<Base>& ptrToThis) override
    {
        this->Base::integrityCheck(ptrToThis);
        if (size_t(numNotes) != notes.size()) {
            MUSX_INTEGRITY_ERROR("Entry " + std::to_string(m_entnum) + " has an incorrect number of notes.");
        }
    }

    /// @brief Calculates the locations for all entries in the document. This function is normally only called by the document factory.
    /// @param document The document to search.
    static void calcLocations(const DocumentPtr& document);

    constexpr static std::string_view XmlNodeName = "entry"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Entry>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.

private:    
    EntryNumber m_entnum;   ///< Entry number.
    EntryNumber m_prev;     ///< Previous entry number in the list. (0 if none)
    EntryNumber m_next;     ///< Next entry number in the list. (0 if none)
};

class EntryInfo;
class EntryFrame;
class NoteInfoPtr;

/// @brief Wraps a frame of shared_ptr<const EntryInfo> and an index for per entry access.
/// This class manages ownership of the frame so that any instance of it keeps the frame alive
/// without the need for circular references.
class EntryInfoPtr
{
public:
    /// @enum BeamIterationMode
    /// @brief Controls which entries are included when iterating over beams.
    enum class BeamIterationMode
    {
        Normal,                             ///< Skip hidden entries. This is how Finale displays beams.
        IncludeAllHidden,                   ///< Include all entries, even if they are hidden.
        IncludeBeamWorkaroundHiddenRests    ///< Include hidden entries only when hidden due to the beamed-rest workaround.
                                            ///< (See #calcIsBeamedRestWorkaroundHiddenRest.)
    };

    /// @brief Result of beamed-rest-aware voice traversal.
    struct BeamedRestWorkaroundAwareResult;

    /** @brief Default constructor */
    EntryInfoPtr() : m_entryFrame(nullptr), m_indexInFrame(0) {}
        
    /** @brief Constructor function
     *
     * @param entryFrame The entry frame.
     * @param index The index of this instance within the frame. 
    */
    explicit EntryInfoPtr(const std::shared_ptr<const EntryFrame>& entryFrame, size_t index = 0)
        : m_entryFrame(entryFrame), m_indexInFrame(index) {}

    /// @brief Searches the given position at @p staffId and @p measureId for the @p entryNumber.
    /// @param document The document to search.
    /// @param partId The part within the document for which to create the #EntryInfoPtr.
    /// @param staffId The ID of the staff to search.
    /// @param measureId The ID of the measure to search.
    /// @param entryNumber The EntryNumber to search for.
    /// @param timeOffset Subtract this amount from elapsed durations. A common usage might be to pass in here the
    /// value returned by #others::Measure::calcMinLegacyPickupSpacer.
    /// @return If found, an #EntryInfoPtr for the given entry number. Otherwise an null instance.
    static EntryInfoPtr fromPositionOrNull(const DocumentPtr& document, Cmper partId, StaffCmper staffId, MeasCmper measureId,
        EntryNumber entryNumber, util::Fraction timeOffset = 0);

    /// @brief Returns an EntryInfoPtr for the entry specified by @p entryNumber.
    ///
    /// If you are directly importing a musx file last saved in a version before Finale 25, it may contain mirrors.
    /// If so, one of the locations is chosen. (This applies to musx files last saved by Finale 2014 or 2014.5.)
    /// @param document The document to search.
    /// @param partId The part within the document for which to create the #EntryInfoPtr.
    /// @param entryNumber The entry to find.
    /// @param timeOffset Subtract this amount from elapsed durations. A common usage might be to pass in here the
    /// value returned by #others::Measure::calcMinLegacyPickupSpacer.
    /// @return An EntryInfoPtr corresponding to the input entryNumber or null if the entry is not in the requested part or otherwise is not found.
    static EntryInfoPtr fromEntryNumber(const DocumentPtr& document, Cmper partId, EntryNumber entryNumber, util::Fraction timeOffset = 0);

    /// @brief Allows `->` access to the underlying @ref EntryInfo instance.
    const std::shared_ptr<const EntryInfo> operator->() const;

    /// @brief Provides a boolean conversion based on whether the frame is valid and contains entries.
    operator bool() const noexcept;

    /// @brief Returns whether the input and the current instance refer to the same entry.
    /// @return false if either this or src is null and true if they are both non null and refer to the same entry.
    bool isSameEntry(const EntryInfoPtr& src) const;

    /// @brief Returns whether this entry and @p src contain the same pitch content or rest value.
    ///
    /// For note entries, only pitch content is compared; duration is ignored.
    /// For rest entries, non-floating rests are compared using their assigned display levels.
    ///
    /// @param src The EntryInfoPtr to compare.
    /// @param compareConcert If true, compares concert pitches. If false, compares scale degrees relative to the prevailing key.
    /// @return true if both pointers are non-null and represent the same pitch content or rest value, false otherwise.
    bool calcIsSamePitchContent(const EntryInfoPtr& src, bool compareConcert = true) const;

    /// @brief Returns whether this entry and @p src represent the same notated value.
    ///
    /// This function performs the same pitch/rest comparison as #calcIsSamePitchContent and additionally
    /// compares symbolic duration, actual duration, and optionally v1v2 voice number and grace-note elapsed duration.
    ///
    /// @param src The EntryInfoPtr to compare.
    /// @param compareConcert If true, compares concert pitches. If false, compares scale degrees
    ///        relative to the prevailing key.
    /// @param requireSameVoice If true, the entries must have identical v1v2 voice numbers.
    /// @param requireSameGraceElapsedDura If true, grace-note elapsed durations must match.
    ///
    /// @return true if both pointers are non-null and all required properties match; false otherwise.
    bool calcIsSamePitchContentAndDuration(const EntryInfoPtr& src, bool compareConcert = true, bool requireSameVoice = true, bool requireSameGraceElapsedDura = false) const;

    /// @brief Returns the frame.
    std::shared_ptr<const EntryFrame> getFrame() const { return m_entryFrame; }

    /// @brief Returns the index within the frame.
    size_t getIndexInFrame() const { return m_indexInFrame; }

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const;

    /// @brief Get the staff cmper
    StaffCmper getStaff() const;

    /// @brief Creates the current StaffComposite for the entry
    /// @param forStaffId Specifies optional staff ID. If supplied, it overrides the entry's staff ID. (Useful when notes are cross-staffed.)
    MusxInstance<others::StaffComposite> createCurrentStaff(const std::optional<StaffCmper>& forStaffId = std::nullopt) const;

    /// @brief Get the measure cmper
    MeasCmper getMeasure() const;

    /// @brief Get the key signature of the entry
    MusxInstance<KeySignature> getKeySignature() const;

    /// @brief Gets the applicable part data for the entry, or nullptr if none.
    MusxInstance<details::EntryPartFieldDetail> getPartFieldData() const;

    /// @brief Returns the manual offset of the entry for the current requested part. This function encapsulates
    /// handling of the case when the manual offset is unlinked and different in score and part(s).
    Evpu calcManuaOffset() const;

    /// @brief Caclulates the grace index counting leftward (used by other standards such as MNX)
    unsigned calcReverseGraceIndex() const;

    /// @brief Calculates a grace note's symbolic starting duration as a negative offset from the main note.
    /// This is useful for comparing grace note sequences.
    /// @return Negative symbolic offset from the main note, or zero if not a grace note.
    util::Fraction calcGraceElapsedDuration() const;

    /// @brief Returns the next higher tuplet index that this entry starts, or std::nullopt if none
    std::optional<size_t> calcNextTupletIndex(std::optional<size_t> currentIndex = 0) const;

    /// @brief Get the next entry in the frame
    EntryInfoPtr getNextInFrame() const;

    /// @brief Get the next entry in the same layer and staff. This can be in the next measure.
    /// @return  The next continguous entry. Returns nullptr if it encounters an empty frame or end of file.
    EntryInfoPtr getNextInLayer() const;

    /// @brief Get the next entry in the frame in the same voice.
    ///
    /// For V2, it returns null after the current V2 launch sequence.
    EntryInfoPtr getNextSameV() const;

    /// @brief Get the next entry in the frame in the same voice, skipping grace notes.
    /// @return The same as #getNextSameV except grace notes are skipped.
    EntryInfoPtr getNextSameVNoGrace() const;

    /// @brief Get the previous entry in the same layer and staff. This can be in the previous measure.
    /// @return  The previous continguous entry. Returns nullptr if it encounters an empty frame or the beginning of the file.
    EntryInfoPtr getPreviousInLayer() const;

    /// @brief Get the previous entry in the frame
    EntryInfoPtr getPreviousInFrame() const;

    /// @brief Get the previous entry in the frame in the same voice
    ///
    /// For V2, it returns null when it hits the v2Launch note for the current V2 launch sequence.
    EntryInfoPtr getPreviousSameV() const;

    /// @brief Get the previous entry in the frame in the same voice, skipping grace notes.
    /// @return The same as #getPreviousSameV except grace notes are skipped.
    EntryInfoPtr getPreviousSameVNoGrace() const;

    /// @brief Returns the next entry in the frame in the specified v1/v2 or null if none.
    ///
    /// Unlike #getNextSameV, this returns the next v2 entry in any v2 launch sequence.
    ///
    /// @param voice  Must be 1 or 2.
    EntryInfoPtr getNextInVoice(int voice) const;

    /// @brief Returns the previous entry in the frame in the specified v1/v2 or null if none.
    ///
    /// Unlike #getPreviousSameV, this returns the next v2 entry in any v2 launch sequence.
    ///
    /// @param voice  Must be 1 or 2.
    EntryInfoPtr getPreviousInVoice(int voice) const;

    /// @brief Returns the next forward entry in this voice using musxdom's
    ///        beamed-rest workaround interpretation.
    ///
    /// This function continues forward traversal from this EntryInfoPtr.  It applies the same
    /// workaround-interpretation rules documented in
    /// #EntryFrame::getFirstInVoiceBeamedRestWorkaroundAware:
    ///
    /// - Extra visible voice-2 workaround rests are **skipped**.
    /// - Hidden voice-1 workaround rests used to force a beam shape are **returned**
    ///   with BeamedRestWorkaroundAwareResult::effectiveHidden set to @c false.
    /// - All other entries retain their stored hidden state.
    ///
    /// If no further usable entry exists, the returned
    /// #BeamedRestWorkaroundAwareResult will have a null entry.
    ///
    /// @param voice  Must be 1 or 2.
    /// @return A BeamedRestWorkaroundAwareResult containing the next usable entry (or null) and its
    ///         effective-hidden flag.
    BeamedRestWorkaroundAwareResult getNextInVoiceBeamedRestWorkaroundAware(int voice) const;

    /// @brief Gets the next entry in a beamed group or nullptr if the entry is not beamed or is the last in the group.
    EntryInfoPtr getNextInBeamGroup(BeamIterationMode beamIterationMode = BeamIterationMode::Normal) const
    { return iterateBeamGroup<&EntryInfoPtr::nextPotentialInBeam, &EntryInfoPtr::previousPotentialInBeam>(beamIterationMode); }

    /// @brief Gets the previous entry in a beamed group or nullptr if the entry is not beamed or is the first in the group.
    EntryInfoPtr getPreviousInBeamGroup(BeamIterationMode beamIterationMode = BeamIterationMode::Normal) const
    { return iterateBeamGroup<&EntryInfoPtr::previousPotentialInBeam, &EntryInfoPtr::nextPotentialInBeam>(beamIterationMode); }

    /// @brief Gets the next entry in a beamed group, or nullptr if the entry is not beamed or is the last in the group.
    /// This function is simular to #getNextInBeamGroup but it traverses into the next bar when it detects a beam across a barline,
    /// as created by the Beam Over Barline plugin.
    EntryInfoPtr getNextInBeamGroupAcrossBars(BeamIterationMode beamIterationMode = BeamIterationMode::Normal) const;

    /// @brief Gets the previous entry in a beamed group or nullptr if the entry is not beamed or is the first in the group.
    /// This function is simular to #getPreviousInBeamGroup but it traverses into the previous bar when it detects a beam across a barline,
    /// as created by the Beam Over Barline plugin.
    EntryInfoPtr getPreviousInBeamGroupAcrossBars(BeamIterationMode beamIterationMode = BeamIterationMode::Normal) const;

    /// @brief Calculates if an entry displays as a rest.
    /// @todo Eventually calcDisplaysAsRest should take into account voiced parts.
    bool calcDisplaysAsRest() const;

    /// @brief Calculates the top and bottom staff positions of the entry, taking into account percussion notes. This function must not
    /// be called on a floating rest. It asserts and throws if so.
    /// @return A std::pair<int, int> with the first being the top staff position and the second being the bottom staff position.
    std::pair<int, int> calcTopBottomStaffPositions() const;

    /// @brief Returns the Entry stem settings for the current requested part. This function encapsulates handling of the case when the
    /// two booleans are unlinked and different in score and part(s).
    /// @note This function is only for getting the entry's two boolean stem settings. Use #calcUpStem to get the entry's effective stem direction,
    /// taking into account all options and situations.
    /// @return A std::pair<bool, bool> with the first being the freezeStem setting and the second being the upStem setting.
    std::pair<bool, bool> calcEntryStemSettings() const;

    /// @brief Calculates if the entry is upstem by default, without considering voices, layers, staff options, cross-staffing, or
    /// manual overrides.
    /// @return True if the entry is upstem barring any other factors that would override the stem direction. False if it is downstem.
    bool calcUpStemDefault() const;

    /// @brief Determines the effective stem direction of the entry, taking into account voices, layers, staff options, manual overrides,
    /// and cross-staff notation.
    ///
    /// The function is designed to handle all common combinations of musical options and contexts, although some rare cases may still produce
    /// incorrect results. The concept of an "up stem" becomes ambiguous when the entry uses a reverse stem.
    /// (See #Entry::reverseUpStem and #Entry::reverseDownStem.) The goal is to compute the logical stem direction that governs
    /// the entry's behavior. This allows selecting the correct up or down variant for any entry detail records or entry properties that have them,
    /// including which reverse-stem value to recognize.
    /// In cases involving reverse stems, the result of `calcUpStem` may not match the visible direction in a Finale-generated PDF.
    ///
    /// @note The #Entry::upStemScore flag may appear to provide this information, but it can be wrong if the layer context changed
    /// without the entry frame being re-edited. It also does not reflect cross-staff stem directions or staff-level overrides of stem direction.
    ///
    /// @return True if the stem is up; false if it is down.
    bool calcUpStem() const
    {
        if (m_upStem.has_value()) {
            return m_upStem.value();
        }
        m_upStem = calcUpStemImpl();
        return m_upStem.value();
    }

    /// @brief Returns whether this is an unbeamed entry
    /// @return 
    bool calcUnbeamed() const;

    /// @brief Returns whether this is the start of a primary beam
    bool calcIsBeamStart() const;

    /// @brief Determines if this entry contains a tuplet that creates a singleton beam left.
    /// See #EntryFrame::TupletInfo::calcCreatesSingletonBeamLeft for more information.
    bool calcCreatesSingletonBeamLeft() const;

    /// @brief Determines if this entry contains a tuplet that creates a singleton beam right.
    /// See #EntryFrame::TupletInfo::calcCreatesSingletonBeamRight for more information.
    bool calcCreatesSingletonBeamRight() const;

    /// @brief Determines if this entry continues a beam across a barline from the previous measure.
    ///
    /// @note The Beam Over Barlines plugin has poor support for v1/v2. This function detects v1/v2 correctly on the chance
    /// that a user may have manually setup v1/v2 the way Beam Over Barlines should have.
    ///
    /// @return If the function returns non-null, the return value here is the previous real entry in the beam over a barline,
    /// taking into account tuplets that create faux singleton beams.
    EntryInfoPtr calcBeamContinuesLeftOverBarline() const;

    /// @brief Determines if this entry continues a beam across a barline to the next measure.
    ///
    /// @note The Beam Over Barlines plugin has poor support for v1/v2. This function detects v1/v2 correctly on the chance
    /// that a user may have manually setup v1/v2 the way Beam Over Barlines should have.
    ///
    /// @return If the function returns non-null, the return value here is the next real entry in the beam over a barline,
    /// taking into account tuplets that create faux singleton beams.
    EntryInfoPtr calcBeamContinuesRightOverBarline() const;

    /// @brief Calculates if the entry starts a feathered beam and returns information about it if so.
    /// @param [out] outLeftY The height of the left side of the feathered beam
    /// @param [out] outRightY The height of the right side of the feathered beam
    /// @return true if this is a feathered beam. If the return value is false, outLeftY and outRightY are unchanged.
    bool calcIsFeatheredBeamStart(Evpu& outLeftY, Evpu& outRightY) const;

    /// @brief Finds the first entry of a beamed group or returns the current entry if it is not beams.
    /// @note This behavior is different than other beam functions in that it returns the current entry if there is no beam
    /// rather than returning NULL.
    /// @return The first entry of a beamed group or the current entry if no beam.
    EntryInfoPtr findBeamStartOrCurrent() const;

    /// @brief Finds the end entry of a beamed group.
    /// @return The entry if found, NULL if the entry cannot be beamed or if it is not part of a beamed group.
    EntryInfoPtr findBeamEnd() const;

    /// @brief Calculates the number of beams or flags on the entry.
    unsigned calcNumberOfBeams() const;

    /// @brief Returns the lowest beam number starting at this entry, where 1 = 8th note beam, 2 = 16th note beam, etc.
    /// @param considerBeamOverBarlines If true, consider beams over barlines as created for system breaks by the Beam Over Barlines plugin.
    /// @return 0 if not beamed or no beam starts this entry; otherwise, the beam number
    unsigned calcLowestBeamStart(bool considerBeamOverBarlines = false) const;

    /// @brief Returns the lowest beam number ending at this entry, where 1 = 8th note beam, 2 = 16th note beam, etc.
    /// @return 0 if not beamed or no beam ends this entry; otherwise, the beam number
    unsigned calcLowestBeamEnd() const;

    /// @brief Returns the lowest beam number ending at this entry, where 1 = 8th note beam, 2 = 16th note beam, etc.
    /// This function takes into account beams the cross barlines, as created by the Beam Over Barline plugin.
    /// @return 0 if not beamed or no beam ends this entry; otherwise, the beam number
    unsigned calcLowestBeamEndAcrossBarlines() const;

    /// @brief Returns the lowest beam stub at this entry, where 2 = 16th note stub, 3 = 32nd note stub, etc.
    /// @return 0 if not beamed or no beam stub exists on this entry; otherwise, the lowest beam stub number
    unsigned calcLowestBeamStub() const;

    /// @brief Calculates if a beam stub on this entry would go left or right. It does not check that an entry actually has a beam stub.
    /// Use #calcLowestBeamStub to discover if the entry has a beam stub.
    /// @note This is a best approximation of Finale's behavior for default beam stub direction. No doubt there are edge cases
    /// where it does not match.
    /// @return True if a beam stub would go left; false if it would go right or if no calculation is possible.
    bool calcBeamStubIsLeft() const;

    /// @brief Calculates the elapsed duration in global edu, removing any time stretch due to independent time signature
    util::Fraction calcGlobalElapsedDuration() const;

    /// @brief Calculates the actual duration in global edu, removing any time stretch due to independent time signature
    util::Fraction calcGlobalActualDuration() const;

    /// @brief Determines if this entry can be beamed.
    bool calcCanBeBeamed() const;

    /// @brief Determines if a beam *must* start on this entry.
    bool calcBeamMustStartHere() const;

    /// @brief Returns the entry size as a percentage, taking into account the beaming.
    /// @return Integer percentage where 100 means 100%.
    int calcEntrySize() const;

    /// @brief Calculates if this entry is part of a cue.
    /// @param includeVisibleInScore If true, include cues that are visible in the score.
    /// @return true if
    ///         - the entry is reduced in size
    ///         - the entry is hidden by "Blank Notation" or "Blank Notation with Rests" alternate notation in the score but not in a part.
    bool calcIsCue(bool includeVisibleInScore = false) const;

    /// @brief Returns whether this is a full measure rest.
    /// @note Note that in Finale, only whole rests are used as full measure rests.
    bool calcIsFullMeasureRest() const;

    /// @brief A common workaround in Finale is to hide a rest in v1 and supply it in v2. Typically it is used when a beam starts or ends with
    /// a 16th beam hook, has a 16th rest in the middle and an 8th note on the other end. This code detects that situation.
    /// @return True if this is either the hidden rest in v1.
    bool calcIsBeamedRestWorkaroundHiddenRest() const;

    /// @brief A common workaround in Finale is to hide a rest in v1 and supply it in v2. Typically it is used when a beam starts or ends with
    /// a 16th beam hook, has a 16th rest in the middle and an 8th note on the other end. This code detects that situation.
    /// @return True if this is either the visible replacement rest in v2.
    bool calcIsBeamedRestWorkaroundVisibleRest() const;

    /// @brief Finds the tuplet info for tuplets that include this entry
    /// @return A list of indices of TupletInfo records that include the entry.
    std::vector<size_t> findTupletInfo() const;

    /// @brief Calculates whether the conditions are met for the layer attributes dependent on #others::LayerAttributes::onlyIfOtherLayersHaveNotes.
    /// This also takes into account #others::LayerAttributes::ignoreHiddenNotesOnly and #others::LayerAttributes::ignoreHiddenLayers.
    /// @return true if the layer settings dependent on #others::LayerAttributes::onlyIfOtherLayersHaveNotes are in effect. Otherwise false.
    bool calcIfLayerSettingsApply() const;

    /// @brief Calculates if this entry has cross-staffed notes all in a single direction.
    /// @param staffList Optional staff list used to determine staff order.
    ///        If it is not supplied, the function automatically retrieves the scroll-view staff order
    ///        from the document. Supplying an explicit list can be used to avoid repeatedly fetching the
    ///        staff list when calling this function in a loop (for example, within a beam).
    /// @return
    ///   - **1**  if all cross-staffed notes cross upward to a higher staff.
    ///   - **0**  if the note is not cross-staffed, or if notes are crossed both up and down.
    ///   - **−1** if all cross-staffed notes cross downward to a lower staff.
    int calcCrossStaffDirectionForAll(DeferredReference<MusxInstanceList<others::StaffUsed>> staffList = {}) const;

    /// @brief Return true if this entry is a grace note and the only grace in the sequence at this location.
    bool calcIsSingletonGrace() const;

    /// @brief Return true if this entry is an auxiliary pitch marker (specifically, a trill-to or gliss-to pitch marker.)
    ///
    /// The conditions that must be met are:
    /// - The entry is a singleton grace note.
    /// - The entry has a hidden custom stem.
    int calcIsAuxiliaryPitchMarker() const;

    /// @brief Calculates if this entry is a trill-to entry as created by the Parenthesize Trill-To Notes plugin.
    ///
    /// The conditions that must be met are:
    /// - The entry is an auxiliary pitch marker. (See #calcIsAuxiliaryPitchMarker.)
    /// - The entry has a main entry.
    /// - The entry is positioned at least 1 space to the right of the main entry.
    ///
    /// Note that the main note is not checked for the existence of a trill. Callers should decide on their own
    /// whether this is important and, if so, how to check for it. Finale provides too many different fonts and options
    /// for creating trill markers to reliably check for it in this function.
    bool calcIsTrillToGraceEntry() const;

    /// @brief Calculates if this entry is a gliss-to entry as created by the Parenthesize Trill-To Notes plugin.
    ///
    /// The conditions that must be met are:
    /// - The entry is an auxiliary pitch marker. (See #calcIsAuxiliaryPitchMarker.)
    /// - The entry must be the terminator for one of the standard entry-attached SmartShape gliss lines.
    ///
    /// Only the standard SmartShape gliss lines are checked. Other CustomLine values do no qualify.
    bool calcIsGlissToGraceEntry() const;

    /// @brief Find the hidden source entry for a mid-system beam created by the Beam Over Barline plugin.
    /// This code captures the logic from the Beam Over Barling plugin, allowing the caller to unwind
    /// that plugin's workarounds and detect the entries in a beam that crosses a barline.
    /// @return The hidden source entry if found, otherwise nullptr.
    EntryInfoPtr findHiddenSourceForBeamOverBarline() const;

    /// @brief Find the display entry for a hidden source entry. The display entry is one or more bars previous
    /// to the source entry.
    /// This code captures the logic from the Beam Over Barling plugin, allowing the caller to unwind
    /// that plugin's workarounds and detect the entries in a beam that crosses a barline.
    /// @return The display entry if this is a hidden source entry, otherwise nullptr.
    EntryInfoPtr findDisplayEntryForBeamOverBarline() const;

    /// @brief Finds the main entry for a grace note, taking into account hidden entries for beams over barlines.
    /// @param ignoreRests If true, the returned entry must not be a rest.
    /// @return The main entry if found. If the grace note is at the end of a measure or v2 sequence,
    /// or if ignoring rests and the next non-grace is a rest, returns null. Also returns null if this
    /// is not a grace note.
    EntryInfoPtr findMainEntryForGraceNote(bool ignoreRests = false) const;

    /// @brief Explicit operator< for std::map
    bool operator<(const EntryInfoPtr& other) const
    {
        if (m_entryFrame != other.m_entryFrame)
            return m_entryFrame < other.m_entryFrame;
        return m_indexInFrame < other.m_indexInFrame;
    }

private:
    unsigned calcVisibleBeams() const;

    bool calcUpStemImpl() const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    std::optional<unsigned> iterateFindRestsInSecondaryBeam(const EntryInfoPtr nextOrPrevInBeam) const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    EntryInfoPtr iteratePotentialEntryInBeam() const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    bool iterateNotesExistLeftOrRight() const;

    EntryInfoPtr nextPotentialInBeam(BeamIterationMode beamIterationMode) const;

    EntryInfoPtr previousPotentialInBeam(BeamIterationMode beamIterationMode) const;

    using BeamIteratorFn = EntryInfoPtr (EntryInfoPtr::*)(BeamIterationMode) const;
    template<BeamIteratorFn Iterator, BeamIteratorFn ReverseIterator>
    EntryInfoPtr iterateBeamGroup(BeamIterationMode beamIterationMode) const;

    /// @brief Returns the beam anchor for a beam over barline left. This code captures the logic from the
    /// Beam Over Barling plugin, allowing the caller to unwind that plugin's workarounds and detect the entries
    /// in a beam that crosses a barline.
    EntryInfoPtr findLeftBeamAnchorForBeamOverBarline() const;

    /// @brief Returns the beam anchor for a beam over barline right. This code captures the logic from the
    /// Beam Over Barling plugin, allowing the caller to unwind that plugin's workarounds and detect the entries
    /// in a beam that crosses a barline.
    EntryInfoPtr findRightBeamAnchorForBeamOverBarline() const;

    std::shared_ptr<const EntryFrame> m_entryFrame;
    size_t m_indexInFrame{};              ///< the index of this item in the frame.

    /// @brief Cache the staff for this entry here to avoid repeated calls to `StaffComposite::createCurrent` for the same information.
    mutable MusxInstance<others::StaffComposite> m_cachedStaff;

    /// @brief Cache the stem direction for this entry to avoid repeatedly calculating it.
    mutable std::optional<bool> m_upStem;
};

struct EntryInfoPtr::BeamedRestWorkaroundAwareResult
{
    EntryInfoPtr entry;         ///< The entry found, or null if there is no usable entry.
    bool effectiveHidden{};     ///< True if the entry should be treated as effectively hidden.

    /// @brief Allows the result to be used directly in boolean contexts.
    /// @return true if @ref entry is non-null; false otherwise.
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(entry);
    }
};

/**
 * @class EntryFrame
 * @brief Represents a vector of @ref EntryInfo instances for a given frame, along with computed information.
 *
 * Its pointers are owned by @ref EntryInfoPtr
 */
class EntryFrame : public std::enable_shared_from_this<EntryFrame>
{
public:
    /** @brief Constructor function
     *
     * @param gfhold The @ref details::GFrameHoldContext instance creating this EntryFrame.
     * @param layerIndex The @ref LayerIndex (0..3) of the entry frame.
     * @param timeStretch The ratio of global Edu to staff edu.
     * @param startStaff The staff at the start of the measure containing this EntryFrame.
    */
    explicit EntryFrame(const details::GFrameHoldContext& gfhold, LayerIndex layerIndex, util::Fraction timeStretch, const MusxInstance<others::StaffComposite>& startStaff) :
        m_context(gfhold),
        m_layerIndex(layerIndex),
        m_timeStretch(timeStretch),
        m_startStaff(startStaff)
    {
    }

    /// @brief class to track tuplets in the frame
    struct TupletInfo
    {
        MusxInstance<details::TupletDef> tuplet;  ///< the tuplet
        size_t startIndex;                              ///< the index of the first entry in the tuplet
        size_t endIndex;                                ///< the index of the last entry in the tuplet
        util::Fraction startDura;                       ///< the actual duration where the tuplet starts
        util::Fraction endDura;                         ///< the actual duration where the tuplet ends
        bool voice2;                                    ///< whether this tuplet is for voice2

        /// @brief Constructor
        TupletInfo(const std::weak_ptr<const EntryFrame>& parent, const MusxInstance<details::TupletDef>& tup, size_t index, util::Fraction start, bool forVoice2)
            : tuplet(tup), startIndex(index), endIndex((std::numeric_limits<size_t>::max)()),
                startDura(start), endDura(-1), voice2(forVoice2), m_parent(parent)
        {}

        /// @brief Return the number of entries in the tuplet
        size_t numEntries() const
        {
            MUSX_ASSERT_IF(startIndex > endIndex) {
                throw std::logic_error("TupletInfo has invalid start and end indices.");
            }
            return endIndex - startIndex + 1;
        }

        /// @brief Return true if the entry is part of this tuplet.
        /// @param entryInfo The entry to check.
        bool includesEntry(const EntryInfoPtr& entryInfo) const
        {
            const size_t x = entryInfo.getIndexInFrame();
            return (x >= startIndex && x <= endIndex);
        }

        /// @brief Calculates if this tuplet represents a tremolo based on the following criteria.
        ///     - the tuplet ratio is a positive integral power of 2.
        ///     - the tuplet contains exactly 2 entries of equal duration and actual duration.
        ///     - the tuplet is invisible.
        ///
        /// @note The TGTools Tremolo plugin always creates beam extensions for both upstem and downstem cases. To detect the type of
        /// stem connections for the tremolo, it is recommended to look for either an upstem or a downstem extension. This
        /// covers the TGTools plugin as well as any that might have been created by hand. See @ref details::BeamExtension.
        ///
        /// @return true if the tuplet is a tremolo. If so, use `EntryInfoPtr::calcNumberOfBeams` on either entry to determine
        /// the number of beams. Use `details::TupletDef::calcReferenceDuration` to get the total length of the tremolo.
        bool calcIsTremolo() const;

        /// @brief Calculates if this tuplet is being used to create a singleton beam to the right.
        ///
        /// Finale has no built-in support for beams on singleton notes. As a workaround, users and (especially)
        /// plugins such as Beam Over Barline create singleton beams using a 0-length tuplet and hiding either the tuplet
        /// note or its next neighbor, depending on whether the beam goes to the left or the right. You should never
        /// encounter a 0-length tuplet encompassing more than one entry, but these functions guarantee this if they return `true`.
        ///
        /// @return True if this tuplet creates a singleton beam to the right. You may handle this as follows.
        ///     - The entry with the tuplet is the visible entry to use. You can mark this entry as having a singleton beam right, if your application allows it.
        ///     - Ignore the tuplet on the visible entry. If you need the entry's actual duration in context, its next neighbor in the same voice
        /// has the correct value.
        ///     - Ignore the entry's next neighbor in the same voice. It will have its leger lines suppressed and non-visible notehead(s) and stem.
        /// Its `hidden` flag, however, will still be false. (This function guarantees these conditions if it returns `true`.)
        bool calcCreatesSingletonBeamRight() const { return calcCreatesSingleton(false); }

        /// @brief Calculates if this tuplet is being used to create a singleton beam to the left.
        ///
        /// See comments at #calcCreatesSingletonBeamRight.
        ///
        /// @return True if this tuplet creates a singleton beam to the left. You may handle this as follows.
        ///     - Skip the entry and its tuplet.
        ///     - You can mark the next entry in the same voice as having a singleton beam left, if your application allows it.
        ///     - The current entry with the 0-length tuplet will have its leger lines suppressed and non-visible notehead(s) and stem.
        /// Its `hidden` flag, however, will still be false. (This function guarantees these conditions if it returns `true`.)
        bool calcCreatesSingletonBeamLeft() const { return calcCreatesSingleton(true); }

        /// @brief Detects tuplets being used to create time stretch in an independent time signature.
        ///
        /// Because the Finale UI is so buggy with smart shapes and clefs when there is an Independent Time Signature,
        /// a common workaround is to set the Independent Time Signature to the same as the global time signature but
        /// display a different time signature. Invisible tuplets then create the appearance of time stretch.
        ///
        /// @return This function returns true if
        ///     - the tuplet's total reference duration matches the length of the measure exactly 
        ///     - the tuplet is invisible
        ///     - the staff has an independent time signature
        //      - the independent display time signature matches the tuplet's total display duration
        bool calcCreatesTimeStretch() const;

    private:
        bool calcCreatesSingleton(bool left) const;

        const std::shared_ptr<const EntryFrame> getParent() const
        {
            auto result = m_parent.lock();
            MUSX_ASSERT_IF(!result) {
                throw std::logic_error("Unable to obtain lock on parent entry frame.");               
            }
            return result;
        }

        const std::weak_ptr<const EntryFrame> m_parent;
    };

    /** @brief A list of the tuplets in the frame and their calculated starting and ending information.
     *
     * @note Tuplets that start on grace notes are omitted from this list. Finale does not display them, and
     * it is not possible to calculate their endpoints correctly in the general case. (Which is probably why
     * Finale does not display them.)
    */
    std::vector<TupletInfo> tupletInfo;
    MusxInstance<KeySignature> keySignature;    ///< This can be different than the measure key sig if the staff has independent key signatures.
    util::Fraction measureStaffDuration;        ///< The duration of the measure in staff duration units.
    util::Fraction maxElapsedStaffDuration;     ///< The max elapsed staff duration that was calculated for the frame. This does not
                                                ///< have to equal the measure duration, but normally it does.

    /// @brief Get the document for the entry frame
    DocumentPtr getDocument() const;

    /// @brief Get the frame context for this frame
    const details::GFrameHoldContext& getContext() const { return m_context; }

    /// @brief Get the requested part ID for the entry frame
    Cmper getRequestedPartId() const { return m_context.getRequestedPartId(); }

    /// @brief Get the staff for the entry
    StaffCmper getStaff() const;

    /// @brief Get the measure for the entry frame
    MeasCmper getMeasure() const;

    /// @brief Get the layer index (0..3) of the entry frame
    LayerIndex getLayerIndex() const { return m_layerIndex; }

    /// @brief Get the LayerAttributes for this entry frame.
    MusxInstance<others::LayerAttributes> getLayerAttributes() const;

    /// @brief Get the time stretch in this frame. Rather than accessing this value directly,
    /// consider using #EntryInfoPtr::calcGlobalElapsedDuration or #EntryInfoPtr::calcGlobalActualDuration instead.
    util::Fraction getTimeStretch() const { return m_timeStretch; }

    /// @brief Get the entry list.
    const std::vector<std::shared_ptr<const EntryInfo>>& getEntries() const
    { return m_entries; }

    /// @brief Returns the first entry in the specified v1/v2 or null if none.
    ///
    /// @param voice  Must be 1 or 2.
    EntryInfoPtr getFirstInVoice(int voice) const;

    /// @brief Returns the last entry in the specified v1/v2 or null if none.
    ///
    /// @param voice  Must be 1 or 2.
    EntryInfoPtr getLastInVoice(int voice) const;

    /// @brief Returns the first entry in the specified voice using musxdom's
    ///        beamed-rest workaround interpretation.
    ///
    /// This function begins forward traversal at the first raw entry for the given voice
    /// (voice 1 or 2).  It interprets user-created beamed-rest workarounds commonly found
    /// in Finale documents.  These workarounds hide or insert additional rests solely to
    /// influence beam shapes.
    ///
    /// The following rules are applied when selecting the returned entry:
    ///
    /// - Extra visible voice-2 workaround rests inserted only to display a beam correctly
    ///   are **skipped entirely**.
    /// - Hidden voice-1 workaround rests used to enforce a beam shape are **returned**
    ///   and treated as visible, with #EntryInfoPtr::BeamedRestWorkaroundAwareResult::effectiveHidden set to @c false.
    /// - All remaining entries are returned with #EntryInfoPtr::BeamedRestWorkaroundAwareResult::effectiveHidden
    ///   matching their stored @c isHidden value.
    ///
    /// If no usable entry remains after applying these rules, the returned
    /// @ref BeamedRestWorkaroundAwareResult will have a null @ref BeamedRestWorkaroundAwareResult::entry.
    ///
    /// @param voice  Must be 1 or 2.
    /// @return A BeamedRestWorkaroundAwareResult containing the selected entry (or null) and its
    ///         effective-hidden flag.
    EntryInfoPtr::BeamedRestWorkaroundAwareResult getFirstInVoiceBeamedRestWorkaroundAware(int voice) const;

    /// @brief Add an entry to the list.
    void addEntry(const std::shared_ptr<const EntryInfo>& entry)
    { m_entries.emplace_back(entry); }

    /// @brief Gets the entry frame for the next measure with the same staff and layer.
    /// @return Frame or nullpter if the next measure has no matching frame.
    std::shared_ptr<const EntryFrame> getNext() const;

    /// @brief Gets the entry frame for the previous measure with the same staff and layer.
    /// @return Frame or nullpter if the previous measure has no matching frame,
    std::shared_ptr<const EntryFrame> getPrevious() const;

    /// @brief Gets the staff at eduPosition 0 without needing to create it again.
    MusxInstance<others::StaffComposite> getStartStaffInstance() const
    { return m_startStaff; }

    /// @brief Creates a current StaffComposite for the entry frame.
    /// @param eduPosition The Edu position for which to create the staff.
    /// @param forStaffId Specifies optional staff ID. If supplied, it overrides the entry's staff ID. (Useful when notes are cross-staffed.)
    MusxInstance<others::StaffComposite> createCurrentStaff(Edu eduPosition, const std::optional<StaffCmper>& forStaffId = std::nullopt) const;

    /// @brief Get the measure instance
    MusxInstance<others::Measure> getMeasureInstance() const;

    /// @brief Calculates if this entry frame is part of a cue.
    /// @todo Revisit this algorithm if needed. The current algorithm is chosen to be mostly accurate while being
    /// fast to compute when there is no cue.
    /// @param includeVisibleInScore If true, include cues that are visible in the score.
    /// @return true if all entries in the frame are either cue entries or hidden.
    bool calcIsCueFrame(bool includeVisibleInScore = false) const;

    /// @brief Calculates if this all notes in the frame are hidden.
    /// This routine only checks that entries are individually hidden. The caller must separately check #others::Staff::calcAlternateNotationHidesEntries
    /// according to its specific needs. For example, to determine if a layer is hidden for the purposes of checking validity of layer attributes,
    /// only the staff at edu position 0 should be checked.
    /// @return true if all entries in the frame are hidden.
    bool calcAreAllEntriesHiddenInFrame() const;

    /// @brief Calculates the nearest non-grace-note entry at the given @p position.
    /// @param position The measure position to find.
    /// @param findExact If true, only find an entry that matches to within 1 evpu. Otherwise find the closest entry in the measure.
    /// @param matchVoice2 If specified, the value of #Entry::voice2 must match the specified value.
    /// @param atGraceNoteDuration Match on this grace note duration. When it is zero, grace notes are skipped.
    /// @return The entry if found, otherwise `nullptr`.
    EntryInfoPtr calcNearestEntry(util::Fraction position, bool findExact = true, std::optional<bool> matchVoice2 = std::nullopt,
        util::Fraction atGraceNoteDuration = 0) const;

    /// @brief Iterates the entries for the specified layer in this EntryFrame from left to right.
    /// @param iterator The callback function for each iteration.
    /// @return true if the caller should continue iteration; false to stop early.
    /// @throws std::invalid_argument if the layer index is out of range.
    bool iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator) const;

private:
    details::GFrameHoldContext m_context;
    LayerIndex m_layerIndex;
    util::Fraction m_timeStretch;

    std::vector<std::shared_ptr<const EntryInfo>> m_entries;

    /// @brief Cache the start staff to avoid getting it again every time it is needed.
    MusxInstance<others::StaffComposite> m_startStaff;

    /// @brief Cache the layer attributes to avoid constantly re-retrieving them.
    mutable MusxInstance<others::LayerAttributes> m_cachedLayerAttributes;
};

namespace details {
class GFrameHold;
} // namespace details

/**
 * @class EntryInfo
 * @brief Information an entry along with the entry.
 *
 * This class is used in iteration functions to supply information about the entry along with the entry itself.
 *
 * Its pointers are owned by @ref EntryFrame.
 */
class EntryInfo
{
    /** @brief Constructor function
     *
     * @param entry The entry.
    */
    explicit EntryInfo(const MusxInstance<Entry>& entry)
        : m_entry(entry) {}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
    friend details::GFrameHoldContext;
#endif

public:
    util::Fraction elapsedDuration{};   ///< the elapsed duration within the measure where this entry occurs (in fractions of a whole note)
                                        ///< This is a staff-level position and must be scaled for the global value. (Use #EntryInfoPtr::calcGlobalElapsedDuration.)
    util::Fraction actualDuration{};    ///< the actual duration of entry (in fractions of a whole note), taking into account tuplets and grace notes
                                        ///< This is a staff-level value and must be scaled for the global value. (Use #EntryInfoPtr::calcGlobalActualDuration.)
    unsigned graceIndex{};              ///< the Finale grace note index, counting from 1 starting from the leftmost grace note counting rightward.
                                        ///< the main note has a grace index of zero.
    ClefIndex clefIndex{};              ///< the clef index in effect for the entry.
    ClefIndex clefIndexConcert{};       ///< the concert clef index in effect for the entry.

    /// @brief Get the entry
    /// @throws std::logic_error if the entry pointer is no longer valid 
    MusxInstance<Entry> getEntry() const
    {
        auto retval = m_entry.lock();
        if (!retval) {
            throw std::logic_error("Entry pointer is no longer valid");
        }
        return retval;
    }

    /// @brief Calculates the next duration position after this entry
    util::Fraction calcNextElapsedDuration() const
    { return elapsedDuration + actualDuration; }

private:
    MusxInstanceWeak<Entry> m_entry;
};

/// @brief Wraps an @ref EntryInfo instance and a note index.
class NoteInfoPtr
{
public:
    /// @brief Default constructor
    NoteInfoPtr() : m_entry(), m_noteIndex(0) {}

    /// @brief Constructor
    /// @param entryInfo The entry info containing the note.
    /// @param noteIndex The index of this note within #Entry::notes.
    NoteInfoPtr(const EntryInfoPtr& entryInfo, size_t noteIndex)
        : m_entry(entryInfo), m_noteIndex(noteIndex)
    {}

    /// @brief Provides a boolean conversion based on whether the EntryInfoPtr is valid and the note index is valid.
    explicit operator bool() const noexcept
    { return m_entry && m_noteIndex < m_entry->getEntry()->notes.size(); }

    /// @brief Returns whether the input and the current instance refer to the same note.
    /// @param src The EntryInfoPtr to compare with.
    bool isSameNote(const NoteInfoPtr& src) const
    { return m_entry.isSameEntry(src.m_entry) && m_noteIndex == src.m_noteIndex; }

    /// @brief Finds a note with the same pitch in the supplied entry
    /// @param entry the entry to search
    /// @return The found note or an null instance of NoteInfoPtr.
    NoteInfoPtr findEqualPitch(const EntryInfoPtr& entry) const;

    /// @brief Allows `->` access to the underlying @ref Note instance.
    MusxInstance<Note> operator->() const
    {
        MUSX_ASSERT_IF(m_noteIndex >= m_entry->getEntry()->notes.size()) {
            throw std::logic_error("Note index is too large for notes array.");
        }
        return m_entry->getEntry()->notes[m_noteIndex];
    }

    /// @brief Gets the entry info for this note
    EntryInfoPtr getEntryInfo() const
    { return m_entry; }

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position. This function does
     * not take into account percussion notes and their staff position override. To get the staff position taking
     * into account percussion notes, use #calcStaffPosition.
     * @note In Finale, the default whole rest staff position is the middle staff line. Other music notation systems
     * frequently expect the standard whole rest staff position to be the second line from the top. You may need to interpolate
     * the staff position returned by #calcNoteProperties for whole rests.
     * @param enharmonicRespell If supplied, return the default enharmonic respelling based on this value. If omitted,
     * this value is calculated automatically based on the score or part settings. Normally you will omit it.
     * @param alwaysUseEntryStaff If true, the entry is not checked for cross-staff staffing. Normally you omit this.
     * @return #Note::NoteProperties
     */
    Note::NoteProperties calcNoteProperties(const std::optional<bool>& enharmonicRespell = std::nullopt, bool alwaysUseEntryStaff = false) const;

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position for the concert pitch of the note. This function does
     * not take into account percussion notes and their staff position override. To discover if a note is a percussion
     * note, call #calcPercussionNoteInfo. If it returns non-null, use that for staff position instead of this function.
     * @param alwaysUseEntryStaff If true, the entry is not checked for cross-staff staffing. Normally you omit this.
     * @return #Note::NoteProperties
     */
    Note::NoteProperties calcNotePropertiesConcert(bool alwaysUseEntryStaff = false) const;

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position for the pitch of the note in view. This may be
     * particularly useful with non-floating rests, but it can be used with any note. As with other versions of the function, it does not
     * handle the staff position override of percussion notes.
     * @param alwaysUseEntryStaff If true, the entry is not checked for cross-staff staffing. Normally you omit this.
     * @return #Note::NoteProperties
     */
    Note::NoteProperties calcNotePropertiesInView(bool alwaysUseEntryStaff = false) const;

    /// @brief Calculates the percussion note info for this note, if any.
    /// @return If the note is on a percussion staff and has percussion note info assigned, returns it. Otherwise `nullptr`.
    MusxInstance<others::PercussionNoteInfo> calcPercussionNoteInfo() const;

    /// @brief Calculates the staff position for this note, taking into account percussion notes.
    /// @return 
    int calcStaffPosition() const;

    /// @brief Calculates the note that this note could tie to. Check the return value's #Note::tieEnd
    /// to see if there is actually a tie end. (Note that Finale shows a tie whether there #Note::tieEnd is true or not.)
    /// @return The candidate note or an empty NoteInfoPtr if no candidate was found.
    NoteInfoPtr calcTieTo() const;

    /// @brief Calculates the note that this note could tie from.
    /// @param requireTie If @p requireTie is true, the returned value must have its #Note::tieStart flag set to true.
    /// You can set @p requireTie to false to find the *potential* note this note might be tied from.
    /// @return The candidate note or an empty NoteInfoPtr if no candidate was found.
    NoteInfoPtr calcTieFrom(bool requireTie = true) const;

    /// @brief Calculates the staff number, taking into account cross staffing
    StaffCmper calcStaff() const;

    /// @brief Creates a transposer for this Note instance.
    /// @return A unique pointer to a transposer for this Note.
    std::unique_ptr<music_theory::Transposer> createTransposer() const;

    /// @brief Gets the next note in a chord on the same entry.
    /// @return The next note or nullptr if none.
    NoteInfoPtr getNext() const
    {
        if (m_noteIndex >= m_entry->getEntry()->notes.size()) {
            return NoteInfoPtr();
        }
        return NoteInfoPtr(m_entry, m_noteIndex + 1);
    }

    /// @brief Gets the next note in a chord on the same entry.
    /// @return The next note or nullptr if none.
    NoteInfoPtr getPrevious() const
    {
        if (m_noteIndex <= 0) {
            return NoteInfoPtr();
        }
        return NoteInfoPtr(m_entry, m_noteIndex - 1);
    }

    /// @brief Returns if this note is enharmonically respelled in the current part view
    bool calcIsEnharmonicRespell() const;

    /// @brief Calculates the default enharmonic equivalent of this note. This is the value that Finale uses when
    /// #details::NoteAlterations::enharmonic is true.
    ///
    /// Normally you do not have to call this function directly. It is called inside #calcNoteProperties. But the function
    /// is available if you need it.
    ///
    /// @return A std::pair containing
    ///         - int: the enharmonic equivalent's displacement value relative to the tonic.
    ///         - int: the enharmonic equivalent's alteration value relative to the key signature.
    std::pair<int, int> calcDefaultEnharmonic() const
    { return (*this)->calcDefaultEnharmonic(m_entry.getKeySignature()); }

    /// @brief Calculates if this note is cross-staffed and if so, which direction.
    /// @param staffList Optional staff list used to determine staff order.
    ///        If it is not supplied, the function automatically retrieves the scroll-view staff order
    ///        from the document. Supplying an explicit list can be used to avoid repeatedly fetching the
    ///        staff list when calling this function in a loop (for example, within a beam).
    /// @return
    ///   - **1**  if the note crosses upward to a higher staff  
    ///   - **0**  if the note is not cross-staffed  
    ///   - **−1** if the note crosses downward to a lower staff
    int calcCrossStaffDirection(DeferredReference<MusxInstanceList<others::StaffUsed>> staffList = {}) const;

private:
    /// @brief Returns true if the two notes represent the same concert pitch or
    /// percussion note.
    /// @param src the value to compare with.
    bool isSamePitch(const NoteInfoPtr& src) const;

    /// @brief Returns true if the @p src and this have the same pitch information.
    /// It is only meaningful when this and src are in the same key.
    /// @param src the value to compare with.
    bool isSamePitchValues(const NoteInfoPtr& src) const;

    EntryInfoPtr m_entry;
    size_t m_noteIndex;
};

} // namespace dom
} // namespace entries