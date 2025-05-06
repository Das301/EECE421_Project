#include "Issue_Unit.h"
#include <string>
#include <iostream>

using namespace std;

Issue_Unit::Issue_Unit(std::string name)
{
    file_name = name;
    instructions_over = false;
    instruction_counter = 1;
}

void Issue_Unit::Current_Program_Info() 
{
    cout << file_name << endl;
    cout << instructions_over << endl;
    cout << instruction_counter << endl;
}