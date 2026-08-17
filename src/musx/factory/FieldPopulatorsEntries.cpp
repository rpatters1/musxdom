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
#include "musx/musx.h"
#include "musx/factory/FieldPopulatorsEntries.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace dom {

using namespace ::musx::xml;
using namespace ::musx::factory;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

// XML mappings for the Note class
MUSX_XML_ELEMENT_ARRAY(Note, {
    {"harmLev", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->harmLev = e->getTextAs<int>(); }},
    {"harmAlt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->harmAlt = e->getTextAs<int>(); }},
    {"isValid", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->isValid = populateBoolean(c, e, i); }},
    {"tieStart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->tieStart = populateBoolean(c, e, i); }},
    {"tieEnd", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->tieEnd = populateBoolean(c, e, i); }},
    {"crossStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->crossStaff = populateBoolean(c, e, i); }},
    {"upStemSecond", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->upStemSecond = populateBoolean(c, e, i); }},
    {"downStemSecond", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->downStemSecond = populateBoolean(c, e, i); }},
    {"upSplitStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->upSplitStem = populateBoolean(c, e, i); }},
    {"showAcci", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->showAcci = populateBoolean(c, e, i); }},
    {"parenAcci", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->parenAcci = populateBoolean(c, e, i); }},
    {"noPlayback", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->noPlayback = populateBoolean(c, e, i); }},
    {"noSpacing", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->noSpacing = populateBoolean(c, e, i); }},
    {"freezeAcci", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->freezeAcci = populateBoolean(c, e, i); }},
    {"playDisabledByHP", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->playDisabledByHP = populateBoolean(c, e, i); }},
});

// XML mappings for the Entry class
MUSX_XML_ELEMENT_ARRAY(Entry, {
    {"dura", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->duration = e->getTextAs<Edu>(); }},
    {"numNotes", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->numNotes = e->getTextAs<int>(); }},
    {"posi", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->hOffsetScore = e->getTextAs<Evpu>(); }},
    {"isValid", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isValid = populateBoolean(c, e, i); }},
    {"isNote", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isNote = populateBoolean(c, e, i); }},
    {"controller", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->v2Launch = populateBoolean(c, e, i); }},
    {"v2", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->voice2 = populateBoolean(c, e, i); }},
    {"createdByHP", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->createdByHP = populateBoolean(c, e, i); }},
    {"playDisabledByHP", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->playDisabledByHP = populateBoolean(c, e, i); }},
    {"graceNote", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->graceNote = populateBoolean(c, e, i); }},
    {"noteDetail", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noteDetail = populateBoolean(c, e, i); }},
    {"articDetail", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->articDetail = populateBoolean(c, e, i); }},
    {"lyricDetail", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->lyricDetail = populateBoolean(c, e, i); }},
    {"tupletStart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->tupletStart = populateBoolean(c, e, i); }},
    {"splitRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->splitRest = populateBoolean(c, e, i); }},
    {"performanceData", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->performanceData = populateBoolean(c, e, i); }},
    {"floatRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->floatRest = populateBoolean(c, e, i); }},
    {"ignore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isHidden = populateBoolean(c, e, i); }},
    {"beamExt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->beamExt = populateBoolean(c, e, i); }},
    {"flipTie", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->flipTie = populateBoolean(c, e, i); }},
    {"dotTieAlt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->dotTieAlt = populateBoolean(c, e, i); }},
    {"beam", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->beam = populateBoolean(c, e, i); }},
    {"secBeam", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->secBeam = populateBoolean(c, e, i); }},
    {"freezeStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->freezeStemScore = populateBoolean(c, e, i); }},
    {"stemDetail", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->stemDetail = populateBoolean(c, e, i); }},
    {"crossStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->crossStaff = populateBoolean(c, e, i); }},
    {"reverseUpStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->reverseUpStem = populateBoolean(c, e, i); }},
    {"reverseDownStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->reverseDownStem = populateBoolean(c, e, i); }},
    {"doubleStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->doubleStem = populateBoolean(c, e, i); }},
    {"splitStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->splitStem = populateBoolean(c, e, i); }},
    {"upStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->upStemScore = populateBoolean(c, e, i); }},
    {"checkAccis", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->checkAccis = populateBoolean(c, e, i); }},
    {"dummy", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->dummy = populateBoolean(c, e, i); }},
    {"smartShapeDetail", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->smartShapeDetail = populateBoolean(c, e, i); }},
    {"noLeger", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noLeger = populateBoolean(c, e, i); }},
    {"sorted", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->sorted = populateBoolean(c, e, i); }},
    {"slashGrace", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->slashGrace = populateBoolean(c, e, i); }},
    {"flatBeam", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->flatBeam = populateBoolean(c, e, i); }},
    {"noPlayback", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noPlayback = populateBoolean(c, e, i); }},
    {"noSpacing", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noSpacing = populateBoolean(c, e, i); }},
    {"freezeBeam", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->freezeBeam = populateBoolean(c, e, i); }},
    {"note", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Entry>& i) {
        auto noteAttr = e->findAttribute("id");
        if (!noteAttr) {
            throw std::invalid_argument("Note in entry " + std::to_string(i->getEntryNumber()) + " has no id attribute.");
        }
        i->notes.push_back(FieldPopulator<Note>::createAndPopulate(c, e, i->getDocument(), noteAttr->getValueAs<NoteNumber>()));
    }},
});

} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS
