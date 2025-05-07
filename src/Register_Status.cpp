#include "Register_Status.h"

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