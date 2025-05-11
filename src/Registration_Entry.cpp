#include "Registration_Entry.h"

Registration_Entry::Registration_Entry(int ID) {
    station_ID = ID;
}

int Registration_Entry::Return_ID(){
    return station_ID;
}

bool Registration_Entry::Check_Busy() {
    return busy;
}

bool Registration_Entry::Check_Available() {
    return !busy;
}

bool Registration_Entry::Check_Sent_To_Unit() {
    return sent_to_unit;
}

void Registration_Entry::Set_Sent_To_Unit() {
    sent_to_unit = true;
}

bool Registration_Entry::Operands_Ready() {
    if (op==5) {
        return (Q1==0 && Q2==0 && Q_store==0);
    }
    return (Q1==0 && Q2==0);
}

void Registration_Entry::Set_Operation(int op_type) {
    busy = true;
    op = op_type;
}

void Registration_Entry::Set_Operand_Source(int Q_index, int value) {
    if (Q_index == 1) {
        Q1 = value;
    } else if(Q_index == 2){
        Q2 = value;
    } else {
        Q_store = value;
    }
}

void Registration_Entry::Set_Operand(int V_index, int value) {
    if (V_index == 1) {
        V1 = value;
        Q1=0;
    } else if (V_index == 2) {
        V2 = value;
        Q2=0;
    } else {
        V_store = value;
        Q_store = 0;
    }
}

int Registration_Entry::Get_Operand_Source(int Q_index) {
    if(Q_index==1){
        return Q1;
    } else if(Q_index==2) {
        return Q2;
    } else {
        return Q_store;
    }
}

void Registration_Entry::Get_Instruction_Complete(int (&instruct_details)[4]) {
    instruct_details[0] = V1;
    instruct_details[1] = V2;
    if(op==5) {
        instruct_details[2] = V_store;
    } else {
        instruct_details[2] = station_ID;
    }
    instruct_details[3] = op;
}

void Registration_Entry::Reset_Entry() {
    op = 0;
    V1 = 0;
    V2 = 0;
    Q1 = 0;
    Q2 = 0;

    V_store = 0;
    Q_store = 0;

    busy = false;
    sent_to_unit = false;
    address_age = 0;
}

void Registration_Entry::Increment_Occupied_Time(){
    occupied_time++;
}

int Registration_Entry::Get_Occupied_Time(){
    return occupied_time;
}

void Registration_Entry::Increment_Address_Age(){
    address_age++;
}

int Registration_Entry::Get_Address_Age(){
    return address_age;
}

ostream& operator<<(ostream& os, const Registration_Entry& obj){
    os << "Station ID: " << obj.station_ID << endl;
    os << "Busy: " << obj.busy << endl;
    os << "Operation: " << obj.op << endl;
    os << "V1: " << obj.V1 << endl;
    os << "V2: " << obj.V2 << endl;
    os << "Q1: " << obj.Q1 << endl;
    os << "Q2: " << obj.Q2 << endl;
    if(obj.op==5){
        os << "V_Store: " << obj.V_store << endl;
        os << "Q_Store: " << obj.Q_store << endl;
    }
    os << "---------------------------" << endl;
    return os;
}
