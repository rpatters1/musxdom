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

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace dom {

using namespace ::musx::xml;
using namespace ::musx::factory;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

// XML mappings for the Note class
MUSX_XML_ELEMENT_ARRAY(Note, {
    {"harmLev", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->harmLev = e->getTextAs<int>(); }},
    {"harmAlt", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->harmAlt = e->getTextAs<int>(); }},
    {"isValid", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->isValid = populateBoolean(e, i); }},
    {"crossStaff", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->crossStaff = populateBoolean(e, i); }},
    {"tieStart", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->tieStart = populateBoolean(e, i); }},
    {"tieEnd", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->tieEnd = populateBoolean(e, i); }},
    {"showAcci", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->showAcci = populateBoolean(e, i); }},
    {"freezeAcci", [](const XmlElementPtr& e, const std::shared_ptr<Note>& i) { i->freezeAcci = populateBoolean(e, i); }},
});

// XML mappings for the Entry class
MUSX_XML_ELEMENT_ARRAY(Entry, {
    {"dura", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->duration = e->getTextAs<Edu>(); }},
    {"numNotes", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->numNotes = e->getTextAs<int>(); }},
    {"isValid", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isValid = populateBoolean(e, i); }},
    {"isNote", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isNote = populateBoolean(e, i); }},
    {"graceNote", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->graceNote = populateBoolean(e, i); }},
    {"floatRest", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->floatRest = populateBoolean(e, i); }},
    {"ignore", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->isHidden = populateBoolean(e, i); }},
    {"v2", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->voice2 = populateBoolean(e, i); }},
    {"articDetail", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->articDetail = populateBoolean(e, i); }},
    {"noteDetail", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noteDetail = populateBoolean(e, i); }},
    {"beam", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->beam = populateBoolean(e, i); }},
    {"secBeam", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->secBeam = populateBoolean(e, i); }},
    {"crossStaff", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->crossStaff = populateBoolean(e, i); }},
    {"freezeStem", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->freezeStem = populateBoolean(e, i); }},
    {"upStem", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->upStem = populateBoolean(e, i); }},
    {"noLeger", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->noLeger = populateBoolean(e, i); }},
    {"stemDetail", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->stemDetail = populateBoolean(e, i); }},
    {"smartShapeDetail", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->smartShapeDetail = populateBoolean(e, i); }},
    {"sorted", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->sorted = populateBoolean(e, i); }},
    {"lyricDetail", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->lyricDetail = populateBoolean(e, i); }},
    {"performanceData", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->performanceData = populateBoolean(e, i); }},
    {"freezeBeam", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) { i->freezeBeam = populateBoolean(e, i); }},
    {"note", [](const XmlElementPtr& e, const std::shared_ptr<Entry>& i) {
        auto noteAttr = e->findAttribute("id");
        if (!noteAttr) {
            throw std::invalid_argument("Note in entry " + std::to_string(i->getEntryNumber()) + " has no id attribute.");
        }
        i->notes.push_back(FieldPopulator<Note>::createAndPopulate(e, i->getDocument(), noteAttr->getValueAs<NoteNumber>()));
    }},
});


} // namespace dom
} // namespace musx


#endif // DOXYGEN_SHOULD_IGNORE_THIS
