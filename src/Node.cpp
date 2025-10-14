#include "Node.h"
#include "ThreeCoord.h"


Node::Node(ThreeCoord<int> pos) {
  position = pos;
  next = nullptr;
}

Node::~Node() {
}

const void Node::print(Node *head) {
  Node *current = head;
  while (current != nullptr) {
    printf("Node Position: (%d, %d, %d)\n", current->position.x, current->position.y, current->position.z);
    current = current->next;
  }
  if (head == nullptr) {
    printf("List is empty.\n");
  }
}


ThreeCoord<int> Node::getPosition() const {
  return position;
}

void Node::setPosition(ThreeCoord<int> pos) {
  position = pos;
}

bool Node::try_lock() {
  uint8_t expected = 0;
  return lock.compare_exchange_strong(expected, 1);
}

void Node::release_lock() {
  lock.store(0);
}