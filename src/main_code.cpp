#include "Issue_Unit.h"
#include "Register_File.h"
#include "Register_Status.h"
#include "Memory_Unit.h"
#include "Functional_Unit.h"
#include "Registration_Entry.h"
#include "Registration_Station.h"
#include "Processor.h"
#include "iostream"
#include <string>

using namespace std;

bool file_checking(std::string test_name) {
    int str_len = test_name.length();
    if(str_len <= 4){
        return false;
    }
    
    string temp = "";
    for(int i=str_len-4; i<str_len; i++){
        temp = temp + test_name[i];
    }
    if(temp==".txt") {
        return true;
    }
    return false;
}

int main() {
    cout << "Welcome to the Tomasulo's Algorithm Simulator" << endl;
    cout << "Please enter the name of your program file (must be .txt) or QUIT to leave the simulator: ";
    bool simulation_run = true;
    
    while(simulation_run) {
        string file_name;
        cin >> file_name;
        
        if(file_name=="QUIT") {
            simulation_run = false;
            break;
        } else{
            if(file_checking(file_name)){
                Processor CPU(file_name);
                CPU.Run_Simulation();
                CPU.Print_Execution_Diagnostics();

                cout << "\nEnter the name of the next program file (must be .txt) or QUIT to leave the simulator: ";
            } else {
                cout << "\nInvalid file type! Please enter a valid .txt file or QUIT to leave: " << endl;
            }
        } 
    }

    cout << "Thank you for using Dany's simulator for the Tomasulo's Algorithm." << endl;
    return 0;
}