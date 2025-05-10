#ifndef REG_H
#define REG_H
#include "Registration_Entry.h"
#include <vector>

using namespace std;

class Registration_Station {
    private:
        int entry_numbers;
        vector<Registration_Entry> entries;
    
    public:
        Registration_Station(int size, int &current_id);
        
        bool Check_Entry_Ready(int index);
        bool Check_Entry_Busy(int index);
        bool Check_Entry_Available(int index);
        
        void Retrieve_Instruction_From_Entry(int index, int (&instr)[4]);
        
        int Retrieve_Q_From_Entry(int index, int Q_index);
        void Set_Value_For_Entry(int index, int V_index, int value);

        void Set_Q_For_Entry(int index, int Q_index, int value);
};

#endif