#pragma once

namespace mcts {
namespace params {
inline auto set(int playout_number, int playout_limit, double evaluation_param)
  -> void {
  params::playout_number = playout_number;
  params::playout_limit = playout_limit;
  params::evaluation_param = evaluation_param;
}
}
}
