#pragma once
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <atomic>
#include <span>
#include <chrono>

namespace truscor {

struct RiskMetrics {
    double valueAtRisk95;      // 95% VaR
    double valueAtRisk99;      // 99% VaR
    double expectedShortfall;  // CVaR
    double operationalScore;   // 0.0 - 100.0 Truscor Trust Rating
    double executionLatencyMs;
    size_t totalSimulations;
};

struct AgentOperationalState {
    std::string agentId;
    double toolFailureRate;
    double contextEntropy;
    double unauthorizedActionScore;
    double latencyDrift;
};

class RiskEngine {
public:
    explicit RiskEngine(size_t workerThreads = std::thread::hardware_concurrency());
    ~RiskEngine() = default;

    /**
     * @brief Run high-throughput multithreaded Monte-Carlo risk simulations.
     * @param state Current runtime operational telemetry of the agent.
     * @param iterations Number of simulation cycles (e.g. 1,000,000+).
     */
    RiskMetrics quantifyRisk(const AgentOperationalState& state, size_t iterations = 1'000'000);

private:
    size_t threadCount_;
};

} // namespace truscor
