#ifndef REGISTER_STAT_H
#define REGISTER_STAT_H

class Register_Status {
    private:
        int registers[8];
    
    public:
        Register_Status();

        int Get_Register_Result(int index);

        void Set_Register_Result(int index, int value);
};

#endif