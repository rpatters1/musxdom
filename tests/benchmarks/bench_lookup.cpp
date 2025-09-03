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
} // namespace bench

DocumentPtr loadDocument(const std::vector<char>& buffer)
{
    using clock = std::chrono::high_resolution_clock;

    {
        auto loadTinyStart = clock::now();
        auto docTiny = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(buffer);
        auto loadTinyEnd = clock::now();
        auto loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadTinyEnd - loadTinyStart).count();
        std::cout << "Loaded enigmaxml with tinyxml2 in " << loadMs << " ms\n";
    }
    {
        auto loadRapidStart = clock::now();
        auto docRapid = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(buffer);
        auto loadRapidEnd = clock::now();
        auto loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadRapidEnd - loadRapidStart).count();
        std::cout << "Loaded enigmaxml with rapidxml in " << loadMs << " ms\n";
    }
    {
        auto loadPugiStart = clock::now();
        auto docPugi = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(buffer);
        auto loadPugiEnd = clock::now();
        auto loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadPugiEnd - loadPugiStart).count();
        std::cout << "Loaded enigmaxml with pugi in " << loadMs << " ms\n";
        return docPugi;
    }
}

void adHocTest(const DocumentPtr& doc)
{
    if (auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, 6)) {
        shape->iterateInstructions([&](others::ShapeDef::InstructionType instruction, std::vector<int> data) -> bool {
            std::cout << "instruction: " << int(instruction) << std::endl;
            std::cout << "    data:";
            for (const auto& item : data) {
                std::cout << ' ' << item;
            }
            std::cout << std::endl;
            return true;
        });
    } 
}

void benchmarkEntries(const DocumentPtr& doc)
{
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

    auto lookupMs = std::chrono::duration_cast<std::chrono::milliseconds>(lookupEnd - lookupStart).count();

    std::cout << "Looked up " << repetitions << " entries in " << lookupMs << " ms ("
            << foundCount << " found)\n";
}

void benchmarkOthersArrays(const DocumentPtr& doc)
{
    using clock = std::chrono::high_resolution_clock;

    constexpr Cmper partId = SCORE_PARTID;
    const std::vector<std::string> nodeIds = {
        "articDef", "beatChart", "frameSpec",
        "instUsed", "layerAtts", "measSpec", "measNumbRegion",
        "miscNoExist", "mmRest", "multiStaffInstGroup", "pageSpec",
        "pageTextAssign", "partDef", "partGlobals", "repeatEndingStart",
        "shapeDef", "smartShape", "staffSpec", "staffStyle", "staffSystemSpec",
        "textBlock"
    };

    std::cout << "Benchmarking others arrays:\n";

    auto othersPool = bench::PoolAccessor<OthersPool>::get(*doc->getOthers());

    for (const auto& nodeId : nodeIds) {
        using ObjectPool = ObjectPool<OthersBase>;
        ObjectPool::ObjectKey key(nodeId, partId);

        auto start = clock::now();
        auto result = othersPool.getArray<OthersBase>(key, partId);
        auto end = clock::now();

        const auto elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "  " << nodeId << ": "
                  << result.size() << " objects found in "
                  << elapsedMs << " μs\n";
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
        std::string nodeId;
        Cmper partId;
        std::optional<Cmper> cmper1;
        std::optional<Inci> inci;
    };

    const std::vector<TestCase> cases = {
        {"staffSpec", score, staff2, std::nullopt},
        {"staffStyle", score, 2, std::nullopt},
        {"smartShape", score, 337, std::nullopt},
        {"instUsed", score, 17, 3},
        {"partGlobals", part1, MUSX_GLOBALS_CMPER, std::nullopt},
        {"textBlock", score, 345, std::nullopt},
        {"measSpec", part2, meas5, std::nullopt},
        {"repeatEndingStart", score, meas5, std::nullopt},
        {"layerAtts", score, 1, std::nullopt},
        {"pageSpec", part2, 4, std::nullopt},
        {"frameSpec", score, 678, 0},
        {"nonExistent", part2, 12345, 0}
    };

    int foundCount = 0;
    auto othersPool = bench::PoolAccessor<OthersPool>::get(*doc->getOthers());

    for (const auto& c : cases) {
        using ObjectPool = ObjectPool<OthersBase>;
        ObjectPool::ObjectKey key(c.nodeId, c.partId, c.cmper1, std::nullopt, c.inci);

        auto start = clock::now();
        auto result = othersPool.getSource<OthersBase>(key);
        auto end = clock::now();

        const auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "  " << c.nodeId
                  << " (part=" << c.partId
                  << (c.cmper1 ? ", cmper1=" + std::to_string(*c.cmper1) : "")
                  << (c.inci ? ", inci=" + std::to_string(*c.inci) : "")
                  << "): " << (result ? "found" : "NOT FOUND")
                  << " in " << elapsedUs << " μs\n";

        if (result) ++foundCount;
    }
    std::cout << foundCount << " of " << cases.size() << " cases found.\n";
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

    adHocTest(doc);
    benchmarkEntries(doc);
    benchmarkOthersArrays(doc);
    benchmarkOthers(doc);

    return 0;
}
