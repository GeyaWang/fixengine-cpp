#pragma once
#include <cstdint>

namespace fix::session {
    class SequenceManager {
        uint32_t sequence_number_ = 1;

    public:
        uint32_t get() const { return sequence_number_; }
        void increment() { sequence_number_++; }
    };
}
