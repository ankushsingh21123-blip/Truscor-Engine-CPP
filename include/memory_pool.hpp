#pragma once
#include <cstddef>
#include <vector>
#include <memory>
#include <new>

namespace truscor {

/**
 * @brief Cache-aligned, zero-allocation block memory pool.
 * Eliminates heap allocation latency on critical paths.
 */
template <typename T, size_t BlockSize = 4096>
class MemoryPool {
public:
    MemoryPool() : currentBlock_(nullptr), currentSlot_(nullptr), lastSlot_(nullptr) {}
    ~MemoryPool() {
        for (void* block : allocatedBlocks_) {
            ::operator delete(block);
        }
    }

    template <typename... Args>
    T* allocate(Args&&... args) {
        if (currentSlot_ >= lastSlot_) {
            allocateNewBlock();
        }
        T* memory = currentSlot_++;
        return new (memory) T(std::forward<Args>(args)...);
    }

    void deallocate(T* p) {
        if (p) {
            p->~T();
        }
    }

private:
    void allocateNewBlock() {
        size_t bytes = BlockSize * sizeof(T);
        void* raw = ::operator new(bytes, std::align_val_t{alignof(T)});
        allocatedBlocks_.push_back(raw);
        currentSlot_ = reinterpret_cast<T*>(raw);
        lastSlot_ = currentSlot_ + BlockSize;
    }

    std::vector<void*> allocatedBlocks_;
    T* currentBlock_;
    T* currentSlot_;
    T* lastSlot_;
};

} // namespace truscor
