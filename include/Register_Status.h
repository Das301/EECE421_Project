#ifndef REGISTER_STAT_H
#define REGISTER_STAT_H
#include <iostream>

using namespace std;

class Register_Status {
    private:
        int registers[8];
    
    public:
        Register_Status();

        int Get_Register_Result(int index);

        void Set_Register_Result(int index, int value);

        friend ostream& operator<<(ostream& os, const Register_Status& obj);
};

#endif