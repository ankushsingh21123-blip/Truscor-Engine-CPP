# Truscor-Engine-CPP ⚡🛡️

**A high-throughput C++20 multithreaded Monte-Carlo risk quantification and operational trust scoring engine.**

Designed to evaluate autonomous AI runtime safety, tool-execution volatility, and actuarial tail-risk distributions at millions of iterations per second.

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Multi-threaded](https://img.shields.io/badge/Concurrency-Lock--Free-4B0082?style=for-the-badge)
![SIMD](https://img.shields.io/badge/Vectorization-SIMD%2FAVX2-FF6F00?style=for-the-badge)

---

## 🏛️ Systems Architecture

```text
       ┌─────────────────────────────────────────────────────────┐
       │             AGENT OPERATIONAL TELEMETRY                 │
       │   Tool Failure Rate • Latency Drift • Context Entropy   │
       └────────────────────────────┬────────────────────────────┘
                                    │
                                    ▼
       ┌─────────────────────────────────────────────────────────┐
       │                 TRUSCOR RISK ENGINE (C++20)             │
       │                                                         │
       │  ┌───────────────────┐       ┌───────────────────────┐  │
       │  │ Thread Pool (xN)  │       │ Lock-Free Aggregator  │  │
       │  │ MT19937_64 PRNG   │──────▶│ Custom Memory Pools   │  │
       │  │ Jump Shocks       │       │ Quantile Vector Sort  │  │
       │  └───────────────────┘       └───────────────────────┘  │
       └────────────────────────────┬────────────────────────────┘
                                    │
                                    ▼
       ┌─────────────────────────────────────────────────────────┐
       │                 ACTUARIAL TRUST METRICS                 │
       │                                                         │
       │  📊 95% Value at Risk (VaR)     📊 99% Value at Risk    │
       │  📉 Expected Shortfall (CVaR)   🛡️ Truscor Trust Score   │
       └─────────────────────────────────────────────────────────┘
```

---

## ⚡ Performance Benchmarks

Tested on standard x86-64 multithreaded hardware:

| Simulations | Latency | Throughput | 95% VaR | 99% VaR | Truscor Rating |
|---|---|---|---|---|---|
| **1,000,000** | ~18.4 ms | **~54,300,000 sims/sec** | 0.4281 | 0.8914 | **94.2 / 100** |
| **5,000,000** | ~82.1 ms | **~60,900,000 sims/sec** | 0.4285 | 0.8920 | **94.1 / 100** |

---

## 🛠️ Key Architectural Decisions

1. **Zero-Allocation Critical Path**: Custom block memory pool (`include/memory_pool.hpp`) pre-allocates aligned contiguous memory buffers to completely eliminate heap allocation stalls.
2. **Parallel Stochastic Sampling**: Threads execute independent 64-bit Mersenne Twister PRNG states with zero cross-thread locking overhead.
3. **Quantile Tail Extraction**: Efficient partial sorting and quantile computation for high-confidence regulatory and operational risk ratings.

---

## ⚡ Build & Run

### Prerequisites
- Modern C++20 compiler (`g++` >= 11, `clang` >= 13, or MSVC 2022)
- CMake 3.20+

### Compilation
```bash
# Clone the repository
git clone https://github.com/ankushsingh21123-blip/Truscor-Engine-CPP.git
cd Truscor-Engine-CPP

# Configure and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Run benchmark
./truscor_engine
```

---

## 👨‍💻 Author

**Ankush Singh**  
- GitHub: [@ankushsingh21123-blip](https://github.com/ankushsingh21123-blip)  
- Affiliations: Scaler School of Technology • BITS Pilani  
- Email: ankushsingh21123@gmail.com
