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

const std::unordered_map<InstrumentUuid, InstrumentFamily>& instrumentFamilyMap()
{
    // Keep this table exhaustive with InstrumentUuids.h so new Finale UUIDs are easy to audit.
    static const std::unordered_map<InstrumentUuid, InstrumentFamily> result = {
        { uuid::BlankStaff, InstrumentFamily::Unspecified },
        { uuid::BlankStaff2, InstrumentFamily::Unspecified },
        { uuid::GrandStaff, InstrumentFamily::Keyboard },
        { uuid::Unknown, InstrumentFamily::Unspecified },
        { uuid::Violin, InstrumentFamily::Strings },
        { uuid::Viola, InstrumentFamily::Strings },
        { uuid::Cello, InstrumentFamily::Strings },
        { uuid::DoubleBass, InstrumentFamily::Strings },
        { uuid::ViolinSection, InstrumentFamily::Strings },
        { uuid::ViolaSection, InstrumentFamily::Strings },
        { uuid::CelloSection, InstrumentFamily::Strings },
        { uuid::VioloncelloSection, InstrumentFamily::Strings },
        { uuid::DoubleBassSection, InstrumentFamily::Strings },
        { uuid::ContrabassSection, InstrumentFamily::Strings },
        { uuid::StringEnsemble, InstrumentFamily::Strings },
        { uuid::ViolaDAmore, InstrumentFamily::Strings },
        { uuid::Ajaeng, InstrumentFamily::Strings },
        { uuid::Arpeggione, InstrumentFamily::Strings },
        { uuid::Baryton, InstrumentFamily::Strings },
        { uuid::ByzantineLyra, InstrumentFamily::Strings },
        { uuid::CretanLyra, InstrumentFamily::Strings },
        { uuid::Crwth, InstrumentFamily::Strings },
        { uuid::Dahu, InstrumentFamily::Strings },
        { uuid::Dangao, InstrumentFamily::Strings },
        { uuid::Dihu, InstrumentFamily::Strings },
        { uuid::Erhu, InstrumentFamily::Strings },
        { uuid::Erxian, InstrumentFamily::Strings },
        { uuid::Fiddle, InstrumentFamily::Strings },
        { uuid::Gaohu, InstrumentFamily::Strings },
        { uuid::Gehu, InstrumentFamily::Strings },
        { uuid::Haegeum, InstrumentFamily::Strings },
        { uuid::HardangerFiddle, InstrumentFamily::Strings },
        { uuid::HurdyGurdy, InstrumentFamily::Strings },
        { uuid::Igil, InstrumentFamily::Strings },
        { uuid::Kamancha, InstrumentFamily::Strings },
        { uuid::Kokyu, InstrumentFamily::Strings },
        { uuid::Kora, InstrumentFamily::PluckedStrings },
        { uuid::LaruAn, InstrumentFamily::Strings },
        { uuid::Leiqin, InstrumentFamily::Strings },
        { uuid::Lirone, InstrumentFamily::Strings },
        { uuid::MorinKhuur, InstrumentFamily::Strings },
        { uuid::Nyckelharpa, InstrumentFamily::Strings },
        { uuid::Octobass, InstrumentFamily::Strings },
        { uuid::Rebab, InstrumentFamily::Strings },
        { uuid::Rebec, InstrumentFamily::Strings },
        { uuid::Sarangi, InstrumentFamily::Strings },
        { uuid::SarangiDrone, InstrumentFamily::Strings },
        { uuid::StrohViolin, InstrumentFamily::Strings },
        { uuid::Trombamarina, InstrumentFamily::Strings },
        { uuid::Vielle, InstrumentFamily::Strings },
        { uuid::Viol, InstrumentFamily::Strings },
        { uuid::ViolaDaGamba, InstrumentFamily::Strings },
        { uuid::ViolinoPiccolo, InstrumentFamily::Strings },
        { uuid::VioloncelloPiccolo, InstrumentFamily::Strings },
        { uuid::Violotta, InstrumentFamily::Strings },
        { uuid::Zhonghu, InstrumentFamily::Strings },
        { uuid::Piano, InstrumentFamily::Keyboard },
        { uuid::PianoNoName, InstrumentFamily::Keyboard },
        { uuid::Harpsichord, InstrumentFamily::Keyboard },
        { uuid::Organ, InstrumentFamily::Organ },
        { uuid::Organ2Staff, InstrumentFamily::Organ },
        { uuid::Celesta, InstrumentFamily::Keyboard },
        { uuid::Accordion, InstrumentFamily::Keyboard },
        { uuid::Melodica, InstrumentFamily::Woodwinds },
        { uuid::ElectricPiano, InstrumentFamily::Keyboard },
        { uuid::Clavinet, InstrumentFamily::Keyboard },
        { uuid::SynthPad, InstrumentFamily::Synth },
        { uuid::SynthLead, InstrumentFamily::Synth },
        { uuid::SynthBrass, InstrumentFamily::Brass },
        { uuid::SynthSoundtrack, InstrumentFamily::Synth },
        { uuid::SoundFX, InstrumentFamily::Synth },
        { uuid::Harmonium, InstrumentFamily::Organ },
        { uuid::OndesMartenot, InstrumentFamily::Keyboard },
        { uuid::Theremin, InstrumentFamily::Synth },
        { uuid::Virginal, InstrumentFamily::Keyboard },
        { uuid::Clavichord, InstrumentFamily::Keyboard },
        { uuid::SopranoVoice, InstrumentFamily::Voice },
        { uuid::AltoVoice, InstrumentFamily::Voice },
        { uuid::TenorVoice, InstrumentFamily::Voice },
        { uuid::BaritoneVoice, InstrumentFamily::Voice },
        { uuid::BassVoice, InstrumentFamily::Voice },
        { uuid::Vocals, InstrumentFamily::Voice },
        { uuid::Voice, InstrumentFamily::Voice },
        { uuid::VoiceNoName, InstrumentFamily::Voice },
        { uuid::MezzoSopranoVoice, InstrumentFamily::Voice },
        { uuid::ContraltoVoice, InstrumentFamily::Voice },
        { uuid::CountertenorVoice, InstrumentFamily::Voice },
        { uuid::BassBaritoneVoice, InstrumentFamily::Voice },
        { uuid::ChoirAahs, InstrumentFamily::Voice },
        { uuid::ChoirOohs, InstrumentFamily::Voice },
        { uuid::Yodel, InstrumentFamily::Voice },
        { uuid::Beatbox, InstrumentFamily::Voice },
        { uuid::Kazoo, InstrumentFamily::Voice },
        { uuid::Talkbox, InstrumentFamily::Voice },
        { uuid::VocalPercussion, InstrumentFamily::Voice },
        { uuid::Piccolo, InstrumentFamily::Woodwinds },
        { uuid::Flute, InstrumentFamily::Woodwinds },
        { uuid::AltoFlute, InstrumentFamily::Woodwinds },
        { uuid::Oboe, InstrumentFamily::Woodwinds },
        { uuid::OboeDAmore, InstrumentFamily::Woodwinds },
        { uuid::EnglishHorn, InstrumentFamily::Woodwinds },
        { uuid::ClarinetBFlat, InstrumentFamily::Woodwinds },
        { uuid::ClarinetA, InstrumentFamily::Woodwinds },
        { uuid::ClarinetEFlat, InstrumentFamily::Woodwinds },
        { uuid::AltoClarinet, InstrumentFamily::Woodwinds },
        { uuid::ContraltoClarinet, InstrumentFamily::Woodwinds },
        { uuid::BassClarinet, InstrumentFamily::Woodwinds },
        { uuid::ContrabassClarinet, InstrumentFamily::Woodwinds },
        { uuid::Bassoon, InstrumentFamily::Woodwinds },
        { uuid::Contrabassoon, InstrumentFamily::Woodwinds },
        { uuid::WindSection, InstrumentFamily::Woodwinds },
        { uuid::SopranoSax, InstrumentFamily::Woodwinds },
        { uuid::AltoSax, InstrumentFamily::Woodwinds },
        { uuid::TenorSax, InstrumentFamily::Woodwinds },
        { uuid::BaritoneSax, InstrumentFamily::Woodwinds },
        { uuid::SopranoRecorder, InstrumentFamily::Woodwinds },
        { uuid::SopraninoRecorder, InstrumentFamily::Woodwinds },
        { uuid::AltoRecorder, InstrumentFamily::Woodwinds },
        { uuid::TenorRecorder, InstrumentFamily::Woodwinds },
        { uuid::BassRecorder, InstrumentFamily::Woodwinds },
        { uuid::DescantRecorder, InstrumentFamily::Woodwinds },
        { uuid::Ocarina, InstrumentFamily::Woodwinds },
        { uuid::PennyWhistle, InstrumentFamily::Woodwinds },
        { uuid::PennyWhistleD, InstrumentFamily::Woodwinds },
        { uuid::PennyWhistleG, InstrumentFamily::Woodwinds },
        { uuid::LowIrishWhistle, InstrumentFamily::Woodwinds },
        { uuid::TinWhistleBFlat, InstrumentFamily::Woodwinds },
        { uuid::Harmonica, InstrumentFamily::Woodwinds },
        { uuid::BassHarmonica, InstrumentFamily::Woodwinds },
        { uuid::Concertina, InstrumentFamily::Keyboard },
        { uuid::Bandoneon, InstrumentFamily::Keyboard },
        { uuid::HornF_WWQuintet, InstrumentFamily::Brass },
        { uuid::Bagpipes, InstrumentFamily::Woodwinds },
        { uuid::UilleannPipes, InstrumentFamily::Woodwinds },
        { uuid::GaidaPipes, InstrumentFamily::Woodwinds },
        { uuid::ContraAltoFlute, InstrumentFamily::Woodwinds },
        { uuid::BassFlute, InstrumentFamily::Woodwinds },
        { uuid::ContrabassFlute, InstrumentFamily::Woodwinds },
        { uuid::DoubleContrabassFlute, InstrumentFamily::Woodwinds },
        { uuid::HyperbassFlute, InstrumentFamily::Woodwinds },
        { uuid::PanPipes, InstrumentFamily::Woodwinds },
        { uuid::Fife, InstrumentFamily::Woodwinds },
        { uuid::BottleBlow, InstrumentFamily::Woodwinds },
        { uuid::Jug, InstrumentFamily::Woodwinds },
        { uuid::PiccoloOboe, InstrumentFamily::Woodwinds },
        { uuid::PiccoloHeckelphone, InstrumentFamily::Woodwinds },
        { uuid::Heckelphone, InstrumentFamily::Woodwinds },
        { uuid::BassOboe, InstrumentFamily::Woodwinds },
        { uuid::BassetClarinet, InstrumentFamily::Woodwinds },
        { uuid::BassetHorn, InstrumentFamily::Woodwinds },
        { uuid::Hornpipe, InstrumentFamily::Woodwinds },
        { uuid::PiccoloClarinet, InstrumentFamily::Woodwinds },
        { uuid::Saxonette, InstrumentFamily::Woodwinds },
        { uuid::SopraninoSax, InstrumentFamily::Woodwinds },
        { uuid::MezzoSopranoSax, InstrumentFamily::Woodwinds },
        { uuid::Sopranino, InstrumentFamily::Woodwinds },
        { uuid::CMelodySax, InstrumentFamily::Woodwinds },
        { uuid::Aulochrome, InstrumentFamily::Woodwinds },
        { uuid::Xaphoon, InstrumentFamily::Woodwinds },
        { uuid::BassSax, InstrumentFamily::Woodwinds },
        { uuid::ContrabassSax, InstrumentFamily::Woodwinds },
        { uuid::SubContrabassSax, InstrumentFamily::Woodwinds },
        { uuid::Tubax, InstrumentFamily::Woodwinds },
        { uuid::Bansuri, InstrumentFamily::Woodwinds },
        { uuid::Danso, InstrumentFamily::Woodwinds },
        { uuid::Dizi, InstrumentFamily::Woodwinds },
        { uuid::DilliKaval, InstrumentFamily::Woodwinds },
        { uuid::Diple, InstrumentFamily::Woodwinds },
        { uuid::DoubleFlute, InstrumentFamily::Woodwinds },
        { uuid::Dvojnice, InstrumentFamily::Woodwinds },
        { uuid::DvojniceDrone, InstrumentFamily::Woodwinds },
        { uuid::Flageolet, InstrumentFamily::Woodwinds },
        { uuid::Fujara, InstrumentFamily::Woodwinds },
        { uuid::Gemshorn, InstrumentFamily::Woodwinds },
        { uuid::Hocchiku, InstrumentFamily::Woodwinds },
        { uuid::Hun, InstrumentFamily::Woodwinds },
        { uuid::IrishFlute, InstrumentFamily::Woodwinds },
        { uuid::Kaval, InstrumentFamily::Woodwinds },
        { uuid::Khlui, InstrumentFamily::Woodwinds },
        { uuid::KnotweedFlute, InstrumentFamily::Woodwinds },
        { uuid::KoncovkaAltoFlute, InstrumentFamily::Woodwinds },
        { uuid::Koudi, InstrumentFamily::Woodwinds },
        { uuid::Ney, InstrumentFamily::Woodwinds },
        { uuid::Nohkan, InstrumentFamily::Woodwinds },
        { uuid::NoseFlute, InstrumentFamily::Woodwinds },
        { uuid::Palendag, InstrumentFamily::Woodwinds },
        { uuid::Quena, InstrumentFamily::Woodwinds },
        { uuid::Ryuteki, InstrumentFamily::Woodwinds },
        { uuid::Shakuhachi, InstrumentFamily::Woodwinds },
        { uuid::ShepherdsPipe, InstrumentFamily::Woodwinds },
        { uuid::Shinobue, InstrumentFamily::Woodwinds },
        { uuid::ShivaWhistle, InstrumentFamily::Woodwinds },
        { uuid::Shvi, InstrumentFamily::Woodwinds },
        { uuid::Suling, InstrumentFamily::Woodwinds },
        { uuid::Tarka, InstrumentFamily::Woodwinds },
        { uuid::TenorOvertoneFlute, InstrumentFamily::Woodwinds },
        { uuid::Tumpong, InstrumentFamily::Woodwinds },
        { uuid::Venu, InstrumentFamily::Woodwinds },
        { uuid::Xiao, InstrumentFamily::Woodwinds },
        { uuid::Xun, InstrumentFamily::Woodwinds },
        { uuid::Albogue, InstrumentFamily::Woodwinds },
        { uuid::Alboka, InstrumentFamily::Woodwinds },
        { uuid::AltoCrumhorn, InstrumentFamily::Woodwinds },
        { uuid::Arghul, InstrumentFamily::Woodwinds },
        { uuid::Bawu, InstrumentFamily::Woodwinds },
        { uuid::Chalumeau, InstrumentFamily::Woodwinds },
        { uuid::ClarinetteDAmour, InstrumentFamily::Woodwinds },
        { uuid::Cornamuse, InstrumentFamily::Woodwinds },
        { uuid::Diplica, InstrumentFamily::Woodwinds },
        { uuid::DoubleClarinet, InstrumentFamily::Woodwinds },
        { uuid::HeckelClarina, InstrumentFamily::Woodwinds },
        { uuid::HeckelphoneClarinet, InstrumentFamily::Woodwinds },
        { uuid::Hirtenschalmei, InstrumentFamily::Woodwinds },
        { uuid::Launeddas, InstrumentFamily::Woodwinds },
        { uuid::Maqrunah, InstrumentFamily::Woodwinds },
        { uuid::Mijwiz, InstrumentFamily::Woodwinds },
        { uuid::Octavin, InstrumentFamily::Woodwinds },
        { uuid::Pibgorn, InstrumentFamily::Woodwinds },
        { uuid::Rauschpfeife, InstrumentFamily::Woodwinds },
        { uuid::Sipsi, InstrumentFamily::Woodwinds },
        { uuid::ModernTarogato, InstrumentFamily::Woodwinds },
        { uuid::TenorCrumhorn, InstrumentFamily::Woodwinds },
        { uuid::Zhaleika, InstrumentFamily::Woodwinds },
        { uuid::Algaita, InstrumentFamily::Woodwinds },
        { uuid::Bifora, InstrumentFamily::Woodwinds },
        { uuid::Bombarde, InstrumentFamily::Woodwinds },
        { uuid::Cromorne, InstrumentFamily::Woodwinds },
        { uuid::Duduk, InstrumentFamily::Woodwinds },
        { uuid::Dulcian, InstrumentFamily::Woodwinds },
        { uuid::Dulzaina, InstrumentFamily::Woodwinds },
        { uuid::Guan, InstrumentFamily::Woodwinds },
        { uuid::Guanzi, InstrumentFamily::Woodwinds },
        { uuid::Hichiriki, InstrumentFamily::Woodwinds },
        { uuid::Hne, InstrumentFamily::Woodwinds },
        { uuid::JogiBaja, InstrumentFamily::Woodwinds },
        { uuid::KenBau, InstrumentFamily::Woodwinds },
        { uuid::Mizmar, InstrumentFamily::Woodwinds },
        { uuid::Nadaswaram, InstrumentFamily::Woodwinds },
        { uuid::OboeDaCaccia, InstrumentFamily::Woodwinds },
        { uuid::Pi, InstrumentFamily::Woodwinds },
        { uuid::Piri, InstrumentFamily::Woodwinds },
        { uuid::PungiSnakeCharmer, InstrumentFamily::Woodwinds },
        { uuid::Rackett, InstrumentFamily::Woodwinds },
        { uuid::ReedContrabass, InstrumentFamily::Woodwinds },
        { uuid::Rhaita, InstrumentFamily::Woodwinds },
        { uuid::Rothphone, InstrumentFamily::Woodwinds },
        { uuid::Sarrusophone, InstrumentFamily::Woodwinds },
        { uuid::Shawm, InstrumentFamily::Woodwinds },
        { uuid::Shehnai, InstrumentFamily::Woodwinds },
        { uuid::Sopila, InstrumentFamily::Woodwinds },
        { uuid::Sorna, InstrumentFamily::Woodwinds },
        { uuid::Sralai, InstrumentFamily::Woodwinds },
        { uuid::Suona, InstrumentFamily::Woodwinds },
        { uuid::Surnay, InstrumentFamily::Woodwinds },
        { uuid::Taepyeongso, InstrumentFamily::Woodwinds },
        { uuid::AncientTarogato, InstrumentFamily::Woodwinds },
        { uuid::TrompetaChina, InstrumentFamily::Woodwinds },
        { uuid::Zurla, InstrumentFamily::Woodwinds },
        { uuid::Zurna, InstrumentFamily::Woodwinds },
        { uuid::KhaenMouthOrgan, InstrumentFamily::Woodwinds },
        { uuid::Hulusi, InstrumentFamily::Woodwinds },
        { uuid::Sheng, InstrumentFamily::Woodwinds },
        { uuid::TrumpetBFlat, InstrumentFamily::Brass },
        { uuid::TrumpetC, InstrumentFamily::Brass },
        { uuid::TrumpetD, InstrumentFamily::Brass },
        { uuid::Cornet, InstrumentFamily::Brass },
        { uuid::Flugelhorn, InstrumentFamily::Brass },
        { uuid::Mellophone, InstrumentFamily::Brass },
        { uuid::HornF, InstrumentFamily::Brass },
        { uuid::Trombone, InstrumentFamily::Brass },
        { uuid::BassTrombone, InstrumentFamily::Brass },
        { uuid::Euphonium, InstrumentFamily::Brass },
        { uuid::BaritoneBC, InstrumentFamily::Brass },
        { uuid::BaritoneTC, InstrumentFamily::Brass },
        { uuid::Tuba, InstrumentFamily::Brass },
        { uuid::BassTuba, InstrumentFamily::Brass },
        { uuid::Sousaphone, InstrumentFamily::Brass },
        { uuid::BrassSection, InstrumentFamily::Brass },
        { uuid::PiccoloTrumpetA, InstrumentFamily::Brass },
        { uuid::Bugle, InstrumentFamily::Brass },
        { uuid::CornetEFlat, InstrumentFamily::Brass },
        { uuid::HornEFlat, InstrumentFamily::Brass },
        { uuid::AltoTrombone, InstrumentFamily::Brass },
        { uuid::TenorTrombone, InstrumentFamily::Brass },
        { uuid::ContrabassTrombone, InstrumentFamily::Brass },
        { uuid::Alphorn, InstrumentFamily::Brass },
        { uuid::AltoHorn, InstrumentFamily::Brass },
        { uuid::Didgeridoo, InstrumentFamily::Brass },
        { uuid::PostHorn, InstrumentFamily::Brass },
        { uuid::ViennaHorn, InstrumentFamily::Brass },
        { uuid::WagnerTuba, InstrumentFamily::Brass },
        { uuid::BaroqueTrumpet, InstrumentFamily::Brass },
        { uuid::BassTrumpet, InstrumentFamily::Brass },
        { uuid::Cornetto, InstrumentFamily::Brass },
        { uuid::Fiscorn, InstrumentFamily::Brass },
        { uuid::Kuhlohorn, InstrumentFamily::Brass },
        { uuid::PocketTrumpet, InstrumentFamily::Brass },
        { uuid::Saxhorn, InstrumentFamily::Brass },
        { uuid::SlideTrumpet, InstrumentFamily::Brass },
        { uuid::Cimbasso, InstrumentFamily::Brass },
        { uuid::DoubleBellEuphonium, InstrumentFamily::Brass },
        { uuid::Sackbut, InstrumentFamily::Brass },
        { uuid::Helicon, InstrumentFamily::Brass },
        { uuid::Ophicleide, InstrumentFamily::Brass },
        { uuid::Serpent, InstrumentFamily::Brass },
        { uuid::SubContrabassTuba, InstrumentFamily::Brass },
        { uuid::ConchShell, InstrumentFamily::Brass },
        { uuid::Horagai, InstrumentFamily::Brass },
        { uuid::Shofar, InstrumentFamily::Brass },
        { uuid::Vuvuzela, InstrumentFamily::Brass },
        { uuid::Harp, InstrumentFamily::PluckedStrings },
        { uuid::TroubadorHarp, InstrumentFamily::PluckedStrings },
        { uuid::Guitar, InstrumentFamily::PluckedStrings },
        { uuid::Guitar8vb, InstrumentFamily::PluckedStrings },
        { uuid::AcousticGuitar, InstrumentFamily::PluckedStrings },
        { uuid::ClassicalGuitar, InstrumentFamily::PluckedStrings },
        { uuid::ElectricGuitar, InstrumentFamily::PluckedStrings },
        { uuid::SteelGuitar, InstrumentFamily::PluckedStrings },
        { uuid::Banjo, InstrumentFamily::PluckedStrings },
        { uuid::TenorBanjo, InstrumentFamily::PluckedStrings },
        { uuid::AcousticBass, InstrumentFamily::PluckedStrings },
        { uuid::BassGuitar, InstrumentFamily::PluckedStrings },
        { uuid::ElectricBass, InstrumentFamily::PluckedStrings },
        { uuid::FretlessBass, InstrumentFamily::PluckedStrings },
        { uuid::StringBass, InstrumentFamily::Strings },
        { uuid::Mandolin, InstrumentFamily::PluckedStrings },
        { uuid::Dulcimer, InstrumentFamily::PluckedStrings },
        { uuid::HammeredDulcimer, InstrumentFamily::PitchedPercussion },
        { uuid::Dulcimer8vb, InstrumentFamily::PluckedStrings },
        { uuid::Autoharp, InstrumentFamily::PluckedStrings },
        { uuid::Lute, InstrumentFamily::PluckedStrings },
        { uuid::Ukulele, InstrumentFamily::PluckedStrings },
        { uuid::TenorUkulele, InstrumentFamily::PluckedStrings },
        { uuid::Sitar, InstrumentFamily::PluckedStrings },
        { uuid::Zither, InstrumentFamily::PluckedStrings },
        { uuid::Archlute, InstrumentFamily::PluckedStrings },
        { uuid::Baglama, InstrumentFamily::PluckedStrings },
        { uuid::Balalaika, InstrumentFamily::PluckedStrings },
        { uuid::Bandura, InstrumentFamily::PluckedStrings },
        { uuid::Banjolele, InstrumentFamily::PluckedStrings },
        { uuid::Barbat, InstrumentFamily::PluckedStrings },
        { uuid::Begena, InstrumentFamily::PluckedStrings },
        { uuid::Biwa, InstrumentFamily::PluckedStrings },
        { uuid::Bolon, InstrumentFamily::PluckedStrings },
        { uuid::Bordonua, InstrumentFamily::PluckedStrings },
        { uuid::Bouzouki, InstrumentFamily::PluckedStrings },
        { uuid::BulgarianTambura, InstrumentFamily::PluckedStrings },
        { uuid::ChapmanStick, InstrumentFamily::PluckedStrings },
        { uuid::Charango, InstrumentFamily::PluckedStrings },
        { uuid::ChitarraBattente, InstrumentFamily::PluckedStrings },
        { uuid::ChaozhouGuzheng, InstrumentFamily::PluckedStrings },
        { uuid::Cimbalom, InstrumentFamily::PitchedPercussion },
        { uuid::Cittern, InstrumentFamily::PluckedStrings },
        { uuid::Cuatro, InstrumentFamily::PluckedStrings },
        { uuid::DanBau, InstrumentFamily::PluckedStrings },
        { uuid::DanNguyet, InstrumentFamily::PluckedStrings },
        { uuid::DanTamThapLuc, InstrumentFamily::PitchedPercussion },
        { uuid::DanTranh, InstrumentFamily::PluckedStrings },
        { uuid::DanTyBa, InstrumentFamily::PluckedStrings },
        { uuid::DiddleyBow, InstrumentFamily::PluckedStrings },
        { uuid::Dobro, InstrumentFamily::PluckedStrings },
        { uuid::Domra, InstrumentFamily::PluckedStrings },
        { uuid::Dutar, InstrumentFamily::PluckedStrings },
        { uuid::Duxianqin, InstrumentFamily::PluckedStrings },
        { uuid::Ektara1, InstrumentFamily::PluckedStrings },
        { uuid::FlamencoGuitar, InstrumentFamily::PluckedStrings },
        { uuid::Geomungo, InstrumentFamily::PluckedStrings },
        { uuid::Ektara2, InstrumentFamily::PluckedStrings },
        { uuid::Gottuvadhyam, InstrumentFamily::PluckedStrings },
        { uuid::GuitarraQuintaHuapanguera, InstrumentFamily::PluckedStrings },
        { uuid::Guitarron, InstrumentFamily::PluckedStrings },
        { uuid::Guitjo, InstrumentFamily::PluckedStrings },
        { uuid::GuitjoDoubleNeck, InstrumentFamily::PluckedStrings },
        { uuid::Guqin, InstrumentFamily::PluckedStrings },
        { uuid::Guzheng, InstrumentFamily::PluckedStrings },
        { uuid::HarpGuitar, InstrumentFamily::PluckedStrings },
        { uuid::IrishBouzouki, InstrumentFamily::PluckedStrings },
        { uuid::JaranaHuasteca, InstrumentFamily::PluckedStrings },
        { uuid::JaranaJarocho, InstrumentFamily::PluckedStrings },
        { uuid::JaranaMosquito, InstrumentFamily::PluckedStrings },
        { uuid::JaranaSegunda, InstrumentFamily::PluckedStrings },
        { uuid::JaranaTercera, InstrumentFamily::PluckedStrings },
        { uuid::Kabosy, InstrumentFamily::PluckedStrings },
        { uuid::Kantele, InstrumentFamily::PluckedStrings },
        { uuid::Kayagum, InstrumentFamily::PluckedStrings },
        { uuid::Khim, InstrumentFamily::PitchedPercussion },
        { uuid::Kobza, InstrumentFamily::PluckedStrings },
        { uuid::Komuz, InstrumentFamily::PluckedStrings },
        { uuid::Koto, InstrumentFamily::PluckedStrings },
        { uuid::Kutiyapi, InstrumentFamily::PluckedStrings },
        { uuid::Langeleik, InstrumentFamily::PluckedStrings },
        { uuid::Lyre, InstrumentFamily::PluckedStrings },
        { uuid::MandoBass, InstrumentFamily::PluckedStrings },
        { uuid::MandoCello, InstrumentFamily::PluckedStrings },
        { uuid::Mandola, InstrumentFamily::PluckedStrings },
        { uuid::Mandora, InstrumentFamily::PluckedStrings },
        { uuid::Mandore, InstrumentFamily::PluckedStrings },
        { uuid::Mangbetu, InstrumentFamily::PluckedStrings },
        { uuid::Marovany, InstrumentFamily::PluckedStrings },
        { uuid::MohanVeena, InstrumentFamily::PluckedStrings },
        { uuid::MoodSwinger, InstrumentFamily::PluckedStrings },
        { uuid::MusicalBow, InstrumentFamily::PluckedStrings },
        { uuid::Ngoni, InstrumentFamily::PluckedStrings },
        { uuid::OctaveMandolin, InstrumentFamily::PluckedStrings },
        { uuid::Oud, InstrumentFamily::PluckedStrings },
        { uuid::Pipa, InstrumentFamily::PluckedStrings },
        { uuid::PortugueseGuitar, InstrumentFamily::PluckedStrings },
        { uuid::Psaltery, InstrumentFamily::PluckedStrings },
        { uuid::RequintoGuitar, InstrumentFamily::PluckedStrings },
        { uuid::Ruan, InstrumentFamily::PluckedStrings },
        { uuid::RudraVeena, InstrumentFamily::PluckedStrings },
        { uuid::Sallaneh, InstrumentFamily::PluckedStrings },
        { uuid::Sanshin, InstrumentFamily::PluckedStrings },
        { uuid::Santoor, InstrumentFamily::PluckedStrings },
        { uuid::Sanxian, InstrumentFamily::PluckedStrings },
        { uuid::Sarod, InstrumentFamily::PluckedStrings },
        { uuid::Saung, InstrumentFamily::PluckedStrings },
        { uuid::Saz, InstrumentFamily::PluckedStrings },
        { uuid::Se, InstrumentFamily::PluckedStrings },
        { uuid::Setar, InstrumentFamily::PluckedStrings },
        { uuid::Shamisen, InstrumentFamily::PluckedStrings },
        { uuid::Tambura, InstrumentFamily::PluckedStrings },
        { uuid::TarPlucked, InstrumentFamily::PluckedStrings },
        { uuid::Theorbo, InstrumentFamily::PluckedStrings },
        { uuid::Timple, InstrumentFamily::PluckedStrings },
        { uuid::Tres, InstrumentFamily::PluckedStrings },
        { uuid::Tsymbaly, InstrumentFamily::PitchedPercussion },
        { uuid::Valiha, InstrumentFamily::PluckedStrings },
        { uuid::Veena, InstrumentFamily::PluckedStrings },
        { uuid::VichitraVeena, InstrumentFamily::PluckedStrings },
        { uuid::VihuelaMexico, InstrumentFamily::PluckedStrings },
        { uuid::VihuelaSpain, InstrumentFamily::PluckedStrings },
        { uuid::WashtubBass, InstrumentFamily::PluckedStrings },
        { uuid::Whamola, InstrumentFamily::PluckedStrings },
        { uuid::Xalam, InstrumentFamily::PluckedStrings },
        { uuid::Yangqin, InstrumentFamily::PitchedPercussion },
        { uuid::Yazheng, InstrumentFamily::Strings },
        { uuid::Yueqin, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitar, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarNoName, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarStems, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarD, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarDADGAD, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarDoubled, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarDropD, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitarG, InstrumentFamily::PluckedStrings },
        { uuid::TabGuitar7String, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoG, InstrumentFamily::PluckedStrings },
        { uuid::TabTenorBanjo, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoC, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoD, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoDoubleC, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoGModal, InstrumentFamily::PluckedStrings },
        { uuid::TabBanjoPlectrum, InstrumentFamily::PluckedStrings },
        { uuid::TabBassGuitar4, InstrumentFamily::PluckedStrings },
        { uuid::TabBassGuitar5, InstrumentFamily::PluckedStrings },
        { uuid::TabBassGuitar6, InstrumentFamily::PluckedStrings },
        { uuid::TabDulcimerDAA, InstrumentFamily::PluckedStrings },
        { uuid::TabDulcimerDAAUnison, InstrumentFamily::PluckedStrings },
        { uuid::TabDulcimerDAD, InstrumentFamily::PluckedStrings },
        { uuid::TabGamba, InstrumentFamily::Strings },
        { uuid::TabLuteItalian, InstrumentFamily::PluckedStrings },
        { uuid::TabLuteLetters, InstrumentFamily::PluckedStrings },
        { uuid::TabMandolin, InstrumentFamily::PluckedStrings },
        { uuid::TabRequinto, InstrumentFamily::PluckedStrings },
        { uuid::TabSitarShankar, InstrumentFamily::PluckedStrings },
        { uuid::TabSitarKhan, InstrumentFamily::PluckedStrings },
        { uuid::TabUkulele, InstrumentFamily::PluckedStrings },
        { uuid::TabVihuela, InstrumentFamily::PluckedStrings },
        { uuid::Timpani, InstrumentFamily::Percussion },
        { uuid::Mallets, InstrumentFamily::PitchedPercussion },
        { uuid::Bells, InstrumentFamily::PitchedPercussion },
        { uuid::Chimes, InstrumentFamily::Percussion },
        { uuid::Crotales, InstrumentFamily::Percussion },
        { uuid::Glockenspiel, InstrumentFamily::PitchedPercussion },
        { uuid::SopranoGlockenspiel, InstrumentFamily::PitchedPercussion },
        { uuid::AltoGlockenspiel, InstrumentFamily::PitchedPercussion },
        { uuid::Marimba, InstrumentFamily::PitchedPercussion },
        { uuid::BassMarimba, InstrumentFamily::PitchedPercussion },
        { uuid::MarimbaSingleStaff, InstrumentFamily::PitchedPercussion },
        { uuid::TubularBells, InstrumentFamily::PitchedPercussion },
        { uuid::Vibraphone, InstrumentFamily::PitchedPercussion },
        { uuid::Xylophone, InstrumentFamily::PitchedPercussion },
        { uuid::SopranoXylophone, InstrumentFamily::PitchedPercussion },
        { uuid::AltoXylophone, InstrumentFamily::PitchedPercussion },
        { uuid::BassXylophone, InstrumentFamily::PitchedPercussion },
        { uuid::Xylorimba, InstrumentFamily::PitchedPercussion },
        { uuid::BellLyre, InstrumentFamily::PitchedPercussion },
        { uuid::Boomwhackers, InstrumentFamily::PitchedPercussion },
        { uuid::ChromanotesInstruments, InstrumentFamily::PitchedPercussion },
        { uuid::Carillon, InstrumentFamily::Percussion },
        { uuid::CrystalGlasses, InstrumentFamily::PitchedPercussion },
        { uuid::FlexatonePitched, InstrumentFamily::Percussion },
        { uuid::GlassHarmonica, InstrumentFamily::PitchedPercussion },
        { uuid::GlassMarimba, InstrumentFamily::PitchedPercussion },
        { uuid::Handbells, InstrumentFamily::PitchedPercussion },
        { uuid::HandbellsTClef, InstrumentFamily::PitchedPercussion },
        { uuid::HandbellsBClef, InstrumentFamily::PitchedPercussion },
        { uuid::HangTClef, InstrumentFamily::Percussion },
        { uuid::JawHarp, InstrumentFamily::Percussion },
        { uuid::Kalimba, InstrumentFamily::PitchedPercussion },
        { uuid::SopranoMetallophone, InstrumentFamily::PitchedPercussion },
        { uuid::AltoMetallophone, InstrumentFamily::PitchedPercussion },
        { uuid::BassMetallophone, InstrumentFamily::PitchedPercussion },
        { uuid::MusicalSaw, InstrumentFamily::Percussion },
        { uuid::SlideWhistle, InstrumentFamily::Woodwinds },
        { uuid::SteelDrumsTClef, InstrumentFamily::Percussion },
        { uuid::SteelDrumsBClef, InstrumentFamily::Percussion },
        { uuid::BonangGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::GansaGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::GenderGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::GiyingGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::KantilGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::PelogPanerusGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::PemadeGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::PenyacahGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::SaronBarungGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::SaronDemongGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::SaronPanerusGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::SlendroPanerusGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::SlenthemGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::Almglocken, InstrumentFamily::Percussion },
        { uuid::Angklung, InstrumentFamily::PitchedPercussion },
        { uuid::ArrayMbira, InstrumentFamily::PitchedPercussion },
        { uuid::Balafon, InstrumentFamily::PitchedPercussion },
        { uuid::Balaphon, InstrumentFamily::PitchedPercussion },
        { uuid::Bianqing, InstrumentFamily::PitchedPercussion },
        { uuid::Bianzhong, InstrumentFamily::PitchedPercussion },
        { uuid::Fangxiang, InstrumentFamily::PitchedPercussion },
        { uuid::GandinganAKayo, InstrumentFamily::PitchedPercussion },
        { uuid::Gyil, InstrumentFamily::PitchedPercussion },
        { uuid::Kubing, InstrumentFamily::Percussion },
        { uuid::Kulintang, InstrumentFamily::PitchedPercussion },
        { uuid::KulintangAKayo, InstrumentFamily::PitchedPercussion },
        { uuid::KulintangATiniok, InstrumentFamily::PitchedPercussion },
        { uuid::Lamellaphone, InstrumentFamily::PluckedStrings },
        { uuid::Likembe, InstrumentFamily::PitchedPercussion },
        { uuid::Luntang, InstrumentFamily::PitchedPercussion },
        { uuid::Mbira, InstrumentFamily::PitchedPercussion },
        { uuid::Murchang, InstrumentFamily::Percussion },
        { uuid::RanatEklek, InstrumentFamily::PitchedPercussion },
        { uuid::RanatThumLek, InstrumentFamily::PitchedPercussion },
        { uuid::Sanza, InstrumentFamily::PitchedPercussion },
        { uuid::TaikoDrums, InstrumentFamily::Percussion },
        { uuid::TempleBells, InstrumentFamily::Percussion },
        { uuid::TibetanBells, InstrumentFamily::Percussion },
        { uuid::TibetanSingingBowls, InstrumentFamily::Percussion },
        { uuid::SnareDrum, InstrumentFamily::Percussion },
        { uuid::BassDrum, InstrumentFamily::Percussion },
        { uuid::DrumSet, InstrumentFamily::Percussion },
        { uuid::TenorDrum, InstrumentFamily::Percussion },
        { uuid::QuadToms, InstrumentFamily::Percussion },
        { uuid::QuintToms, InstrumentFamily::Percussion },
        { uuid::RotoToms, InstrumentFamily::Percussion },
        { uuid::TenorLine, InstrumentFamily::Percussion },
        { uuid::SnareLine, InstrumentFamily::Percussion },
        { uuid::BassDrums5Line, InstrumentFamily::Percussion },
        { uuid::Djembe, InstrumentFamily::Percussion },
        { uuid::BongoDrums, InstrumentFamily::Percussion },
        { uuid::CongaDrums, InstrumentFamily::Percussion },
        { uuid::LogDrum, InstrumentFamily::Percussion },
        { uuid::Tablas, InstrumentFamily::Percussion },
        { uuid::Timbales, InstrumentFamily::Percussion },
        { uuid::AfricanLogDrum, InstrumentFamily::Percussion },
        { uuid::Apentemma, InstrumentFamily::Percussion },
        { uuid::ArabianFrameDrum, InstrumentFamily::Percussion },
        { uuid::Ashiko, InstrumentFamily::Percussion },
        { uuid::Atabaque, InstrumentFamily::Percussion },
        { uuid::Bata, InstrumentFamily::Percussion },
        { uuid::Bendir, InstrumentFamily::Percussion },
        { uuid::Bodhran, InstrumentFamily::Percussion },
        { uuid::Bombo, InstrumentFamily::Percussion },
        { uuid::Bougarabou, InstrumentFamily::Percussion },
        { uuid::BuffaloDrum, InstrumentFamily::Percussion },
        { uuid::Chenda, InstrumentFamily::Percussion },
        { uuid::Chudaiko, InstrumentFamily::Percussion },
        { uuid::Dabakan, InstrumentFamily::Percussion },
        { uuid::Daibyosi, InstrumentFamily::Percussion },
        { uuid::Damroo, InstrumentFamily::Percussion },
        { uuid::Darabuka, InstrumentFamily::Percussion },
        { uuid::DatanguLionDrum, InstrumentFamily::Percussion },
        { uuid::Dhol, InstrumentFamily::Percussion },
        { uuid::Dholak, InstrumentFamily::Percussion },
        { uuid::Dollu, InstrumentFamily::Percussion },
        { uuid::Dondo, InstrumentFamily::Percussion },
        { uuid::Doundounba, InstrumentFamily::Percussion },
        { uuid::Duff, InstrumentFamily::Percussion },
        { uuid::Dumbek, InstrumentFamily::Percussion },
        { uuid::EweDrumKagan, InstrumentFamily::Percussion },
        { uuid::EweDrumKpanlogo1Large, InstrumentFamily::Percussion },
        { uuid::EweDrumKpanlogo2Medium, InstrumentFamily::Percussion },
        { uuid::EweDrumKpanlogo3Combo, InstrumentFamily::Percussion },
        { uuid::EweDrumSogo, InstrumentFamily::Percussion },
        { uuid::Fontomfrom, InstrumentFamily::Percussion },
        { uuid::Geduk, InstrumentFamily::Percussion },
        { uuid::HandDrum, InstrumentFamily::Percussion },
        { uuid::Hiradaiko, InstrumentFamily::Percussion },
        { uuid::Igihumurizo, InstrumentFamily::Percussion },
        { uuid::Ingoma, InstrumentFamily::Percussion },
        { uuid::Inyahura, InstrumentFamily::Percussion },
        { uuid::Janggu, InstrumentFamily::Percussion },
        { uuid::Kakko, InstrumentFamily::Percussion },
        { uuid::Kanjira, InstrumentFamily::Percussion },
        { uuid::KendangGamelan, InstrumentFamily::Percussion },
        { uuid::Kenkeni, InstrumentFamily::Percussion },
        { uuid::Khol, InstrumentFamily::Percussion },
        { uuid::Kodaiko, InstrumentFamily::Percussion },
        { uuid::Kudum, InstrumentFamily::Percussion },
        { uuid::LambegDrum, InstrumentFamily::Percussion },
        { uuid::Madal, InstrumentFamily::Percussion },
        { uuid::Maddale, InstrumentFamily::Percussion },
        { uuid::MoroccoDrum, InstrumentFamily::Percussion },
        { uuid::Mridangam, InstrumentFamily::Percussion },
        { uuid::Naal, InstrumentFamily::Percussion },
        { uuid::NagaDodaiko, InstrumentFamily::Percussion },
        { uuid::Nagara, InstrumentFamily::Percussion },
        { uuid::Naqara, InstrumentFamily::Percussion },
        { uuid::NativeLogDrum, InstrumentFamily::Percussion },
        { uuid::NigerianLogDrum, InstrumentFamily::Percussion },
        { uuid::Odaiko, InstrumentFamily::Percussion },
        { uuid::Okawa, InstrumentFamily::Percussion },
        { uuid::OkedoDodaiko, InstrumentFamily::Percussion },
        { uuid::PahuHula, InstrumentFamily::Percussion },
        { uuid::Pakhavaj, InstrumentFamily::Percussion },
        { uuid::Pandero, InstrumentFamily::Percussion },
        { uuid::PowwowDrum, InstrumentFamily::Percussion },
        { uuid::PuebloDrum, InstrumentFamily::Percussion },
        { uuid::Repinique, InstrumentFamily::Percussion },
        { uuid::Sabar, InstrumentFamily::Percussion },
        { uuid::Sakara, InstrumentFamily::Percussion },
        { uuid::Sampho, InstrumentFamily::Percussion },
        { uuid::Sangban, InstrumentFamily::Percussion },
        { uuid::ShimeDaiko, InstrumentFamily::Percussion },
        { uuid::Surdo, InstrumentFamily::Percussion },
        { uuid::TalkingDrum, InstrumentFamily::Percussion },
        { uuid::Tama, InstrumentFamily::Percussion },
        { uuid::Tamborita, InstrumentFamily::Percussion },
        { uuid::Tamte, InstrumentFamily::Percussion },
        { uuid::Tantan, InstrumentFamily::Percussion },
        { uuid::Tangku, InstrumentFamily::Percussion },
        { uuid::Taphon, InstrumentFamily::Percussion },
        { uuid::TarDrum, InstrumentFamily::Percussion },
        { uuid::Tasha, InstrumentFamily::Percussion },
        { uuid::Thavil, InstrumentFamily::Percussion },
        { uuid::Tombak, InstrumentFamily::Percussion },
        { uuid::Tumbak, InstrumentFamily::Percussion },
        { uuid::Tsuzumi, InstrumentFamily::Percussion },
        { uuid::UchiwaDaiko, InstrumentFamily::Percussion },
        { uuid::Udaku, InstrumentFamily::Percussion },
        { uuid::Zarb, InstrumentFamily::Percussion },
        { uuid::PercussionGeneral, InstrumentFamily::Percussion },
        { uuid::PercAccessories, InstrumentFamily::Percussion },
        { uuid::WindChimes, InstrumentFamily::Percussion },
        { uuid::ChimeTree, InstrumentFamily::Percussion },
        { uuid::BellTree, InstrumentFamily::Percussion },
        { uuid::JingleBells, InstrumentFamily::Percussion },
        { uuid::Tambourine, InstrumentFamily::Percussion },
        { uuid::Triangle, InstrumentFamily::Percussion },
        { uuid::Cymbals, InstrumentFamily::Percussion },
        { uuid::FingerCymbals, InstrumentFamily::Percussion },
        { uuid::CrashCymbal, InstrumentFamily::Percussion },
        { uuid::HiHatCymbal, InstrumentFamily::Percussion },
        { uuid::RideCymbal, InstrumentFamily::Percussion },
        { uuid::SplashCymbal, InstrumentFamily::Percussion },
        { uuid::TamTam, InstrumentFamily::Percussion },
        { uuid::Gong, InstrumentFamily::Percussion },
        { uuid::AgogoBells, InstrumentFamily::Percussion },
        { uuid::AirHorn, InstrumentFamily::Percussion },
        { uuid::BrakeDrum, InstrumentFamily::Percussion },
        { uuid::Cabasa, InstrumentFamily::Percussion },
        { uuid::Cajon, InstrumentFamily::Percussion },
        { uuid::Castanets, InstrumentFamily::Percussion },
        { uuid::Clap, InstrumentFamily::Percussion },
        { uuid::Clapper, InstrumentFamily::Percussion },
        { uuid::Claves, InstrumentFamily::Percussion },
        { uuid::Cowbell, InstrumentFamily::Percussion },
        { uuid::Cuica, InstrumentFamily::Percussion },
        { uuid::Guiro, InstrumentFamily::Percussion },
        { uuid::Maracas, InstrumentFamily::Percussion },
        { uuid::PoliceWhistle, InstrumentFamily::Percussion },
        { uuid::Rainstick, InstrumentFamily::Percussion },
        { uuid::Ratchet, InstrumentFamily::Percussion },
        { uuid::Rattle, InstrumentFamily::Percussion },
        { uuid::SandBlock, InstrumentFamily::Percussion },
        { uuid::Shakers, InstrumentFamily::Percussion },
        { uuid::Spoons, InstrumentFamily::Percussion },
        { uuid::TempleBlocks, InstrumentFamily::Percussion },
        { uuid::Vibraslap, InstrumentFamily::Percussion },
        { uuid::Washboard, InstrumentFamily::Percussion },
        { uuid::Whip, InstrumentFamily::Percussion },
        { uuid::WindMachine, InstrumentFamily::Percussion },
        { uuid::WoodBlocks, InstrumentFamily::Percussion },
        { uuid::CengCengGamelan, InstrumentFamily::Percussion },
        { uuid::GongAgengGamelan, InstrumentFamily::Percussion },
        { uuid::KempulGamelan, InstrumentFamily::Percussion },
        { uuid::KempyangGamelan, InstrumentFamily::Percussion },
        { uuid::KenongGamelan, InstrumentFamily::Percussion },
        { uuid::KetukGamelan, InstrumentFamily::Percussion },
        { uuid::ReyongGamelan, InstrumentFamily::PitchedPercussion },
        { uuid::Adodo, InstrumentFamily::Percussion },
        { uuid::AeolianHarp, InstrumentFamily::Percussion },
        { uuid::Afoxe, InstrumentFamily::Percussion },
        { uuid::AgogoBlock, InstrumentFamily::Percussion },
        { uuid::Agung, InstrumentFamily::Percussion },
        { uuid::AgungAtamLang, InstrumentFamily::Percussion },
        { uuid::Ahoko, InstrumentFamily::Percussion },
        { uuid::Babendil, InstrumentFamily::Percussion },
        { uuid::BasicAfricanPercussion, InstrumentFamily::Percussion },
        { uuid::BasicIndianPercussion, InstrumentFamily::Percussion },
        { uuid::BasicMiddleEastPercussion, InstrumentFamily::Percussion },
        { uuid::Berimbau, InstrumentFamily::Percussion },
        { uuid::Bo, InstrumentFamily::Percussion },
        { uuid::Bones, InstrumentFamily::Percussion },
        { uuid::BongoBells, InstrumentFamily::Percussion },
        { uuid::Bullroarer, InstrumentFamily::Percussion },
        { uuid::Caxixi, InstrumentFamily::Percussion },
        { uuid::ChachaBells, InstrumentFamily::Percussion },
        { uuid::Chabara, InstrumentFamily::Percussion },
        { uuid::Chanchiki, InstrumentFamily::Percussion },
        { uuid::Chimta, InstrumentFamily::Percussion },
        { uuid::ChinaTempleBlocks, InstrumentFamily::Percussion },
        { uuid::ChineseCymbals, InstrumentFamily::Percussion },
        { uuid::ChineseGongs, InstrumentFamily::Percussion },
        { uuid::ChinesePercussionEnsemble, InstrumentFamily::Percussion },
        { uuid::Ching, InstrumentFamily::Percussion },
        { uuid::Chippli, InstrumentFamily::Percussion },
        { uuid::Daff, InstrumentFamily::Percussion },
        { uuid::Dafli, InstrumentFamily::Percussion },
        { uuid::Dawuro, InstrumentFamily::Percussion },
        { uuid::Def, InstrumentFamily::Percussion },
        { uuid::Doira, InstrumentFamily::Percussion },
        { uuid::EweDrumAtoke, InstrumentFamily::Percussion },
        { uuid::EweDrumAxatse, InstrumentFamily::Percussion },
        { uuid::EweDrumGangokui, InstrumentFamily::Percussion },
        { uuid::FlexatonePerc, InstrumentFamily::Percussion },
        { uuid::Gandingan, InstrumentFamily::Percussion },
        { uuid::Ganza, InstrumentFamily::Percussion },
        { uuid::Ghatam, InstrumentFamily::Percussion },
        { uuid::Ghungroo, InstrumentFamily::Percussion },
        { uuid::Gome, InstrumentFamily::Percussion },
        { uuid::Guban, InstrumentFamily::Percussion },
        { uuid::HandCymbal, InstrumentFamily::Percussion },
        { uuid::Hang, InstrumentFamily::Percussion },
        { uuid::Hatheli, InstrumentFamily::Percussion },
        { uuid::Hosho, InstrumentFamily::Percussion },
        { uuid::Hyoushigi, InstrumentFamily::Percussion },
        { uuid::Ibo, InstrumentFamily::Percussion },
        { uuid::IndianGong, InstrumentFamily::Percussion },
        { uuid::Ipu, InstrumentFamily::Percussion },
        { uuid::Jawbone, InstrumentFamily::Percussion },
        { uuid::KaEkeEke, InstrumentFamily::Percussion },
        { uuid::Kagul, InstrumentFamily::Percussion },
        { uuid::Kalaau, InstrumentFamily::Percussion },
        { uuid::Kashiklar, InstrumentFamily::Percussion },
        { uuid::Kesi, InstrumentFamily::Percussion },
        { uuid::Khartal, InstrumentFamily::Percussion },
        { uuid::Kkwaenggwari, InstrumentFamily::Percussion },
        { uuid::Kpokopoko, InstrumentFamily::Percussion },
        { uuid::KrinSlitDrum, InstrumentFamily::Percussion },
        { uuid::LavaStones, InstrumentFamily::Percussion },
        { uuid::LuoGong, InstrumentFamily::Percussion },
        { uuid::Manjeera, InstrumentFamily::Percussion },
        { uuid::PanClappers, InstrumentFamily::Percussion },
        { uuid::Patschen, InstrumentFamily::Percussion },
        { uuid::RattleCog, InstrumentFamily::Percussion },
        { uuid::Riq, InstrumentFamily::Percussion },
        { uuid::Shekere, InstrumentFamily::Percussion },
        { uuid::Sistre, InstrumentFamily::Percussion },
        { uuid::Sistrum, InstrumentFamily::Percussion },
        { uuid::SlideWhistlePercClef, InstrumentFamily::Percussion },
        { uuid::SlitDrum, InstrumentFamily::Percussion },
        { uuid::Snap, InstrumentFamily::Percussion },
        { uuid::Stamp, InstrumentFamily::Percussion },
        { uuid::StirDrum, InstrumentFamily::Percussion },
        { uuid::TebYoshi, InstrumentFamily::Percussion },
        { uuid::Televi, InstrumentFamily::Percussion },
        { uuid::Teponaztli, InstrumentFamily::Percussion },
        { uuid::ThaiGong, InstrumentFamily::Percussion },
        { uuid::TibetanCymbals, InstrumentFamily::Percussion },
        { uuid::TicTocBlock, InstrumentFamily::Percussion },
        { uuid::TimbaleBell, InstrumentFamily::Percussion },
        { uuid::Tinaja, InstrumentFamily::Percussion },
        { uuid::Tingsha, InstrumentFamily::Percussion },
        { uuid::Toere, InstrumentFamily::Percussion },
        { uuid::ToneTang, InstrumentFamily::Percussion },
        { uuid::Trychel, InstrumentFamily::Percussion },
        { uuid::Udu, InstrumentFamily::Percussion },
        { uuid::Zills, InstrumentFamily::Percussion },

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
    return it == map.end() ? InstrumentFamily::Unspecified : it->second;
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
