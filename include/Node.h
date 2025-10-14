#ifndef NODE_H
#define NODE_H

#include <random>
#include "ThreeCoord.h"
#include <atomic>

class Node
{
private:
  ThreeCoord<int> position;
  Node *next;
  std::atomic<uint8_t> lock{0};

public:
  Node(ThreeCoord<int> pos);
  ~Node();
  const static void print(Node *head);
  ThreeCoord<int> getPosition() const;
  void setPosition(ThreeCoord<int> pos);
  Node *getNext() const { return next; }
  void setNext(Node *n) { next = n; }
  bool try_lock();
  void release_lock();
};

#endif