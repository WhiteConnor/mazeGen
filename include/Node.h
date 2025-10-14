#ifndef NODE_H
#define NODE_H

#include <random>
#include "ThreeCoord.h"

class Node
{
private:
  ThreeCoord<int> position;
  Node *next;

public:
  Node(ThreeCoord<int> pos);
  ~Node();
  const static void print(Node *head);
  ThreeCoord<int> getPosition() const;
  void setPosition(ThreeCoord<int> pos);
  Node *getNext() const { return next; }
  void setNext(Node *n) { next = n; }
};

#endif