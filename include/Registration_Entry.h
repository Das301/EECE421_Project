#ifndef ENTRY_H
#define ENTRY_H

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
    public:
        Registration_Entry(int ID);

        bool Check_Busy();
        bool Check_Available();
        bool Operands_Ready();

        void Set_Operation(int op_type);
        void Set_Operand_Source(int Q_index, int value);
        void Set_Operand(int V_index, int value);

        int Get_Operand_Source(int Q_index);

        void Get_Instruction_Complete(int (&instruct_details)[4]);
        void Reset_Entry();
};

#endif