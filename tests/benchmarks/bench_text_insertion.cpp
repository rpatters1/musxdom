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

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "musx/dom/ObjectPool.h"
#include "musx/dom/Texts.h"
#include "musx/factory/DocumentFactory.h"

namespace {

using BenchmarkText = musx::dom::texts::BlockText;
using Clock = std::chrono::steady_clock;

constexpr std::size_t benchmarkTextCount = 10'000;
constexpr std::size_t warmupSampleCount = 5;
constexpr std::size_t measuredSampleCount = 25;
constexpr int outputPrecision = 1;
constexpr int failureExitCode = 1;

struct Sample
{
    std::chrono::nanoseconds insertion;
    std::chrono::nanoseconds finalization;
};

[[noreturn]] void fail(const std::string& message)
{
    throw std::runtime_error(message);
}

Sample runSample()
{
    auto session = musx::factory::DocumentFactory::begin();
    const auto document = session.getDocument();
    auto& pool = *document->getTexts();

    std::vector<std::shared_ptr<BenchmarkText>> instances;
    instances.reserve(benchmarkTextCount);
    for (std::size_t index = 0; index < benchmarkTextCount; ++index) {
        auto instance = std::make_shared<BenchmarkText>(document,
            musx::dom::SCORE_PARTID, musx::dom::EnigmaBase::ShareMode::All,
            static_cast<musx::dom::Cmper>(index));
        instance->text = "benchmark text";
        instances.push_back(std::move(instance));
    }

    const auto insertionStart = Clock::now();
    for (auto& instance : instances) {
        pool.add(BenchmarkText::XmlNodeName, std::move(instance));
    }
    const auto insertionEnd = Clock::now();

    const auto finalizationStart = Clock::now();
    const auto finished = std::move(session).finish();
    const auto finalizationEnd = Clock::now();

    const auto texts = finished->getTexts()->getArray<BenchmarkText>();
    if (texts.size() != benchmarkTextCount) {
        fail("TextsPool contains the wrong number of benchmark objects");
    }
    if (!finished->getTexts()->get<BenchmarkText>(0)
        || !finished->getTexts()->get<BenchmarkText>(
            static_cast<musx::dom::Cmper>(benchmarkTextCount - 1))) {
        fail("TextsPool does not contain the boundary benchmark objects");
    }

    return {
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            insertionEnd - insertionStart),
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            finalizationEnd - finalizationStart)};
}

void printSummary(const std::string& name, std::vector<std::chrono::nanoseconds> samples)
{
    std::sort(samples.begin(), samples.end());
    const auto minimum = samples.front().count();
    const auto median = samples[samples.size() / 2].count();
    const auto p90 = samples[(samples.size() - 1) * 90 / 100].count();
    const auto perObject = static_cast<double>(median) / benchmarkTextCount;

    std::cout << name
              << ": min=" << minimum << " ns"
              << " median=" << median << " ns"
              << " p90=" << p90 << " ns"
              << " median_per_object=" << std::fixed << std::setprecision(outputPrecision)
              << perObject << " ns\n";
}

} // namespace

int main()
{
    try {
        for (std::size_t sample = 0; sample < warmupSampleCount; ++sample) {
            static_cast<void>(runSample());
        }

        std::vector<std::chrono::nanoseconds> insertionSamples;
        std::vector<std::chrono::nanoseconds> finalizationSamples;
        insertionSamples.reserve(measuredSampleCount);
        finalizationSamples.reserve(measuredSampleCount);
        for (std::size_t sample = 0; sample < measuredSampleCount; ++sample) {
            const auto result = runSample();
            insertionSamples.push_back(result.insertion);
            finalizationSamples.push_back(result.finalization);
        }

        std::cout << "objects=" << benchmarkTextCount
                  << " warmups=" << warmupSampleCount
                  << " samples=" << measuredSampleCount << '\n';
        printSummary("insertion", std::move(insertionSamples));
        printSummary("finalization", std::move(finalizationSamples));
    } catch (const std::exception& error) {
        std::cerr << "text insertion benchmark failed: " << error.what() << '\n';
        return failureExitCode;
    }
    return 0;
}
