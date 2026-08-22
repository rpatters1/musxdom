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

#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <memory>
#include <random>

#include "musx/musx.h"

using namespace musx::dom;

// provide access to the raw others pool
namespace bench {
template<>
class PoolAccessor<OthersPool>
{
public:
    static ObjectPool<OthersBase>& get(OthersPool& o) { return o.m_pool; }
};

template<>
class PoolAccessor<EntryPool>
{
public:
    static const std::unordered_map<EntryNumber, std::shared_ptr<Entry>>& get(EntryPool& e) { return e.m_pool; }
};

} // namespace bench

template <typename T>
static std::string duration(T timeDiff)
{
    auto loadMs = std::chrono::duration_cast<std::chrono::nanoseconds>(timeDiff).count();
    if (loadMs < 1500) {
        return std::to_string(loadMs) + " ns";
    }
    loadMs = std::chrono::duration_cast<std::chrono::microseconds>(timeDiff).count();
    if (loadMs < 1500) {
        return std::to_string(loadMs) + " µs";
    }
    loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count();
    return std::to_string(loadMs) + " ms";
}

DocumentPtr loadDocument(const std::vector<char>& buffer)
{
    using clock = std::chrono::high_resolution_clock;

    {
        auto loadTinyStart = clock::now();
        auto docTiny = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(buffer);
        auto loadTinyEnd = clock::now();
        auto loadMs = loadTinyEnd - loadTinyStart;
        std::cout << "Loaded enigmaxml with tinyxml2 in " << duration(loadMs) << "\n";
    }
    {
        auto loadRapidStart = clock::now();
        auto docRapid = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(buffer);
        auto loadRapidEnd = clock::now();
        auto loadMs = loadRapidEnd - loadRapidStart;
        std::cout << "Loaded enigmaxml with rapidxml in " << duration(loadMs) << "\n";
    }
    {
        auto loadPugiStart = clock::now();
        auto docPugi = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(buffer);
        auto loadPugiEnd = clock::now();
        auto loadMs = loadPugiEnd - loadPugiStart;
        std::cout << "Loaded enigmaxml with pugi in " << duration(loadMs) << "\n";
        return docPugi;
    }
}

void adHocTest([[maybe_unused]]const DocumentPtr& doc)
{
/*
    9/3/2025: Test how brackets are handled in ShapeDef.

    if (auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, 6)) {
        shape->iterateInstructions([&](others::ShapeDefInstructionType instruction, std::vector<int> data) -> bool {
            std::cout << "instruction: " << int(instruction) << std::endl;
            std::cout << "    data:";
            for (const auto& item : data) {
                std::cout << ' ' << item;
            }
            std::cout << std::endl;
            return true;
        });
    }
*/
}

void traverseEntries(const DocumentPtr& doc)
{
    auto entryPool = bench::PoolAccessor<EntryPool>::get(*doc->getEntries());

    using clock = std::chrono::high_resolution_clock;

    std::unordered_set<EntryNumber> entryList;
    std::unordered_set<EntryNumber> entryListLightweight;

    size_t entryCount = 0;
    auto iterateStart = clock::now();
    doc->iterateEntries(SCORE_PARTID, [&](const EntryInfoPtr& entryInfo) {
        if (entryInfo.getFrame()->getContext()->mirrorFrame) {
            return true;
        }
        auto entryNum = entryInfo->getEntry()->getEntryNumber();
        auto result = entryList.emplace(entryNum);
        if (!result.second) {
            std::cout << "Duplicate entry " << entryNum << " found at staff " << entryInfo.getStaff() << " measure " << entryInfo.getMeasure() << ".\n";
        }
        entryCount++;
        return true;
    });
    auto iterateEnd = clock::now();
    auto iterateMs = iterateEnd - iterateStart;
    std::cout << "Using full iterator, iterated " << entryCount << " entries in " << duration(iterateMs) << "\n";

    entryCount = 0;
    size_t extraEntries = 0;
    iterateStart = clock::now();
    auto gfHolds = doc->getDetails()->getArray<details::GFrameHold>(SCORE_PARTID);
    for (const auto& gfHold : gfHolds) {
        if (gfHold->mirrorFrame) {
            continue;
        }
        gfHold->iterateRawEntries([&](const MusxInstance<Entry>& entry, LayerIndex) {
            if (gfHold->getCmper1() != 32767) { // 32767 is the Studio View click staff
                auto entryNum = entry->getEntryNumber();
                auto result = entryList.emplace(entryNum);
                if (result.second) {
                    std::cout << "New entry " << entryNum << " added in lightweight search at staff " << gfHold->getCmper1() << " measure " << gfHold->getCmper2() << ".\n";
                }
                result = entryListLightweight.emplace(entryNum);
                if (!result.second) {
                    std::cout << "Duplicate lightweight entry " << entryNum << " found at staff " << gfHold->getCmper1() << " measure " << gfHold->getCmper2() << ".\n";
                }
            } else {
                extraEntries++;
            }
            entryCount++;
            return true;
        });
    }
    iterateEnd = clock::now();
    iterateMs = iterateEnd - iterateStart;
    std::cout << "Using lightweight search, iterated " << entryCount << " entries in " << duration(iterateMs) << "\n";
    std::cout << "Encountered " << entryList.size() << " unique entries not including " << extraEntries << " extra entries.\n";

    std::vector<EntryNumber> missing;
    for (const auto& x : entryList) {
        if (entryListLightweight.find(x) == entryListLightweight.end()) {
            missing.push_back(x);
        }
    }
    std::cout << "Encountered " << missing.size() << " missing entries in the lightweight search.\n";

    std::vector<EntryNumber> orphanEntries;
    for (const auto& [num, entry] : entryPool) {
        if (!entry->location.found()) {
            orphanEntries.push_back(num);
        }
        entry->location.clear();
    }
    std::cout << "Encountered " << orphanEntries.size() << " entries with no location.\n";

    iterateStart = clock::now();
    Entry::calcLocations(doc);
    iterateEnd = clock::now();
    iterateMs = iterateEnd - iterateStart;
    std::cout << "Entry::calcLocations ran for " << duration(iterateMs) << "\n";
}

void benchmarkEntries(const DocumentPtr& doc)
{
    auto entryPool = bench::PoolAccessor<EntryPool>::get(*doc->getEntries());

    using clock = std::chrono::high_resolution_clock;

    const int repetitions = 1'000'000;
    int foundCount = 0;

    // Replace these limits with a realistic Cmper range
    constexpr int cmperMin = 1;
    constexpr int cmperMax = 200'000;

    // Fixed seed for reproducibility
    std::mt19937 rng(42); // Mersenne Twister with fixed seed
    std::uniform_int_distribution<int> dist(cmperMin, cmperMax);

    // Pre-generate pseudo-random Cmper values
    std::vector<int> cmperIds;
    cmperIds.reserve(repetitions);
    for (int i = 0; i < repetitions; ++i) {
        cmperIds.push_back(dist(rng));
    }

    auto lookupStart = clock::now();
    for (int i = 0; i < repetitions; ++i) {
        auto entry = doc->getEntries()->get(cmperIds[i]);
        if (entry) ++foundCount;
    }
    auto lookupEnd = clock::now();

    auto lookupMs = lookupEnd - lookupStart;
    std::cout << "Looked up " << repetitions << " entries in " << duration(lookupMs) << " ("
            << foundCount << " found)\n";
            
    constexpr int MAX_ENTRIES_TO_CREATE = 10000;
    int numEntriesCreated = 0;
    int numNotFound = 0;
    lookupStart = clock::now();
    for (const auto& [entNum, _] : entryPool) {
        if (EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, entNum)) {
            if (++numEntriesCreated >= MAX_ENTRIES_TO_CREATE) {
                break;
            }
        } else {
            numNotFound++;
        }
    }
    lookupEnd = clock::now();
    lookupMs = lookupEnd - lookupStart;
    std::cout << "Created " << numEntriesCreated << " EntryInfoPtr instances in " << duration(lookupMs) << " ("
            << numNotFound << " not found)\n";
}

void benchmarkOthersArrays(const DocumentPtr& doc, Cmper partId)
{
    using clock = std::chrono::high_resolution_clock;

    const std::vector<std::pair<std::string_view, std::type_index>> nodeIds = {
        {others::ArticulationDef::XmlNodeName, typeid(others::ArticulationDef)},
        {others::BeatChartElement::XmlNodeName, typeid(others::BeatChartElement)},
        {others::Frame::XmlNodeName, typeid(others::Frame)},
        {others::StaffUsed::XmlNodeName, typeid(others::StaffUsed)},
        {others::LayerAttributes::XmlNodeName, typeid(others::LayerAttributes)},
        {others::Measure::XmlNodeName, typeid(others::Measure)},
        {others::MeasureExprAssign::XmlNodeName, typeid(others::MeasureExprAssign)},
        {others::MeasureNumberRegion::XmlNodeName, typeid(others::MeasureNumberRegion)},
        {"miscNoExist", typeid(void)},
        {others::MultimeasureRest::XmlNodeName, typeid(others::MultimeasureRest)},
        {others::MultiStaffGroupId::XmlNodeName, typeid(others::MultiStaffGroupId)},
        {others::Page::XmlNodeName, typeid(others::Page)},
        {others::PageTextAssign::XmlNodeName, typeid(others::PageTextAssign)},
        {others::PartDefinition::XmlNodeName, typeid(others::PartDefinition)},
        {others::PartGlobals::XmlNodeName, typeid(others::PartGlobals)},
        {others::RepeatEndingStart::XmlNodeName, typeid(others::RepeatEndingStart)},
        {others::ShapeDef::XmlNodeName, typeid(others::ShapeDef)},
        {others::SmartShape::XmlNodeName, typeid(others::SmartShape)},
        {others::Staff::XmlNodeName, typeid(others::Staff)},
        {others::StaffStyle::XmlNodeName, typeid(others::StaffStyle)},
        {others::StaffSystem::XmlNodeName, typeid(others::StaffSystem)},
        {others::TextBlock::XmlNodeName, typeid(others::TextBlock)}
    };

    std::string partName = "Part " + std::to_string(partId);
    if (const auto partDef = doc->getOthers()->get<others::PartDefinition>(SCORE_PARTID, partId)) {
        partName = partDef->getName();
    }
    std::cout << "Benchmarking others arrays:\n";

    auto othersPool = bench::PoolAccessor<OthersPool>::get(*doc->getOthers());

    for (const auto& [nodeId, typeId] : nodeIds) {
        using ObjectPool = ObjectPool<OthersBase>;
        ObjectPool::ObjectKey key(typeId, nodeId, partId);

        auto start = clock::now();
        auto result = othersPool.getArrayForPart<OthersBase>(key);
        auto end = clock::now();

        const auto elapsedMs = end - start;

        std::cout << "  " << nodeId << " " << partName << ": "
                  << result.size() << " objects found in "
                  << duration(elapsedMs) << "\n";
    }
}

template <typename T>
class BenchmarkPoolShim : public ObjectPool<T>
{
public:
    using Base = ObjectPool<T>;
    using ObjectKey = typename Base::ObjectKey;

    // Construct from an existing pool by shallow-copying shared_ptrs
    explicit BenchmarkPoolShim(const Base& source)
        : Base(source) // invokes ObjectPool<T>'s copy constructor
    {
    }

    // Expose the protected getSource<T> method for benchmarking
    using Base::getSource;
};

void benchmarkOthers(const DocumentPtr& doc)
{
    using clock = std::chrono::high_resolution_clock;

    std::cout << "Benchmarking individual Others lookups:\n";

    // These part IDs should exist in the file; adjust if needed
    constexpr Cmper score = SCORE_PARTID;
    constexpr Cmper part1 = 1;
    constexpr Cmper part2 = 2;
    constexpr Cmper meas5 = 5;
    constexpr Cmper staff2 = 2;

    struct TestCase {
        std::type_index typeId;
        std::string_view nodeId;
        Cmper partId;
        std::optional<Cmper> cmper1;
        std::optional<Inci> inci;
    };

    const std::vector<TestCase> cases = {
        {typeid(others::Staff), others::Staff::XmlNodeName, score, staff2, std::nullopt},
        {typeid(others::StaffStyle), others::StaffStyle::XmlNodeName, score, Cmper{2}, std::nullopt},
        {typeid(others::SmartShape), others::SmartShape::XmlNodeName, score, Cmper{337}, std::nullopt},
        {typeid(others::StaffUsed), others::StaffUsed::XmlNodeName, score, Cmper{17}, Inci{3}},
        {typeid(others::PartGlobals), others::PartGlobals::XmlNodeName, part1, MUSX_GLOBALS_CMPER, std::nullopt},
        {typeid(others::TextBlock), others::TextBlock::XmlNodeName, score, Cmper{345}, std::nullopt},
        {typeid(others::Measure), others::Measure::XmlNodeName, part2, meas5, std::nullopt},
        {typeid(others::RepeatEndingStart), others::RepeatEndingStart::XmlNodeName, score, meas5, std::nullopt},
        {typeid(others::LayerAttributes), others::LayerAttributes::XmlNodeName, score, Cmper{1}, std::nullopt},
        {typeid(others::Page), others::Page::XmlNodeName, part2, Cmper{4}, std::nullopt},
        {typeid(others::Frame), others::Frame::XmlNodeName, score, Cmper{678}, Inci{0}},
        {typeid(void), "nonExistent", part2, Cmper{12345}, Inci{0}}
    };

    int foundCount = 0;
    auto othersPool = bench::PoolAccessor<OthersPool>::get(*doc->getOthers());

    for (const auto& c : cases) {
        using ObjectPool = ObjectPool<OthersBase>;
        ObjectPool::ObjectKey key(c.typeId, c.nodeId, c.partId,
            c.cmper1, std::nullopt, c.inci);

        auto start = clock::now();
        auto result = othersPool.getSource<OthersBase>(key);
        auto end = clock::now();

        const auto elapsedUs = end - start;

        std::cout << "  " << c.nodeId
                  << " (part=" << c.partId
                  << (c.cmper1 ? ", cmper1=" + std::to_string(*c.cmper1) : "")
                  << (c.inci ? ", inci=" + std::to_string(*c.inci) : "")
                  << "): " << (result ? "found" : "NOT FOUND")
                  << " in " << duration(elapsedUs) << "\n";

        if (result) ++foundCount;
    }
    std::cout << "  " << foundCount << " of " << cases.size() << " cases found.\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path-to-musx-file>\n";
        return 1;
    }

    musx::util::Logger::setCallback([](musx::util::Logger::LogLevel level, const std::string& msg) {
        using Level = musx::util::Logger::LogLevel;
        if (level == Level::Error || level == Level::Warning) {
            std::cout << msg << std::endl;
        }
    });

    std::filesystem::path filePath = argv[1];
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open musx file: " << filePath << '\n';
        return 1;
    }

    const std::streamsize size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("tellg() failed");
    }
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(static_cast<size_t>(size));
    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read musx file.\n";
        return 1;
    }

    auto doc = loadDocument(buffer);

    if (!doc) {
        std::cerr << "Failed to load musx document.\n";
        return 1;
    }
    auto partDefs = others::PartDefinition::getInUserOrder(doc);

    adHocTest(doc);
    traverseEntries(doc);
    benchmarkEntries(doc);
    benchmarkOthersArrays(doc, SCORE_PARTID);
    if (partDefs.size() >= 2) {
        benchmarkOthersArrays(doc, partDefs[1]->getCmper());
    }
    if (partDefs.size() >= 3) {
        benchmarkOthersArrays(doc, partDefs[2]->getCmper());
    }
    benchmarkOthers(doc);

    return 0;
}
