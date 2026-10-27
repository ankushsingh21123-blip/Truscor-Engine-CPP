#include "risk_engine.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "========================================================\n";
    std::cout << "   TRUSCOR ENGINE (C++20) — HIGH-THROUGHPUT RISK CORE   \n";
    std::cout << "   Author: Ankush Singh | Scaler & BITS Pilani          \n";
    std::cout << "========================================================\n\n";

    truscor::AgentOperationalState agentState{
        .agentId = "agent-autonomous-viber-01",
        .toolFailureRate = 0.024,
        .contextEntropy = 0.12,
        .unauthorizedActionScore = 0.001,
        .latencyDrift = 0.015
    };

    truscor::RiskEngine engine;
    const size_t SIMULATION_COUNT = 2'000'000;

    std::cout << ">> Running " << SIMULATION_COUNT << " Monte-Carlo risk simulations...\n";
    auto metrics = engine.quantifyRisk(agentState, SIMULATION_COUNT);

    std::cout << "\n>> Benchmark Completed Successfully:\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  Agent ID:               " << agentState.agentId << "\n";
    std::cout << "  Simulations Evaluated:  " << metrics.totalSimulations << "\n";
    std::cout << "  Execution Latency:      " << metrics.executionLatencyMs << " ms\n";
    std::cout << "  Throughput:             " 
              << static_cast<size_t>((metrics.totalSimulations / (metrics.executionLatencyMs / 1000.0)))
              << " simulations/sec\n";
    std::cout << "  95% Value at Risk:      " << metrics.valueAtRisk95 << "\n";
    std::cout << "  99% Value at Risk:      " << metrics.valueAtRisk99 << "\n";
    std::cout << "  Expected Shortfall:     " << metrics.expectedShortfall << "\n";
    std::cout << "  TRUSCOR Safety Score:   " << std::setprecision(1) << metrics.operationalScore << " / 100.0\n";
    std::cout << "--------------------------------------------------------\n";

    return 0;
}
