//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <map>
#include <memory>

#include "boost/di.hpp"

//#define LOG(STREAM) std::cout << "DEBUG " << STREAM << std::endl;

#ifndef LOG
#  define LOG(STREAM)
#endif
BOOST_DI_NAMESPACE_BEGIN
namespace extension {

struct item_t
{
  std::string name;
  int max_childs;
};

using items_t = std::vector<item_t>;

struct node_t;

using node_ptr_t = std::shared_ptr<node_t>;
using nodes_t = std::vector<node_ptr_t>;

struct node_t
{
  std::string name;
  int max_childs;
  nodes_t childs;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

items_t items;

///////////////////

auto print_tree_impl(const node_ptr_t& root, std::map<std::string, bool>& visited) -> void
{
  if(visited.find(root->name) != visited.cend())
  {
    return;
  }

  visited[root->name] = true;
  for(const auto& c : root->childs)
  {
    std::cout << "\"" << root->name << "\" ..> \"" << c->name << "\"" << std::endl;
  }

  for(const auto& c : root->childs)
  {
    print_tree_impl(c, visited);
  }
}

auto print_tree(const node_ptr_t& root) -> void
{
  std::map<std::string, bool> visited;
  print_tree_impl(root, visited);
}

auto tree_find(node_ptr_t root, const std::string& name) -> node_ptr_t
{
  if(root->name == name)
  {
    return root;
  }
  for(auto c : root->childs)
  {
    auto found = tree_find(c, name);
    if(found != nullptr)
    {
      return found;
    }
  }
  return nullptr;
}

auto make_tree_impl(const items_t& items, int& idx, node_ptr_t root, node_ptr_t node) -> void
{
  if(items.empty() || idx >= (int)items.size()) return;

  node->name = items[idx].name;
  node->max_childs = items[idx].max_childs;

  LOG("make_tree_impl() idx: " << idx << " name: " << node->name)

  for(int i = 0; i < node->max_childs; i++)
  {
    idx++;
    auto child_node_name = items[idx].name;
    auto existing_child = tree_find(root, child_node_name);

    LOG("make_tree_impl() node->name " << node->name << " child: idx: " << idx << " name: " << child_node_name << "  exists: " << ((existing_child != nullptr) ? "true" : "false"))
    if(existing_child != nullptr )
    {
      node->childs.push_back(existing_child);
    }
    else
    {
      auto child = std::make_shared<node_t>();
      node->childs.push_back(child);
      make_tree_impl(items, idx, root, child);
    }
  }
  if(node->max_childs == 0)
  {
    LOG("make_tree_impl() no childs")
  }
}

auto make_tree(const items_t& items) -> node_ptr_t
{
  auto root = std::make_shared<node_t>();
  int idx = 0;

  make_tree_impl(items, idx, root, root);

  return root;
}




class uml_dumper_2 : public config {
 public:

  ~uml_dumper_2()
  {
    auto tree = make_tree(items);
    print_tree(tree);
  }

  static auto policies(...) noexcept {

    return make_policies([&](auto type) {
      using T = decltype(type);
      using given = typename T::given;

      item_t item;
      item.name = std::string{typeid(given).name()};
      item.max_childs = T::arity::value;

      //LOG("make_policies() name " << item.name << " childs " << item.max_childs)
      items.push_back(item);
    });
  }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
