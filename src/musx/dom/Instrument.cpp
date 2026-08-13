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

#include "musx/dom/Instrument.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <utility>

#include "musx/dom/InstrumentUuids.h"
#include "musx/musx.h"

namespace musx {
namespace dom {

namespace {

struct InstrumentFamilyData
{
    InstrumentFamily family{};
    SoloOrEnsemble soloOrEnsemble{};
};

const std::unordered_map<InstrumentUuid, InstrumentFamilyData>& instrumentFamilyMap()
{
    // Keep this table exhaustive with InstrumentUuids.h so new Finale UUIDs are easy to audit.
    // Solo/ensemble classifications come from Finale 27's bundled MusicXML sounds.xml.
    static const std::unordered_map<InstrumentUuid, InstrumentFamilyData> result = {
        { uuid::BlankStaff, { InstrumentFamily::Unspecified, SoloOrEnsemble::Unspecified } },
        { uuid::BlankStaff2, { InstrumentFamily::Unspecified, SoloOrEnsemble::Unspecified } },
        { uuid::GrandStaff, { InstrumentFamily::Keyboard, SoloOrEnsemble::Unspecified } },
        { uuid::Unknown, { InstrumentFamily::Unspecified, SoloOrEnsemble::Unspecified } },
        { uuid::MusicXmlDrumGroup, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::MusicXmlDrumGroupSet, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::MusicXmlTabor, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlAccordion, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlSuspendedCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::MusicXmlHandchimes, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlMusicBox, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlCavaquinho, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlVocals, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlCalliope, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::MusicXmlAlbogue, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Violin, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Viola, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Cello, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::DoubleBass, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::ViolinSection, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::ViolaSection, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::CelloSection, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::VioloncelloSection, { InstrumentFamily::Strings, SoloOrEnsemble::Unspecified } },
        { uuid::DoubleBassSection, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::ContrabassSection, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::StringEnsemble, { InstrumentFamily::Strings, SoloOrEnsemble::Ensemble } },
        { uuid::ViolaDAmore, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Ajaeng, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Arpeggione, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Baryton, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::ByzantineLyra, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::CretanLyra, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Crwth, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Dahu, { InstrumentFamily::Strings, SoloOrEnsemble::Unspecified } },
        { uuid::Dangao, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Dihu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Erhu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Erxian, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Fiddle, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Gaohu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Gehu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Haegeum, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::HardangerFiddle, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::HurdyGurdy, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Igil, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Kamancha, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Kokyu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Kora, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::LaruAn, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Leiqin, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Lirone, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::MorinKhuur, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Nyckelharpa, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Octobass, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Rebab, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Rebec, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Sarangi, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::SarangiDrone, { InstrumentFamily::Strings, SoloOrEnsemble::Unspecified } },
        { uuid::StrohViolin, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Trombamarina, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Vielle, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Viol, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::ViolaDaGamba, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::ViolinoPiccolo, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::VioloncelloPiccolo, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Violotta, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Zhonghu, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Piano, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::PianoNoName, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Harpsichord, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Organ, { InstrumentFamily::Organ, SoloOrEnsemble::Solo } },
        { uuid::Organ2Staff, { InstrumentFamily::Organ, SoloOrEnsemble::Unspecified } },
        { uuid::Celesta, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Accordion, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Melodica, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ElectricPiano, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Clavinet, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::SynthPad, { InstrumentFamily::Synth, SoloOrEnsemble::Solo } },
        { uuid::SynthLead, { InstrumentFamily::Synth, SoloOrEnsemble::Solo } },
        { uuid::SynthBrass, { InstrumentFamily::Brass, SoloOrEnsemble::Ensemble } },
        { uuid::SynthSoundtrack, { InstrumentFamily::Synth, SoloOrEnsemble::Solo } },
        { uuid::SoundFX, { InstrumentFamily::Synth, SoloOrEnsemble::Solo } },
        { uuid::Harmonium, { InstrumentFamily::Organ, SoloOrEnsemble::Solo } },
        { uuid::OndesMartenot, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Theremin, { InstrumentFamily::Synth, SoloOrEnsemble::Solo } },
        { uuid::Virginal, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Clavichord, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::SopranoVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::AltoVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::TenorVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::BaritoneVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::BassVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::Vocals, { InstrumentFamily::Voice, SoloOrEnsemble::Ensemble } },
        { uuid::Voice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::VoiceNoName, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::MezzoSopranoVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::ContraltoVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::CountertenorVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::BassBaritoneVoice, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::ChoirAahs, { InstrumentFamily::Voice, SoloOrEnsemble::Ensemble } },
        { uuid::ChoirOohs, { InstrumentFamily::Voice, SoloOrEnsemble::Ensemble } },
        { uuid::Yodel, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::Beatbox, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::Kazoo, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::Talkbox, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::VocalPercussion, { InstrumentFamily::Voice, SoloOrEnsemble::Solo } },
        { uuid::Piccolo, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Flute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AltoFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Oboe, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::OboeDAmore, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::EnglishHorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ClarinetBFlat, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ClarinetA, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ClarinetEFlat, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AltoClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ContraltoClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ContrabassClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Bassoon, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Contrabassoon, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::WindSection, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Ensemble } },
        { uuid::SopranoSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AltoSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TenorSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BaritoneSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::SopranoRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::SopraninoRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AltoRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TenorRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::DescantRecorder, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Ocarina, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PennyWhistle, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PennyWhistleD, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PennyWhistleG, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::LowIrishWhistle, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TinWhistleBFlat, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Harmonica, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassHarmonica, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Concertina, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::Bandoneon, { InstrumentFamily::Keyboard, SoloOrEnsemble::Solo } },
        { uuid::HornF_WWQuintet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Bagpipes, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::UilleannPipes, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::GaidaPipes, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ContraAltoFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ContrabassFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::DoubleContrabassFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::HyperbassFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::PanPipes, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Fife, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BottleBlow, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Jug, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PiccoloOboe, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PiccoloHeckelphone, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Heckelphone, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassOboe, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassetClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassetHorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hornpipe, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PiccoloClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Saxonette, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::SopraninoSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::MezzoSopranoSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sopranino, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::CMelodySax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Aulochrome, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Xaphoon, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::BassSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ContrabassSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::SubContrabassSax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Tubax, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Bansuri, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Danso, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Dizi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::DilliKaval, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Diple, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::DoubleFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Dvojnice, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::DvojniceDrone, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Flageolet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Fujara, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Gemshorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hocchiku, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hun, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::IrishFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Kaval, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Khlui, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::KnotweedFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::KoncovkaAltoFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Koudi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Ney, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Nohkan, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::NoseFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Palendag, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Quena, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Ryuteki, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Shakuhachi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ShepherdsPipe, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Shinobue, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ShivaWhistle, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Shvi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Suling, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Tarka, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TenorOvertoneFlute, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Tumpong, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Venu, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Xiao, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Xun, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Albogue, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Alboka, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AltoCrumhorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Arghul, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Bawu, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Chalumeau, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ClarinetteDAmour, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Cornamuse, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Diplica, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::DoubleClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::HeckelClarina, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::HeckelphoneClarinet, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hirtenschalmei, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Launeddas, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Maqrunah, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Mijwiz, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Octavin, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Pibgorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Rauschpfeife, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sipsi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ModernTarogato, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TenorCrumhorn, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Zhaleika, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Algaita, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Bifora, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Bombarde, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Cromorne, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Duduk, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Dulcian, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Dulzaina, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Guan, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Guanzi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Hichiriki, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hne, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::JogiBaja, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::KenBau, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Mizmar, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Nadaswaram, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::OboeDaCaccia, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Pi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Piri, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::PungiSnakeCharmer, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Rackett, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::ReedContrabass, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Rhaita, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Rothphone, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sarrusophone, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Shawm, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Shehnai, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sopila, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sorna, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sralai, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Suona, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Surnay, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Taepyeongso, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::AncientTarogato, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TrompetaChina, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Zurla, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Unspecified } },
        { uuid::Zurna, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::KhaenMouthOrgan, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Hulusi, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::Sheng, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::TrumpetBFlat, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::TrumpetC, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::TrumpetD, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Cornet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Flugelhorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Mellophone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::HornF, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Trombone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BassTrombone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Euphonium, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BaritoneBC, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BaritoneTC, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Tuba, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BassTuba, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Sousaphone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BrassSection, { InstrumentFamily::Brass, SoloOrEnsemble::Ensemble } },
        { uuid::PiccoloTrumpetA, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Bugle, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::CornetEFlat, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::HornEFlat, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::AltoTrombone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::TenorTrombone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::ContrabassTrombone, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Alphorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::AltoHorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Didgeridoo, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::PostHorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::ViennaHorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::WagnerTuba, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BaroqueTrumpet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::BassTrumpet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Cornetto, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Fiscorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Kuhlohorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::PocketTrumpet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Saxhorn, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::SlideTrumpet, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Cimbasso, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::DoubleBellEuphonium, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Sackbut, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Helicon, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Ophicleide, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Serpent, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::SubContrabassTuba, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::ConchShell, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Horagai, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Shofar, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Vuvuzela, { InstrumentFamily::Brass, SoloOrEnsemble::Solo } },
        { uuid::Harp, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TroubadorHarp, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guitar8vb, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::AcousticGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ClassicalGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ElectricGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::SteelGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Banjo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TenorBanjo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::AcousticBass, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::BassGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ElectricBass, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::FretlessBass, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::StringBass, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Mandolin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Dulcimer, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::HammeredDulcimer, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Dulcimer8vb, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Autoharp, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Lute, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Ukulele, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TenorUkulele, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Sitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Zither, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Archlute, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Baglama, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Balalaika, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Bandura, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Banjolele, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Barbat, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Begena, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Biwa, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Bolon, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Bordonua, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Bouzouki, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::BulgarianTambura, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ChapmanStick, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Charango, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ChitarraBattente, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::ChaozhouGuzheng, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Cimbalom, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Cittern, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Cuatro, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::DanBau, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::DanNguyet, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::DanTamThapLuc, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::DanTranh, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::DanTyBa, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::DiddleyBow, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Dobro, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Domra, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Dutar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Duxianqin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Ektara1, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::FlamencoGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Geomungo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Ektara2, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Gottuvadhyam, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::GuitarraQuintaHuapanguera, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guitarron, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guitjo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::GuitjoDoubleNeck, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guqin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Guzheng, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::HarpGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::IrishBouzouki, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::JaranaHuasteca, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::JaranaJarocho, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::JaranaMosquito, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::JaranaSegunda, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::JaranaTercera, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Kabosy, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Kantele, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Kayagum, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Khim, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Kobza, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Komuz, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Koto, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Kutiyapi, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Langeleik, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Lyre, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MandoBass, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MandoCello, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Mandola, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Mandora, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Mandore, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Mangbetu, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Marovany, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MohanVeena, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MoodSwinger, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::MusicalBow, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Ngoni, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::OctaveMandolin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Oud, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Pipa, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::PortugueseGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Psaltery, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::RequintoGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Ruan, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::RudraVeena, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Sallaneh, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Sanshin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Santoor, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Sanxian, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Sarod, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Saung, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Saz, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Se, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Setar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Shamisen, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Tambura, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TarPlucked, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Theorbo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Timple, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Tres, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Tsymbaly, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Valiha, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Veena, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::VichitraVeena, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::VihuelaMexico, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::VihuelaSpain, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::WashtubBass, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Whamola, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Xalam, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Yangqin, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Yazheng, { InstrumentFamily::Strings, SoloOrEnsemble::Solo } },
        { uuid::Yueqin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarNoName, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarStems, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarD, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarDADGAD, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarDoubled, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarDropD, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitarG, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabGuitar7String, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoG, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabTenorBanjo, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoC, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoD, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoDoubleC, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoGModal, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBanjoPlectrum, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBassGuitar4, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBassGuitar5, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabBassGuitar6, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabDulcimerDAA, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Unspecified } },
        { uuid::TabDulcimerDAAUnison, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Unspecified } },
        { uuid::TabDulcimerDAD, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Unspecified } },
        { uuid::TabGamba, { InstrumentFamily::Strings, SoloOrEnsemble::Unspecified } },
        { uuid::TabLuteItalian, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabLuteLetters, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Unspecified } },
        { uuid::TabMandolin, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabRequinto, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabSitarShankar, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabSitarKhan, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabUkulele, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::TabVihuela, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Timpani, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Mallets, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Unspecified } },
        { uuid::Bells, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Chimes, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Crotales, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Glockenspiel, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SopranoGlockenspiel, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::AltoGlockenspiel, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Marimba, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::BassMarimba, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::MarimbaSingleStaff, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::TubularBells, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Vibraphone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Xylophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SopranoXylophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::AltoXylophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::BassXylophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Xylorimba, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::BellLyre, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Boomwhackers, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::ChromanotesInstruments, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Carillon, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::CrystalGlasses, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::FlexatonePitched, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::GlassHarmonica, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::GlassMarimba, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Handbells, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::HandbellsTClef, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::HandbellsBClef, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::HangTClef, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::JawHarp, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kalimba, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SopranoMetallophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::AltoMetallophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::BassMetallophone, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::MusicalSaw, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SlideWhistle, { InstrumentFamily::Woodwinds, SoloOrEnsemble::Solo } },
        { uuid::SteelDrumsTClef, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SteelDrumsBClef, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BonangGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::GansaGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::GenderGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::GiyingGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::KantilGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::PelogPanerusGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::PemadeGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::PenyacahGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SaronBarungGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SaronDemongGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SaronPanerusGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SlendroPanerusGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::SlenthemGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Almglocken, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Angklung, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::ArrayMbira, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Balafon, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Balaphon, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Unspecified } },
        { uuid::Bianqing, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Bianzhong, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Fangxiang, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::GandinganAKayo, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Gyil, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Kubing, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kulintang, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::KulintangAKayo, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::KulintangATiniok, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Lamellaphone, { InstrumentFamily::PluckedStrings, SoloOrEnsemble::Solo } },
        { uuid::Likembe, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Luntang, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Mbira, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Murchang, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::RanatEklek, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::RanatThumLek, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Sanza, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::TaikoDrums, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TempleBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TibetanBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TibetanSingingBowls, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SnareDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BassDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::DrumSet, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::TenorDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::QuadToms, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::QuintToms, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::RotoToms, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TenorLine, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::SnareLine, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::BassDrums5Line, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Djembe, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BongoDrums, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::CongaDrums, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::LogDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tablas, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Timbales, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AfricanLogDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Apentemma, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ArabianFrameDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Ashiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Atabaque, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bata, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bendir, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bodhran, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bombo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bougarabou, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BuffaloDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chenda, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chudaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dabakan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Daibyosi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Damroo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Darabuka, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::DatanguLionDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dhol, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dholak, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dollu, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Dondo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Doundounba, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Duff, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dumbek, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumKagan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumKpanlogo1Large, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumKpanlogo2Medium, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumKpanlogo3Combo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumSogo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Fontomfrom, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Geduk, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::HandDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Hiradaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Igihumurizo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ingoma, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Inyahura, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Janggu, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kakko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kanjira, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KendangGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kenkeni, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Khol, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kodaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kudum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::LambegDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Madal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Maddale, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::MoroccoDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Mridangam, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Naal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::NagaDodaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Nagara, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Naqara, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::NativeLogDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::NigerianLogDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Odaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Okawa, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::OkedoDodaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PahuHula, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Pakhavaj, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Pandero, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PowwowDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PuebloDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Repinique, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Sabar, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Sakara, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Sampho, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Sangban, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ShimeDaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Surdo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TalkingDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tama, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tamborita, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tamte, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tantan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tangku, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Taphon, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TarDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tasha, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Thavil, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tombak, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Tumbak, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tsuzumi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::UchiwaDaiko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Udaku, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Zarb, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PercussionGeneral, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::PercAccessories, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::WindChimes, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChimeTree, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BellTree, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::JingleBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tambourine, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Triangle, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Cymbals, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::FingerCymbals, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::CrashCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::HiHatCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::RideCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SplashCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TamTam, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Gong, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AgogoBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AirHorn, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BrakeDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Cabasa, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Cajon, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Castanets, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Clap, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Clapper, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Claves, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Cowbell, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Cuica, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Guiro, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Maracas, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PoliceWhistle, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Rainstick, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ratchet, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Rattle, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SandBlock, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Shakers, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Spoons, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TempleBlocks, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Vibraslap, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Washboard, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Whip, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::WindMachine, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::WoodBlocks, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::CengCengGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::GongAgengGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KempulGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KempyangGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KenongGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KetukGamelan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ReyongGamelan, { InstrumentFamily::PitchedPercussion, SoloOrEnsemble::Solo } },
        { uuid::Adodo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AeolianHarp, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Afoxe, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AgogoBlock, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Agung, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::AgungAtamLang, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ahoko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Babendil, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BasicAfricanPercussion, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::BasicIndianPercussion, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::BasicMiddleEastPercussion, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::Berimbau, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bones, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::BongoBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Bullroarer, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Caxixi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChachaBells, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chabara, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chanchiki, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chimta, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChinaTempleBlocks, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChineseCymbals, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChineseGongs, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ChinesePercussionEnsemble, { InstrumentFamily::Percussion, SoloOrEnsemble::Ensemble } },
        { uuid::Ching, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Chippli, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Daff, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dafli, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Dawuro, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Def, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Doira, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumAtoke, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumAxatse, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::EweDrumGangokui, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::FlexatonePerc, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Gandingan, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ganza, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ghatam, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ghungroo, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Gome, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Guban, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::HandCymbal, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Hang, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Hatheli, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Hosho, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Hyoushigi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ibo, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::IndianGong, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Ipu, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Jawbone, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KaEkeEke, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kagul, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kalaau, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kashiklar, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kesi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Khartal, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kkwaenggwari, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Kpokopoko, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::KrinSlitDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::LavaStones, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::LuoGong, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Manjeera, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::PanClappers, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Patschen, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::RattleCog, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Riq, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Shekere, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Sistre, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Sistrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SlideWhistlePercClef, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::SlitDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Snap, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Stamp, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::StirDrum, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TebYoshi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Televi, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Teponaztli, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ThaiGong, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TibetanCymbals, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TicTocBlock, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::TimbaleBell, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tinaja, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Tingsha, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Toere, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::ToneTang, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Trychel, { InstrumentFamily::Percussion, SoloOrEnsemble::Unspecified } },
        { uuid::Udu, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },
        { uuid::Zills, { InstrumentFamily::Percussion, SoloOrEnsemble::Solo } },

    };
    return result;
}

struct StaffStyleInstrumentSpan {
    std::string instUuid;
    std::optional<MusicPoint> revertPosition;

    bool operator==(const StaffStyleInstrumentSpan& other) const
    { return instUuid == other.instUuid && revertPosition == other.revertPosition; }

    bool operator!=(const StaffStyleInstrumentSpan& other) const
    { return !(*this == other); }
};

using StaffStyleInstrumentSpans = std::map<MusicPoint, StaffStyleInstrumentSpan>;

StaffStyleInstrumentSpans collectStaffStyleInstrumentSpans(const DocumentPtr& document, Cmper partId, StaffCmper staffId)
{
    StaffStyleInstrumentSpans result;
    const auto assignments = document->getOthers()->getArray<others::StaffStyleAssign>(partId, staffId);
    for (const auto& assignment : assignments) {
        const auto style = assignment->getStaffStyle();
        if (style && style->containsInstrumentChange()) {
            const auto start = MusicPoint(assignment->startMeas, util::Fraction::fromEdu(assignment->startEdu));
            result.emplace(start, StaffStyleInstrumentSpan{ style->instUuid, assignment->nextLocation(staffId) });
        }
    }
    return result;
}

std::string formatMusicPoint(const MusicPoint& point)
{
    return "measure " + std::to_string(point.measureId)
        + ", edu " + std::to_string(point.position.calcEduDuration());
}

} // namespace

InstrumentFamily instrumentFamilyFromUuid(InstrumentUuid uuid)
{
    const auto& map = instrumentFamilyMap();
    const auto it = map.find(uuid);
    return it == map.end() ? InstrumentFamily::Unspecified : it->second.family;
}

SoloOrEnsemble instrumentSoloOrEnsembleFromUuid(InstrumentUuid uuid)
{
    const auto& map = instrumentFamilyMap();
    const auto it = map.find(uuid);
    return it == map.end() ? SoloOrEnsemble::Unspecified : it->second.soloOrEnsemble;
}

std::vector<StaffCmper> InstrumentInfo::getSequentialStaves() const
{
    std::vector<std::pair<StaffCmper, size_t>> sorted(staves.begin(), staves.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    std::vector<StaffCmper> result;
    result.reserve(sorted.size());
    for (const auto& [staffId, _] : sorted) {
        result.push_back(staffId);
    }
    return result;
}

InstrumentInfo::InstrumentChangeEvents InstrumentInfo::getChanges() const
{
    const auto sequentialStaves = getSequentialStaves();
    if (sequentialStaves.empty()) {
        return {};
    }

    const auto document = getDocument();
    const auto partId = getPartId();
    const auto topStaffId = sequentialStaves.front();
    const auto topStaffEvents = collectStaffStyleInstrumentSpans(document, partId, topStaffId);
    StaffStyleInstrumentSpans acceptedEvents = topStaffEvents;

    for (auto staffIt = std::next(sequentialStaves.begin()); staffIt != sequentialStaves.end(); ++staffIt) {
        const auto staffId = *staffIt;
        const auto staffEvents = collectStaffStyleInstrumentSpans(document, partId, staffId);
        for (auto eventIt = acceptedEvents.begin(); eventIt != acceptedEvents.end();) {
            const auto staffEventIt = staffEvents.find(eventIt->first);
            if (staffEventIt == staffEvents.end() || staffEventIt->second != eventIt->second) {
                util::Logger::log(util::Logger::LogLevel::Warning,
                    "Ignoring instrument change for top staff " + std::to_string(topStaffId)
                    + " at " + formatMusicPoint(eventIt->first)
                    + " because staff " + std::to_string(staffId) + " has no aligned instrument change.");
                eventIt = acceptedEvents.erase(eventIt);
            } else {
                ++eventIt;
            }
        }
        for (const auto& [staffChangeStart, staffRevert] : staffEvents) {
            const auto topEventIt = topStaffEvents.find(staffChangeStart);
            if (topEventIt == topStaffEvents.end() || topEventIt->second != staffRevert) {
                util::Logger::log(util::Logger::LogLevel::Warning,
                    "Ignoring instrument change for staff " + std::to_string(staffId)
                    + " at " + formatMusicPoint(staffChangeStart)
                    + " because top staff " + std::to_string(topStaffId) + " has no aligned instrument change.");
            }
        }
    }

    std::set<MusicPoint> changePositions;
    changePositions.emplace(MusicPoint{});
    for (const auto& [start, span] : acceptedEvents) {
        changePositions.emplace(start);
        if (span.revertPosition) {
            changePositions.emplace(*span.revertPosition);
        }
    }

    InstrumentChangeEvents result;
    for (const auto& position : changePositions) {
        auto topStaffComposite = others::StaffComposite::createCurrent(document, partId, topStaffId, position.measureId, position.position.calcEduDuration());
        if (!topStaffComposite) {
            util::Logger::log(util::Logger::LogLevel::Warning,
                "Unable to create top staff composite for instrument change on staff " + std::to_string(topStaffId)
                + " at " + formatMusicPoint(position) + ".");
            continue;
        }
        const auto identity = InstrumentIdentity{ topStaffComposite->instUuid };
        result.emplace(position, InstrumentChange{ identity, topStaffComposite });
    }
    return result;
}

std::vector<InstrumentInfo::InstrumentIdentity> InstrumentInfo::getInstrumentIdentities() const
{
    std::vector<InstrumentIdentity> result;
    std::set<InstrumentIdentity> seen;
    for (const auto& [_, change] : getChanges()) {
        if (seen.insert(change.identity).second) {
            result.emplace_back(change.identity);
        }
    }
    return result;
}

InstrumentInfo::InstrumentIdentity InstrumentInfo::getInstrumentIdentityAt(MusicPoint point) const
{
    const auto changes = getChanges();
    const auto it = changes.upper_bound(point);
    MUSX_ASSERT_IF(it == changes.begin()) {
        throw std::logic_error("No instrument identity found at " + formatMusicPoint(point) + ".");
    }
    return std::prev(it)->second.identity;
}

const InstrumentInfo* InstrumentMap::getInstrumentForStaff(StaffCmper staffId) const
{
    const auto& instIt = this->find(staffId);
    if (instIt != this->end()) {
        return &instIt->second;
    } else {
        for (const auto& [top, info] : *this) {
            if (info.staves.find(staffId) != info.staves.end()) {
                return &info;
            }
        }
    }
    return nullptr;
}

} // namespace dom
} // namespace musx
