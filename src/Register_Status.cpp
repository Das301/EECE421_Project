#include "Register_Status.h"
#include <iostream>

using namespace std;

Register_Status::Register_Status() {
    for (int i=0; i<8; i++){
        registers[i] = 0;
    } 
}

int Register_Status::Get_Register_Result(int index) {
    return registers[index];
}

void Register_Status::Set_Register_Result(int index, int value) {
    registers[index] = value;
}

ostream& operator<<(ostream& os, const Register_Status& obj) {
    os << "Index | Source station" << endl;
    for(int i=0; i<8; i++){
        os << i << "     |  " << obj.registers[i] << endl;
    }

    return os;
}