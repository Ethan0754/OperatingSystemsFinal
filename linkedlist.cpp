#include <iostream>

class LinkedList {
    private:
        struct Node {
            int data;
            Node* next;
        };
        Node* head;

    public:
        LinkedList() : head(nullptr) {};
        void insert(int value) {
            Node* newNode = new Node{value, head};
            head = newNode;
        }

        void display() const {
            Node* current = head;
            while (current) {
                std::cout << current->data << std::endl;
                current = current->next;
            }
            std::cout << "nullptr" << std::endl;
        }

        int search(int value) const {
            Node* current = head;
            int index = 0;
            while (current) {
                if (current->data == value) {
                    return index;
                }
                current = current->next;
                index++;
            }
            return -1;
        }

        ~LinkedList() {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    };