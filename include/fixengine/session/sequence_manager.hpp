#pragma once

namespace fix::session {
    class SequenceManager {
        int sequence_number_ = 0;

    public:
        [[nodiscard]] int get() const { return sequence_number_; }

        int increment() { return ++sequence_number_; }
    };
}
