//Authors: Ethan Pendergraft, Cosmo House    
    
    #include <iostream>
    #include <fstream>
    #include <string>
    #include "linkedlist.cpp"
    #include "outerlist.cpp"

    int algorithmLRU(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames);
    int algorithmOPT(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames);

    int main() { 
        //Open a file
        std::string FileName = "";
        std::cout << "Enter the file name ";
        std::cin >> FileName;
        std::fstream File(FileName, std::ios::in);
        
        
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

        int frameFaults = 0; //Variable to store the number of page faults
        switch (algorithm) {
            case 'F': {
                std::cout << "Using FIFO Page Replacement Algorithm" << std::endl;
                break;
            }
            case 'L': {
                std::cout << "Using LRU Page Replacement Algorithm" << std::endl;
                frameFaults = algorithmLRU(numberList, innerList, outerList, blankList, numFrames);
                std::cout << "Total Page Faults: " << frameFaults << std::endl;
                break;
            }

            case 'O': {
                std::cout << "Using Optimal Page Replacement Algorithm" << std::endl;
                frameFaults = algorithmOPT(numberList, innerList, outerList, blankList, numFrames);
                std::cout << "Total Page Faults: " << frameFaults << std::endl;
                break;
            }

            default:
                std::cerr << "Unknown algorithm!" << std::endl;
                return 1;
            }
        };


 int algorithmLRU(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames) {
    int frameFaults = 0; //Count of page faults
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
            frameFaults++; //Increment page fault count
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
    return frameFaults;
 }


 int algorithmOPT(LinkedList& numberList, LinkedList& innerList, OuterList& outerList, LinkedList& blankList, int numFrames) {

     int frameFaults = 0; // Total OPT faults

     // Go through each page reference in order
     for (int currentIndex = 0; currentIndex < numberList.size(); currentIndex++) {

         int currentPage = numberList.get(currentIndex); // Page being referenced now

         // HIT: page already in frames -> no fault
         if (innerList.contains(currentPage)) {
             // blank
             outerList.insert(blankList, currentPage);
             continue;
         }

         // MISS: page not in frames -> fault
         frameFaults++;

         // If we have an empty frame, use it
         if (innerList.search(-1) != -1) {
             innerList.insertInEmptyFrame(currentPage);
         }
         // Otherwise, choose a victim using OPT
         else {
             int victimIndex = 0;      // Frame index to replace
             int farthestNextUse = -1; // The largest "next use" position so far

             // Check each frame�s page
             for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {

                 int pageInFrame = innerList.get(frameIndex);
                 int nextUseIndex = -1; // -1 means "never used again"

                 // Look ahead to find this page�s next use
                 for (int futureIndex = currentIndex + 1; futureIndex < numberList.size(); futureIndex++) {
                     if (numberList.get(futureIndex) == pageInFrame) {
                         nextUseIndex = futureIndex;
                         break;
                     }
                 }

                 // Best possible victim: never used again
                 if (nextUseIndex == -1) {
                     victimIndex = frameIndex;
                     farthestNextUse = 2147483647; // Treat as �infinite future�
                     break;
                 }

                 // Otherwise, pick the one used farthest in the future
                 if (nextUseIndex > farthestNextUse) {
                     farthestNextUse = nextUseIndex;
                     victimIndex = frameIndex;
                 }
             }

             // Replace the victim frame with the current page
             innerList.replaceAtIndex(victimIndex, currentPage);
         }

         // Record a snapshot AFTER the fault is handled (so the table shows the new frames)
         LinkedList tempList = innerList;
         outerList.insert(tempList, currentPage);
     }

     // Print oldest-to-newest in correct order (same pattern as your LRU code)
     outerList.reverse();
     outerList.display(numFrames);

     return frameFaults;
 }

    