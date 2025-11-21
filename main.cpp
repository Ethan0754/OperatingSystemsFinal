//Authors: Ethan Pendergraft, Cosmo House    
    
    #include <iostream>
    #include <fstream>
    #include <string>
    #include "linkedlist.cpp"
    #include "outerlist.cpp"

    void algorithmLRU(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames);

    int main() { 
        //Open a file
        std::fstream File("LRU.txt", std::ios::in);
        
        
        //If the file fails to open, print an error and exit
        if(!File.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return 1;
        }
        
        //Pull the algorithm letter and number of frames from the first two characters of the file
        char algorithm; int numFrames; char temp;
        File >> algorithm >> temp >> numFrames; //Assign algorithm and numFrames, ignore comma
        std::cout << "Algorithm: " << algorithm << ", Number of Frames: " << numFrames << std::endl; 
        
        
        char character; //Temporary character for reading file
        int frameNum; //Temporary integer for storing frame number - to be inserted into linked list
        LinkedList numberList; //Linked list to store the numbers from the file
        std::string frameString = ""; //String to build the number from characters

        //Get the string of numbers in between commas, and insert them into the linked list
        while (File >> character) {

            if (character == ',') { //If we hit a comma, start building the number string
                File.get(character); //Get the next character after the comma

                while(character != ',' && !File.eof()) { //While not at the next comma or end of file, build the number string
                    frameString += character; //Add character to number string
                    File.get(character);
                }
                File.unget(); //Put back the comma for the next iteration

                //Convert the number string to an integer and insert it into the linked list
                frameNum = std::stoi(frameString);
                frameString = "";
                numberList.insert(frameNum);
            }
            
        }

        numberList.reverse(); //Reverse the list to maintain original order

        //Create a blank linked list to use for inserting into the outer list (page full, but no page fault)
        LinkedList blankList;
        for (int i = 0; i < numFrames; i++) {
            blankList.insert(-1);
        }

        OuterList outerList; //Outer list to store the inner lists for display later
        LinkedList innerList; //Inner list to store the current state of frames

        for(int i = 0; i < numFrames; i++) { //Initialize inner list with blank frames (-1)
            innerList.insert(-1);
        }

        switch (algorithm) {
            case 'F': {
                std::cout << "Using FIFO Page Replacement Algorithm" << std::endl;
                break;
            }
            case 'L': {
                std::cout << "Using LRU Page Replacement Algorithm" << std::endl;
                algorithmLRU(numberList, innerList, outerList, blankList, numFrames);
                break;
            }

            case 'O': {
                std::cout << "Using Optimal Page Replacement Algorithm" << std::endl;
                break;
            }

            default:
                std::cerr << "Unknown algorithm!" << std::endl;
                return 1;
            }
        };


 void algorithmLRU(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames) {
    //Iterate through the numbers in the number list and simulate the LRU algorithm
    for(int i = 0; i < numberList.size(); i++) {
        int currentNumber = numberList.get(i);
        if(!innerList.contains(currentNumber)) { //If the current number is not in the frames
            if(innerList.search(-1) != -1) { //If there is an empty frame

                innerList.increaseAges();
                innerList.insertInEmptyFrame(currentNumber);

            } else { //If there are no empty frames, replace the least recently used frame  
                
                innerList.increaseAges();
                innerList.replaceLRU(currentNumber); //Uses the age to determine LRU

            }
        }
        else { //If the current number is already in the frames, reset its age and skip iteration

            innerList.increaseAges();
            innerList.resetOneAge(innerList.search(currentNumber)); //Reset age of number already in frame to 0
            outerList.insert(blankList, currentNumber); //Insert blank list for no page fault
            continue;

        }

        LinkedList tempList = innerList; //Create a copy of the current inner list to insert into the outer list
        outerList.insert(tempList, currentNumber);

    }

    outerList.reverse(); //Reverse the outer list to maintain original order
    outerList.display(numFrames); 
 }
    