#ifndef REGISTER_H
#define REGISTER_H
#include <iostream>

using namespace std;

class Register_File {
    private:
        int registers[8];
    
    public:
        Register_File();

        int Get_Register_Value(int index);

        void Set_Register_Value(int index, int value);

        friend ostream& operator<<(ostream& os, const Register_File& obj);
};

#endif