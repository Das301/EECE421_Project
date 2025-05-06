#include "Issue_Unit.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Issue_Unit::Issue_Unit(std::string name)
{
    file_name = name;
    instructions_over = false;
    instruction_counter = 1;
}

void Issue_Unit::Reset_program(std::string name) {
    if(name!=""){
        file_name = name;
    }
    instructions_over = false;
    instruction_counter = 1;
}

void Issue_Unit::Fetch_Decode_Instruction() {
    string Instruction;
    int count = 1;
    bool done = true;

    ifstream MyCode(file_name);
    while (getline(MyCode, Instruction)) {
        if (count==instruction_counter) {
            instruction_counter++;
            cout << Instruction << endl;
            done = false;
            break;
        }
        count++;
    }
    MyCode.close();

    if(done) {
        cout << "Issued all instructions." << endl;
        instructions_over = true;
    }

}


void Issue_Unit::Current_Program_Info() 
{
    cout << file_name << endl;
    cout << instructions_over << endl;
    cout << instruction_counter << endl;
}