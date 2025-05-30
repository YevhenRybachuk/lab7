#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T>
struct SinglyNode {
    T data;
    std::shared_ptr<SinglyNode<T>> next;

    SinglyNode(T value) : data(value), next(nullptr) {}
};

template <typename T>
struct DoublyNode {
    T data;
    std::shared_ptr<DoublyNode<T>> next;
    std::weak_ptr<DoublyNode<T>> prev;

    DoublyNode(T value) : data(value), next(nullptr), prev() {}
};

template <typename T>
class SinglyLinkedList {
private:
    std::shared_ptr<SinglyNode<T>> head;
    size_t size = 0;

public:
    bool isEmpty() const { return size == 0; }
    size_t getSize() const { return size; }

    void pushFront(T value) {
        auto newNode = std::make_shared<SinglyNode<T>>(value);
        newNode->next = head;
        head = newNode;
        ++size;
    }

    void pushBack(T value) {
        auto newNode = std::make_shared<SinglyNode<T>>(value);
        if (!head) {
            head = newNode;
        } else {
            auto current = head;
            while (current->next) current = current->next;
            current->next = newNode;
        }
        ++size;
    }

    void popFront() {
        if (isEmpty()) throw std::out_of_range("List is empty");
        head = head->next;
        --size;
    }

    void popBack() {
        if (isEmpty()) throw std::out_of_range("List is empty");
        if (size == 1) {
            head = nullptr;
        } else {
            auto current = head;
            while (current->next->next) current = current->next;
            current->next = nullptr;
        }
        --size;
    }

    T& at(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        auto current = head;
        for (size_t i = 0; i < index; ++i) current = current->next;
        return current->data;
    }

    void insert(size_t index, T value) {
        if (index > size) throw std::out_of_range("Index out of range");
        if (index == 0) {
            pushFront(value);
        } else {
            auto newNode = std::make_shared<SinglyNode<T>>(value);
            auto current = head;
            for (size_t i = 0; i < index - 1; ++i) current = current->next;
            newNode->next = current->next;
            current->next = newNode;
            ++size;
        }
    }

    void remove(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        if (index == 0) {
            popFront();
        } else {
            auto current = head;
            for (size_t i = 0; i < index - 1; ++i) current = current->next;
            current->next = current->next->next;
            --size;
        }
    }

    int find(const T& value) const {
        auto current = head;
        int index = 0;
        while (current) {
            if (current->data == value) return index;
            current = current->next;
            ++index;
        }
        return -1;
    }

    void print() const {
        auto current = head;
        while (current) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "null\n";
    }
};

template <typename T>
class DoublyLinkedList {
private:
    std::shared_ptr<DoublyNode<T>> head;
    std::shared_ptr<DoublyNode<T>> tail;
    size_t size = 0;

public:
    bool isEmpty() const { return size == 0; }
    size_t getSize() const { return size; }

    void pushFront(T value) {
        auto newNode = std::make_shared<DoublyNode<T>>(value);
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
        if (!tail) tail = newNode;
        ++size;
    }

    void pushBack(T value) {
        auto newNode = std::make_shared<DoublyNode<T>>(value);
        newNode->prev = tail;
        if (tail) tail->next = newNode;
        tail = newNode;
        if (!head) head = tail;
        ++size;
    }

    void popFront() {
        if (isEmpty()) throw std::out_of_range("List is empty");
        head = head->next;
        if (head) head->prev.reset();
        else tail = nullptr;
        --size;
    }

    void popBack() {
        if (isEmpty()) throw std::out_of_range("List is empty");
        tail = tail->prev.lock();
        if (tail) tail->next = nullptr;
        else head = nullptr;
        --size;
    }

    T& at(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        auto current = head;
        for (size_t i = 0; i < index; ++i) current = current->next;
        return current->data;
    }

    void insert(size_t index, T value) {
        if (index > size) throw std::out_of_range("Index out of range");
        if (index == 0) {
            pushFront(value);
        } else if (index == size) {
            pushBack(value);
        } else {
            auto newNode = std::make_shared<DoublyNode<T>>(value);
            auto current = head;
            for (size_t i = 0; i < index; ++i) current = current->next;
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev.lock()->next = newNode;
            current->prev = newNode;
            ++size;
        }
    }

    void remove(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        if (index == 0) {
            popFront();
        } else if (index == size - 1) {
            popBack();
        } else {
            auto current = head;
            for (size_t i = 0; i < index; ++i) current = current->next;
            current->prev.lock()->next = current->next;
            current->next->prev = current->prev;
            --size;
        }
    }

    int find(const T& value) const {
        auto current = head;
        int index = 0;
        while (current) {
            if (current->data == value) return index;
            current = current->next;
            ++index;
        }
        return -1;
    }

    void print() const {
        auto current = head;
        while (current) {
            std::cout << current->data << " <-> ";
            current = current->next;
        }
        std::cout << "null\n";
    }
};

int main() {
    SinglyLinkedList<int> sList;
    sList.pushBack(1);
    sList.pushBack(2);
    sList.pushBack(3);
    sList.print();

    DoublyLinkedList<std::string> dList;
    dList.pushFront("World");
    dList.pushFront("Hello");
    dList.insert(1, "Goodbye");
    dList.print();
    return 0;
}

