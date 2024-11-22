#include <atomic>
#include <memory>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;

        Node() : next(nullptr) {}
        Node(T value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue() {
        Node* dummy = new Node();
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue() {
        while (Node* node = head.load()) {
            head.store(node->next.load());
            delete node;
        }
    }

    void enqueue(T value) {
        Node* newNode = new Node(value);
        Node* oldTail = nullptr;

        while (true) {
            oldTail = tail.load();
            Node* next = oldTail->next.load();

            if (oldTail == tail.load()) {
                if (next == nullptr) {
                    if (oldTail->next.compare_exchange_weak(next, newNode)) {
                        tail.compare_exchange_weak(oldTail, newNode);
                        return;
                    }
                }
                else {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
    }

    bool dequeue(T& result) {
        Node* oldHead = nullptr;

        while (true) {
            oldHead = head.load();
            Node* oldTail = tail.load();
            Node* next = oldHead->next.load();

            if (oldHead == head.load()) {
                if (oldHead == oldTail) {
                    if (next == nullptr) {
                        return false; // Queue is empty
                    }
                    tail.compare_exchange_weak(oldTail, next);
                }
                else {
                    if (head.compare_exchange_weak(oldHead, next)) {
                        result = *(next->data);
                        delete oldHead;
                        return true;
                    }
                }
            }
        }
    }
};