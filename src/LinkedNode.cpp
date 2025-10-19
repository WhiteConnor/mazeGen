#include "LinkedNode.h"
#include <atomic>
#include "mazeUtils.h"

LinkedNode::LinkedNode(ThreeCoord<int> pos) {
  append(pos);
}

LinkedNode::~LinkedNode() {
  Node *current = head;
  while (current) {
    Node *next = current->getNext();
    delete current;
    current = next;
  }
}

void LinkedNode::removeAt(ThreeCoord<int> pos) {
  // Implement singly linked list removal
  Node *current = this->head;
  if (current->getPosition() == pos) {
      // head is node to remove
      this->head = current->getNext();
      delete current;
      nodeCount--;
      return;
  }
  while (current != nullptr) {
    if (current->getNext() != nullptr && current->getNext()->getPosition() == pos) {
      Node *toDelete = current->getNext();
      current->setNext(toDelete->getNext());
      delete toDelete;
      nodeCount--;
      return;
    }
    current = current->getNext();
  }
}

ThreeCoord<int> LinkedNode::getRandom(std::atomic<uint8_t> *maze, ThreeCoord<int> d, std::mt19937 &gen) {
  int choice = gen() % nodeCount;
  Node *current = head;
  bool chosen = true;
  do {
    for (int i = 0; i < choice; i++) {
      if (current->getNext() != nullptr) {
        current = current->getNext();
      } else {
        return current->getPosition(); 
      }
      if (!hasAvailable(maze, d, current->getPosition())) {
        if (current->tryLock()) {
          ThreeCoord<int> toRemove = current->getPosition();
          current = head;
          removeAt(toRemove);
          chosen = false;
        }
      }
    }
  } while (!chosen || current->getLocked() || !current->getPosition());
  return current->getPosition();
}

const int LinkedNode::getNodeCount() {
  return nodeCount;
}

void LinkedNode::append(ThreeCoord<int> pos)
{
  Node *newNode = new Node(pos);
  Node *next = head;
  head = newNode;
  newNode->setNext(next);
  nodeCount++;
}