#pragma once

namespace mcts {
template <class State, class F>
auto Tree::_getNodePosition(const Nodes<State>& nodes,
                            const F& function) -> NodesItr<State> {
  auto node_pos = nodes.cend();
  double max_value = -1.0;
  for(auto it = nodes.cbegin(); it != nodes.cend(); it++) {
    auto value = function(*it);
    if(value > max_value) {
      node_pos = it;
      max_value = value;
    }
  }
  assert(node_pos != nodes.cend());
  return node_pos;
}

template <class State>
auto Tree::_selectPlayoutNode(const Nodes<State>& leaf_nodes,
                              int total_count) -> NodesItr<State> {
  auto function = [total_count](Node<State>* node) {
    return node->getValueUct(total_count);
  };
  return Tree::_getNodePosition(leaf_nodes, function);
}

template <class State>
inline auto Tree::_isPlayoutLimit(const Node<State>* const node) -> bool {
  return node->count_ >= params::playout_limit;
}

template <class State>
auto Tree::_update(const Node<State>* const root_node,
                   Nodes<State>& leaf_nodes) -> bool {
  if(leaf_nodes.empty()) return false;
  auto node_pos = Tree::_selectPlayoutNode(leaf_nodes, root_node->count_);
  auto* node = *node_pos;
  node->playout();
  if(Tree::_isPlayoutLimit(node)) {
    node->expand();
    leaf_nodes.erase(node_pos);
    const auto& nodes = node->child_nodes_;
    leaf_nodes.insert(leaf_nodes.cend(), nodes.cbegin(), nodes.cend());
  }
  return true;
}

template <class State>
auto Tree::_selectBetterState(const Node<State>* const root_node) -> State {
  assert(!root_node->isLeafNode());
  auto function = [](Node<State>* node) {return node->getValue();};
  return (*Tree::_getNodePosition(root_node->child_nodes_, function))->state_;
}

template <class T, class State>
auto Tree::search(T&& state) -> State {
  if(state.isEnd()) return std::forward<State>(state);
  auto* root_node = new Node<State>(std::forward<State>(state));
  root_node->expand();
  auto leaf_nodes = root_node->child_nodes_;
  for(int i = 0; i < params::playout_number; i++) {
    if(!Tree::_update(root_node, leaf_nodes)) break;
  }
  auto better_state = Tree::_selectBetterState(root_node); 
  delete root_node;
  return std::move(better_state);
}
}
