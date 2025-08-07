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
        "articulation", "beatChart", "enclosure", "expression", "fonts", "frame",
        "instrumentUsed", "layerAttributes", "measure", "measureNumberRegion",
        "miscellaneous", "mmrest", "multiStaffInstrumentGroup", "page",
        "partDefinition", "partGlobals", "percussionNote", "repeats",
        "shape", "smartShape", "staff", "staffStyle", "staffSystem",
        "text", "textBlock"
    };

    std::cout << "Benchmarking others arrays:\n";

    for (const auto& nodeId : nodeIds) {
        using ObjectPool = ObjectPool<OthersBase>;
        ObjectPool::ObjectKey key(nodeId, partId);

        auto start = clock::now();
        auto result = doc->getOthers()->ObjectPool::getArray<OthersBase>(key, partId);
        auto end = clock::now();

        const auto elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "  " << nodeId << ": "
                  << result.size() << " objects found in "
                  << elapsedMs << " μs\n";
    }
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
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read musx file.\n";
        return 1;
    }

    auto doc = loadDocument(buffer);

    if (!doc) {
        std::cerr << "Failed to load musx document.\n";
        return 1;
    }

    benchmarkEntries(doc);
    benchmarkOthersArrays(doc);

    return 0;
}
