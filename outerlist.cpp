#include <iostream>

class OuterList {
    private:
        struct Node {
            LinkedList innerList;
            int headerName;
            Node* next;
        };
        Node* head;
    public:
        // Default constructor
        OuterList() : head(nullptr) {};

        //Destructor: cleans up all nodes to prevent memory leaks
        ~OuterList() {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
        
        void insert(const LinkedList& list, int headerName) {
            """ Insert a new inner list with the given header name at the front of the outer list """;
            Node* newNode = new Node{list, headerName, head};
            head = newNode;
        }

        void display(int numFrames) const {
            Node* current = head;
            
            while (current) { //While there are still nodes to check
                std::cout << current->headerName << "   "; //Display the header names
                current = current->next;
            }

            std::cout << std::endl;
    
            current = head;
            while (current) { //While there are still nodes to check
                std::cout << "__" << "  "; //Display underscores for formatting
                current = current->next;
            }
            
            std::cout << std::endl;

            int index = 0;
            while (index < numFrames) { //Iterate through each frame index - enables row-wise display
                current = head;

                while (current) { //While there are still nodes to check
                    current->innerList.displayByIndex(index); //Display the data at the specified index
                    current = current->next;
                }
                index++;
                std::cout << std::endl; //New line after end of pages
            }
        }

        void reverse() {
            """ Reverse the linked list in place """;
            Node* prev = nullptr;
            Node* current = head;
            Node* next = nullptr;

            while (current) { //While there are still nodes to check
                next = current->next; //Store next node
                current->next = prev; //Reverse the link
                prev = current; //Move prev to current
                current = next; //Move to the next node
            }
            head = prev;
        }
};
