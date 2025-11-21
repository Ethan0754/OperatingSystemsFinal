    #include <iostream>
    #include <fstream>
    #include <string>
    #include "linkedlist.cpp"

    int main() { 
        //Open the three files
        std::fstream File("FIFO.txt", std::ios::in);
        
        
        //If one of the files fail to open, print an error and exit
        if(!File.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return 1;
        }
        
        //Pull the algorithm letter and number of frames from the first two characters of the file
        char algorithm; int numFrames; char temp;
        File >> algorithm >> temp >> numFrames; //Assign algorithm and numFrames, ignore comma
        std::cout << "Algorithm: " << algorithm << ", Number of Frames: " << numFrames << std::endl; 
        

        char character;
        int frameNum;
        LinkedList numberList;
        std::string frameString = "";

        //Get the string of numbers in between commas, and insert them into the linked list
        while (File >> character) {
            if (character == ',') {
                File.get(character); //Get the next character after the comma
                while(character != ',' && !File.eof()) { //While not at the next comma or end of file, build the number string
                    frameString += character;
                    File.get(character); 
                }
                File.unget(); //Put back the comma for the next iteration

                //Convert the number string to an integer and insert it into the linked list
                frameNum = std::stoi(frameString);
                frameString = "";
                numberList.insert(frameNum);
            }
            
        }


        numberList.display();

        
        LinkedList outerList;
        switch(algorithm) {
            case 'F':
                std::cout << "Using FIFO Page Replacement Algorithm" << std::endl;
                

                break;
            case 'L':
                std::cout << "Using LRU Page Replacement Algorithm" << std::endl;
                break;
            case 'O':
                std::cout << "Using Optimal Page Replacement Algorithm" << std::endl;
                break;
            default:
                std::cerr << "Unknown algorithm!" << std::endl;
                return 1;
        }
        



    }