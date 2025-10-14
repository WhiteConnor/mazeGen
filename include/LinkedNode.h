#ifndef LINKEDNODE_H
#define LINKEDNODE_H

#include <random>
#include "ThreeCoord.h"
#include "Node.h"

class LinkedNode {
private:
  Node *head = nullptr;
  int nodeCount = 0;

public:
  LinkedNode(ThreeCoord<int> pos);
  ~LinkedNode();

  void removeAt(ThreeCoord<int> pos);
  ThreeCoord<int> getRandom(std::mt19937 &gen);
  const int getNodeCount();
  void append(ThreeCoord<int> pos);
};

#endif