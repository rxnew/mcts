#pragma once

namespace mcts {
namespace params {
extern int playout_number;
extern int playout_limit;
extern double exploration_param;

auto set(int playout_number,
         int playout_limit,
         double exploration_param) -> void;
}
}

#include "impl/params_impl.hpp"
