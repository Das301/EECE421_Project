#ifndef FUNC_H
#define FUNC_H

#include "Memory_Unit.h"

class Functional_Unit {
    private:
        int type;
        int curr_cycle;

        //clock cycles of each operation
        int op1_cycles;
        int op2_cycles;

        //values of the operands passed to the functional unit
        int operand1;
        int operand2;

        //value to store in case of STORE operation, otherwise, source reservation station number
        int operand3;

        //either first or second operation offered by the unit
        bool operation;

        //unit and output availability variables
        bool busy;
        bool output_ready;
    public:
        Functional_Unit(int type, int op1_cycles, int op2_cycles);

        void Set_Operation(int operand1, int operand2, int operand3, bool operation);

        void Update_Operation_Status();

        bool Check_Available();
        bool Check_Output();
        bool Check_Busy();

        void Return_Result(int (&arr_result)[2], Memory_Unit* memory=nullptr);

        void Reset_Unit();
};

#endif