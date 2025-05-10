#include "Registration_Station.h"
#include "Registration_Entry.h"
#include <vector>

using namespace std;

Registration_Station::Registration_Station(int size, int &curr_id) {
    entry_numbers = size;

    for(int i=0; i<entry_numbers; i++) {
        entries.emplace_back(curr_id);
        curr_id++;
    }
}

bool Registration_Station::Check_Entry_Ready(int index) {
    return entries[index].Operands_Ready();
}

bool Registration_Station::Check_Entry_Busy(int index) {
    return entries[index].Check_Busy();
}

bool Registration_Station::Check_Entry_Available(int index) {
    return entries[index].Check_Available();
}

void Registration_Station::Retrieve_Instruction_From_Entry(int index, int (&instr)[4]){
    return entries[index].Get_Instruction_Complete(instr);
}

int Registration_Station::Retrieve_Q_From_Entry(int index, int Q_index) {
    return entries[index].Get_Operand_Source(Q_index);
}

void Registration_Station::Set_Value_For_Entry(int index, int V_index, int value){
    entries[index].Set_Operand(V_index, value);
}

void Registration_Station::Set_Q_For_Entry(int index, int Q_index, int value){
    entries[index].Set_Operand_Source(Q_index, value);
}
