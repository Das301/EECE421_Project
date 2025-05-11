#ifndef ENTRY_H
#define ENTRY_H
#include <iostream>

using namespace std;

class Registration_Entry {
    private:
        int station_ID;

        int op = 0;
        int V1 = 0;
        int V2 = 0;
        int Q1 = 0;
        int Q2 = 0;

        //only for store operations
        int V_store = 0;
        int Q_store = 0;

        bool busy = false;
        bool sent_to_unit = false;
        int occupied_time = 0;
        int address_age = 0;

    public:
        Registration_Entry(int ID);

        int Return_ID();

        bool Check_Busy();
        bool Check_Available();
        bool Operands_Ready();
        bool Check_Sent_To_Unit();
        void Set_Sent_To_Unit();

        void Set_Operation(int op_type);
        void Set_Operand_Source(int Q_index, int value);
        void Set_Operand(int V_index, int value);

        int Get_Operand_Source(int Q_index);

        void Increment_Occupied_Time();
        int Get_Occupied_Time();

        void Increment_Address_Age();
        int Get_Address_Age();

        void Get_Instruction_Complete(int (&instruct_details)[4]);
        void Reset_Entry();

        friend ostream& operator<<(ostream& os, const Registration_Entry& obj);
};

#endif