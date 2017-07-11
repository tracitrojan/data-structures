#include <stdexcept>
#include <iostream>
using std::cout;

template <typename E>
class Stack;


template <typename E>
class Node {
private:
    E data;
    Node<E>* next;
    friend class Stack<E>;
};


template <typename E>
class Stack {
private:
    Node<E>* head;
    int num_of_nodes;

    void set_to_index(Node<E>*& ptr, int index) {
        if (index < 0 || index > num_of_nodes)
            throw out_of_range("index out of range");

        current = head;
        for (; index > 0; index--)
            current = current->next;
    }

public:
    Stack() {
        head = nullptr;
        num_of_nodes = 0;
    }

    ~Stack() {
        while (!is_empty())
            pop();
    }

    bool is_empty() {
        return num_of_nodes == 0;
    }

    int length() {
        return num_of_nodes;
    }

    void push(const E& elem) {
        Node<E>* new_node = new Node<E>;
        new_node->data = elem;
        new_node->next = head;
        head = new_node;
        num_of_nodes++;
    }

    void pop() {
        if (is_empty())
            throw  length_error("stack is empty");

        Node<E>* temp = head;
        head = head->next;
        delete temp;
        num_of_nodes--;
    }

    E& top() {
        if (is_empty())
            throw length_error("stack is empty");

        return head->data;
    }

    void insert(const E& elem, int index) {
        if (index < 0 || index > num_of_nodes)
            throw out_of_range("index out of range");

        if (index == 0) {
            push(elem);
            return;
        }

        Node<E>* new_node = new Node<E>;
        Node<E>* current;
        set_to_index(current, index);

        new_node->next = current->next;
        current->next = new_node;
        num_of_nodes++;
    }

    void remove(int index) {
        if (is_empty())
            throw length_error("stack is empty");
        if (index < 0 || index > num_of_nodes)
            throw out_of_range("index out of range");

        if (index == 0) {
            pop(elem);
            return;
        }

        Node<E>* current;
        set_to_index(current, index);
        Node<E>* temp = current->next;

        current->next = temp->next;
        delete temp;
        num_of_nodes--;
    }

    E& retrieve(int index) {
        if (is_empty())
            throw length_error("stack is empty");
        if (index < 0 || index > num_of_nodes)
            throw out_of_range("index out of range");

        Node<E>* current;
        set_to_index(current, index);

        return current->data;
    }

    void print() {
        if (is_empty())
            throw length_error("stack is empty");

        Node<E>* current = head;
        while (current != nullptr)
            cout << current->data << " ";
    }

    void print(int index) {
        if (is_empty())
            throw length_error("stack is empty");
        if (index < 0 || index > num_of_nodes)
            throw out_of_range("index out of range");

        Node<E>* current;
        set_to_index(current, index);

        cout << current->data;
    }

    void reverse() {
        if (is_empty())
            throw length_error("stack is empty");

        Node<E>* current = head;
        Node<E>* next = current->next;
        Node<E>* prev = nullptr;

        while (current != nullptr) {
            current->next = prev;
            prev = current;
            current = next;
            next = next->next;
        }

        head = prev;
    }

    E& operator[](){
        return retrieve(index);
    }

};