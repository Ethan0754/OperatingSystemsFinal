#include <iostream>
#include <stdexcept>
#include <utility>

class LinkedList {
    private:
        struct Node {
            int data;
            int age; // For use in FIFO algorithm
            Node* next;
        };
        Node* head;

    public:
        // Default constructor
        LinkedList() : head(nullptr) {};

        // Copy constructor: performs deep copy of nodes
        LinkedList(const LinkedList& other) : head(nullptr) {
            Node* currentOther = other.head; // Pointer to traverse the other list
            Node** lastPtr = &head; // Pointer to the last node's next pointer

            while (currentOther) { // While there are still nodes to check in the other list

                Node* newNode = new Node{currentOther->data, currentOther->age, nullptr};
                *lastPtr = newNode; // Link the new node to the last node's next pointer
                lastPtr = &newNode->next; // Move lastPtr to the new node's next pointer

                currentOther = currentOther->next; 
            }
        }
        
        //Descturor: cleans up all nodes to prevent memory leaks
        ~LinkedList() {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }

        void display() const {
            """ Display the data in the list, replacing -1 with 0 - Used to display the data in columns, one page at a time""";
            Node* current = head;

            // Traverse the list and display each node's data, replacing -1 with 0
            while (current) { // While there are still nodes to check
                if (current->data == -1) { //if frame is blank, display 0
                    std::cout << "0" << std::endl;
                } 
                else {
                std::cout << current->data << std::endl;
                }

                current = current->next; // Move to the next node
            }
        }

        void displayByIndex(int index) const {
            """ Display the data at a specific index, replacing -1 with 0 - Used to display the data in rows instead of columns""";
            Node* current = head;

            int count = 0;

            // Traverse the list until the specified index is reached
            while (current) { // While there are still nodes to check
                if (count == index) { 
                    if (current->data == -1) { //if frame is blank, display 0
                        std::cout << "0" << "   ";
                    } 
                    else {
                        std::cout << current->data << "   "; //Display the data with spacing at the specified index
                    }
                    return;
                }
                current = current->next;
                count++;
            }
        }


        int search(int value) const {
            """ Search for a value in the list and return its index, or -1 if not found """;
            Node* current = head;

            int index = 0;
            while (current) { // While there are still nodes to check
                if (current->data == value) { // Value found
                    return index; // Return the index of the found value
                }
                current = current->next; // Move to the next node
                index++;
            }
            return -1;
        }

        bool contains(int value) const {
            """ Check if the list contains a specific value """;
            return search(value) != -1;
        }

        int get(int index) const {
            """ Get the value at a specific index """;
            Node* current = head;

            int count = 0;
            while (current) { // While there are still nodes to check
                if (count == index) {
                    return current->data; // Return the data at the specified index
                }
                current = current->next;
                count++;
            }
            return -1;
        }

        void insertInEmptyFrame(int value) {
            """ Insert a value into the first empty frame (-1) found in the list """;
            Node* current = head;

            while (current) { // While there are still nodes to check
                if (current->data == -1) { // Found an empty frame
                    current->data = value; // Insert the value into the empty frame
                    return;
                }
                current = current->next;
            }
        }

        void insert(int value) {
            """ Insert a new node with the given value at the head of the list """;
            Node* newNode = new Node{value, 0, head}; // Create a new node with the given value and age 0, insert at head
            head = newNode; // Update head to point to the new node
        }

        int size() const {
            """ Get the size of the linked list """;
            Node* current = head;

            int count = 0;
            while (current) { // While there are still nodes to check
                current = current->next;
                count++; // Increment count for each node
            }
            return count;
        }

        void increaseAges() {
            """ Increase the age of all nodes in the list by 1, except for nodes with data -1 """;
            Node* current = head;

            while (current) { // While there are still nodes to check
                if (current->data != -1) { // Only increase age for non-empty frames
                    current->age++;
                }
                current = current->next;
            }
        }

        void resetOneAge(int index) {
            """ Reset the age of the node at the specified index to 0 - This function is meant to handle the case where a page is already in a frame """;
            Node* current = head;

            int count = 0;
            while (current) { // While there are still nodes to check
                if (count == index) {
                    current->age = 0; // Reset age to 0 for the specified index
                    return;
                }
                current = current->next;
                count++;
            }
        }

        void replaceLRU(int value) {
            """ Replace the least recently used (oldest) node's data with the given value """;
            Node* current = head;
            Node* oldestNode = head;

            while (current) { // While there are still nodes to check
                if (current->age > oldestNode->age) { // Found an older node
                    oldestNode = current;
                }
                current = current->next;
            }
            oldestNode->data = value;
            oldestNode->age = 0; // Reset age after replacement
        }

        void reverse() {
            """ Reverse the linked list in place """;
            Node* prev = nullptr;
            Node* current = head;
            Node* next = nullptr;

            while (current) { // While there are still nodes to check
                next = current->next; // Store next node
                current->next = prev; // Reverse the link
                prev = current; // Move prev to current
                current = next; // Move to the next node
            }
            head = prev; // Update head to the new first node
        }


    };