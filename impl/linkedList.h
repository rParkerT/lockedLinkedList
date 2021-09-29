#pragma once

#include <iostream>
#include <mutex>
#include <thread>

namespace list {

/**
 * Implementation of the ordered locked Linked List class.
 *
 * Linked List is a linear collection of data elements in which every element
 * points to the next element. Concurrency is implemented using mutex on each
 * node. Each operation holds only the locks on the nodes it’s actually
 * interested in and unlocks each node as it moves on to the next.
 *
 * @tparam V type of data stored in Linked List
 */
template <typename V> class LinkedList {
private:
  /**
   * Implementation of the Node structure.
   *
   * Each Node carries a key and a pointer to next Node.
   *
   * @tparam V data type of key in Node
   */
  struct Node {
    std::mutex m; ///< mutex for Node
    V value;      ///< key value of Node
    Node *next;   ///< pointer to next Node in Linked List

    /**
     * Node constructor.
     * Each Node is constructed using key value and pointer to next Node.
     *
     * @tparam T data type of key in Node
     */
    explicit Node(V v) : value(v), next(nullptr) {}
  };

  /**
   * Head Node's value is the smallest value of each Node in Linked List.
   */
  Node *head = nullptr;

  /**
   * Adds Node to Linked List
   *
   * @tparam V data type of key in Node
   *
   * @return Node with set value
   */
  Node *addNode(V value) { return new Node(value); }

public:
  /**
   * Constructor of Linked List
   *
   * Constructor takes no arguments. During Linked List object construction,
   * head Node is created.
   *
   */
  LinkedList();

  /**
   * Destructor  of Linked List
   *
   * During Linked List destruction, all elements of Linked List are deleted.
   *
   */
  ~LinkedList();

  /// Disabling construction of Linked List object using copy constructor
  LinkedList(const LinkedList &rhs) = delete;

  /// Disabling construction of Linked List object using copy assignment
  LinkedList &operator=(const LinkedList &rhs) = delete;

  /**
   * Insert Node to Linked List
   *
   * Nodes are inserted in order, from the lowest to the highest key value. All
   * links for Node that's to be inserted are fetched. Pointer of predecessor
   * and new Node, and new Node and next Node is connected. First, head node
   * mutex is locked (lk), if the pointer to the next node is not nullptr, the
   * mutex on that node is locked (next_lk) in order to process the data. Once
   * you have the lock on that node, you can release the lock on the previous
   * node. If the node's value is smaller then newValue, next node is fetched
   * and the ownership of the lock from next_lk out to lk is moved.
   *
   * @tparam V newValue key value of Node
   *
   */
  void insertNode(V newValue);

  /**
   * Removes Node to Linked List
   *
   * Pointers of Node's predecessor is fetched. Pointers of
   * predecessor and successor of removed Node are connected. It is similar
   * procedure as insert if the value is not the same as tempNode value, if it
   * is, predecessor and successor of tempNode are connected, at which point it
   * is safe to release the lock held on the mutex for the next node (next_lk).
   *
   * @tparam V value key value of Node that it is to be deleted
   *
   */
  void eraseNode(V value);

  /**
   * Search Node in Linked List for given key value
   *
   * @tparam V value key value of Node
   *
   * @return true if Node with the same key value as value is
   * inserted in Linked List, else returns false
   */
  const bool searchNode(V value);
};

template <typename V> LinkedList<V>::LinkedList() {
  V valueMin = std::numeric_limits<V>::min();
  head = new Node(valueMin);
}

template <typename V> LinkedList<V>::~LinkedList() {
  Node *current = head;
  while (current) {
    head = current->next;
    delete current;
    current = head;
  }
}

template <typename V> void LinkedList<V>::insertNode(V newValue) {
  Node *tempNode, *newNode;
  newNode = addNode(newValue);
  tempNode = head;
  std::unique_lock<std::mutex> lk(tempNode->m);
  while (tempNode->next != nullptr && tempNode->next->value < newValue) {
    std::unique_lock<std::mutex> next_lk(tempNode->next->m);
    lk.unlock();
    tempNode = tempNode->next;
    lk = std::move(next_lk);
  }
  newNode->next = tempNode->next;
  tempNode->next = newNode;
}

template <typename V> void LinkedList<V>::eraseNode(V newValue) {
  Node *tempNode, *prevNode;
  prevNode = head;
  if (prevNode->next == nullptr)
    return;
  tempNode = prevNode->next;
  std::unique_lock<std::mutex> lk(prevNode->m);
  while (tempNode != nullptr && tempNode->value <= newValue) {
    std::unique_lock<std::mutex> next_lk(tempNode->m);
    if (tempNode->value == newValue) {
      prevNode->next = tempNode->next;
      next_lk.unlock();
      delete tempNode;
      return;
    }
    lk.unlock();
    prevNode = tempNode;
    tempNode = tempNode->next;
    lk = std::move(next_lk);
  }
}

template <typename V> const bool LinkedList<V>::searchNode(V value) {
  std::unique_lock<std::mutex> lk(head->m);
  if (head->next == nullptr)
    return false; // empyy list, except for head node
  Node *tempNode = head->next;
  while (tempNode != nullptr && tempNode->value <= value) {
    std::unique_lock<std::mutex> next_lk(tempNode->m);
    lk.unlock();
    if (tempNode->value == value) {
      return true;
    }
    tempNode = tempNode->next;
    lk = std::move(next_lk);
  }
  return false;
}

} // namespace list
