#include "risk_engine.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <future>

namespace truscor {

RiskEngine::RiskEngine(size_t workerThreads)
    : threadCount_(workerThreads == 0 ? 4 : workerThreads) {}

RiskMetrics RiskEngine::quantifyRisk(const AgentOperationalState& state, size_t iterations) {
    auto startTime = std::chrono::high_resolution_clock::now();

    size_t itersPerThread = iterations / threadCount_;
    std::vector<std::future<std::vector<double>>> futures;
    futures.reserve(threadCount_);

    for (size_t t = 0; t < threadCount_; ++t) {
        futures.push_back(std::async(std::launch::async, [t, itersPerThread, &state]() {
            std::vector<double> simulatedLosses;
            simulatedLosses.reserve(itersPerThread);

            // Fast thread-local PRNG
            std::mt19937_64 rng(1337 + t);
            std::normal_distribution<double> normDist(0.0, 1.0);
            std::uniform_real_distribution<double> uniformDist(0.0, 1.0);

            const double drift = state.toolFailureRate * 0.4 + state.unauthorizedActionScore * 0.6;
            const double volatility = std::max(0.05, state.contextEntropy * 0.35 + state.latencyDrift * 0.2);

            for (size_t i = 0; i < itersPerThread; ++i) {
                double shock = normDist(rng);
                double jump = (uniformDist(rng) < 0.05) ? (uniformDist(rng) * 2.5) : 0.0;
                double simulatedLoss = drift + (volatility * shock) + jump;
                simulatedLosses.push_back(simulatedLoss);
            }
            return simulatedLosses;
        }));
    }

    // Merge loss vectors
    std::vector<double> allLosses;
    allLosses.reserve(iterations);
    for (auto& f : futures) {
        auto threadResults = f.get();
        allLosses.insert(allLosses.end(), threadResults.begin(), threadResults.end());
    }

    // Sort losses to compute quantile metrics (VaR & CVaR)
    std::sort(allLosses.begin(), allLosses.end());

    size_t idx95 = static_cast<size_t>(allLosses.size() * 0.95);
    size_t idx99 = static_cast<size_t>(allLosses.size() * 0.99);

    double var95 = allLosses[std::min(idx95, allLosses.size() - 1)];
    double var99 = allLosses[std::min(idx99, allLosses.size() - 1)];

    // Expected Shortfall (CVaR 95)
    double cvarSum = 0.0;
    size_t tailCount = allLosses.size() - idx95;
    for (size_t i = idx95; i < allLosses.size(); ++i) {
        cvarSum += allLosses[i];
    }
    double cvar95 = tailCount > 0 ? (cvarSum / tailCount) : var95;

    // Calculate normalized Truscor Operational Score [0.0 - 100.0]
    double riskPenalty = (var95 * 30.0) + (var99 * 40.0) + (cvar95 * 30.0);
    double operationalScore = std::clamp(100.0 - (riskPenalty * 15.0), 10.0, 99.9);

    auto endTime = std::chrono::high_resolution_clock::now();
    double latencyMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    return RiskMetrics{
        .valueAtRisk95 = var95,
        .valueAtRisk99 = var99,
        .expectedShortfall = cvar95,
        .operationalScore = operationalScore,
        .executionLatencyMs = latencyMs,
        .totalSimulations = allLosses.size()
    };
}

} // namespace truscor
