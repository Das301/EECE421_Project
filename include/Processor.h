#ifndef PROC_H
#define PROC_H
#include "Issue_Unit.h"
#include "Register_File.h"
#include "Register_Status.h"
#include "Memory_Unit.h"
#include "Functional_Unit.h"
#include "Registration_Entry.h"
#include "Registration_Station.h"
#include <string>

class Processor {
    private:
        Issue_Unit unit;
        Register_File reg_unit;
        Register_Status reg_stat_unit;
        Memory_Unit Memory;

        Functional_Unit ALU1;
        Functional_Unit ALU2;
        Functional_Unit MUL_DIV;
        Functional_Unit LOAD_STORE;

        int entry_id = 1;

        Registration_Station ALUs;
        Registration_Station MUL_DIV_STAT;
        Registration_Station LOAD_STAT;
        Registration_Station STORE_STAT;
        
        bool execution_complete = false;
        bool instruction_issue_complete = false;
        bool instruction_issue_stall = false;
        bool instruction_error = false;
        int instruct_issue[4] = {-1, -1, -1, -1};
        
        bool CDB_Available = true;
        int instruct[4] = {-1, -1, -1, -1};
        int result[2] = {-1, -1};

        int store_station_ID=-1;
        int total_clock_cycles=0;
        int structural_stall_cycles = 0;
        int total_instructions_number = 0;
        bool stall_counted = false;
        
    public:
        Processor(std::string file_name) : ALU1(1, 2, 2),
         ALU2(1, 2, 2), MUL_DIV(2, 10, 20), LOAD_STORE(3, 5, 5),
         ALUs(3, entry_id), MUL_DIV_STAT(2, entry_id), LOAD_STAT(2, entry_id), STORE_STAT(2, entry_id) {
            unit.Reset_program(file_name);
         };

        void Run_Simulation();
        
        //step 1 functions
        void Pass_Instruction_To_Unit(int type, int operand1, int operand2, int operand3, bool operation);
        bool Check_Store_Buffer(int load_index);
        bool Check_Load_Buffer(int store_index);
        void Check_Ready_Registration_Entry();

        //step 2 functions
        void Update_Stations_Entries();
        void Update_Register_File();

        void Clear_ALU_Station_Entry();
        void Clear_MUL_DIV_Station_Entry();
        void Clear_LOAD_Station_Entry();
        void Clear_STORE_Station_Entry(int station_ID);

        void Write_Output_Ready_Func_Units();

        //step 3 functions
        void Update_Functional_Units_Status();

        //step 4 functions
        void Get_New_Instruction();

        int Get_Available_ALU_Station();
        int Get_Available_MUL_DIV_Station();
        int Get_Available_LOAD_Station();
        int Get_Available_STORE_Station();

        void Pass_Instruction_To_ALU_Station(int index);
        void Pass_Instruction_To_MUL_DIV_Station(int index);
        void Pass_Instruction_To_LOAD_Station(int index);
        void Pass_Instruction_To_STORE_Station(int index);

        bool Check_Execution_Complete();

        void Print_Execution_Diagnostics();
};

#endif