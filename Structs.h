#include <memory>
#include <string>

#pragma once

struct Complex {
    float I;
    float Q;
};

struct BufferSlot {
    std::unique_ptr<Complex[]> data;
    size_t count;
    BufferSlot(size_t n) : data(std::make_unique<Complex[]>(n)), count(0) {}
    BufferSlot(const BufferSlot&) = delete;
    BufferSlot& operator=(const BufferSlot&) = delete;
    BufferSlot(BufferSlot&&) = default;
    BufferSlot& operator=(BufferSlot&&) = default;
};

struct Band {
    std::string name = "";
    int low = 0;
    int high = 0;
};