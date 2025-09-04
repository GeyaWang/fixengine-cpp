#include <iostream>

#include <fixengine/protocol/message.hpp>
#include <fixengine/utils/time.hpp>

int main() {
    fix::protocol::Message msg{"FIX.4.2", "D"};
    //8=FIX.4.29=11835=D49=ONIXS56=CME34=252=20240528-09:20:52.11111=983532-321=138=10055=NVDA40=154=160=20240528-09:20:52.00410 = 017
    const auto m = msg
        .add(49, "ONIXS")
        .add(56, "CME")
        .add(34, 2)
        .add(52, "20240528-09:20:52.111")
        .add(11, "983532-3")
        .add(21, 1)
        .add(38, 100)
        .add(55, "NVDA")
        .add(40, 1)
        .add(54, 1)
        .add(60, "20240528-09:20:52.004")
        .build();
    std::cout << m << "\n";

    fix::protocol::Message msg2{m};
    std::cout << msg2.build() << "\n";

    return 0;
}
