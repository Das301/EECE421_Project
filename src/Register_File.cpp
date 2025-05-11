#include "Register_File.h"

Register_File::Register_File() {
    for (int i=0; i<8; i++){
        registers[i] = i;
    } 
}

int Register_File::Get_Register_Value(int index) {
    return registers[index];
}

void Register_File::Set_Register_Value(int index, int value) {
    registers[index] = value;
}

ostream& operator<<(ostream& os, const Register_File& obj) {
    os << "Index | Source station" << endl;
    for(int i=0; i<8; i++){
        os << i << "     |  " << obj.registers[i] << endl;
    }

    return os;
}
