#include "Registration_Station.h"
#include "Registration_Entry.h"
#include <vector>
#include <iostream>

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

bool Registration_Station::Check_Entry_Sent_To_Unit(int index){
    return entries[index].Check_Sent_To_Unit();
}

void Registration_Station::Set_Entry_Sent_To_Unit(int index){
    entries[index].Set_Sent_To_Unit();
}

void Registration_Station::Retrieve_Instruction_From_Entry(int index, int (&instr)[4]){
    entries[index].Get_Instruction_Complete(instr);
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

void Registration_Station::Set_Operation_For_Entry(int index, int op_type){
    entries[index].Set_Operation(op_type);
}

void Registration_Station::Reset_Entry_I(int index){
    entries[index].Reset_Entry();
}

int Registration_Station::Get_Entry_ID(int index){
    return entries[index].Return_ID();
}

void Registration_Station::Increment_Entry_Address_Age(int index){
    entries[index].Increment_Address_Age();
}

int Registration_Station::Get_Entry_Address_Age(int index){
    return entries[index].Get_Address_Age();
}

ostream& operator<<(ostream& os, const Registration_Station& obj){
    os << "Here is the content of this reservation system." << endl;

    for(int i=0; i<obj.entry_numbers; i++) {
        os << obj.entries[i];
    }
    return os;
}
