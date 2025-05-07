#ifndef REGISTER_H
#define REGISTER_H

class Register_File {
    private:
        int registers[8];
    
    public:
        Register_File();

        int Get_Register_Value(int index);

        void Set_Register_Value(int index, int value);
};

#endif