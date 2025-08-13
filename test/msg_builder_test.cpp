#include <iostream>

#include <fixengine/protocol/msg_builder.hpp>
#include <fixengine/utils/sending_time.hpp>

int main() {
    fix::protocol::MsgBuilder msg{};
    //8=FIX.4.29=11835=D49=ONIXS56=CME34=252=20240528-09:20:52.11111=983532-321=138=10055=NVDA40=154=160=20240528-09:20:52.00410 = 017
    const auto m = msg
    .add(8, "FIX.4.2")
    .add(35, "D")
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
    std::cout << m << std::endl;
    return 0;
}
