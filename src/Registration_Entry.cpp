#include "Registration_Entry.h"

Registration_Entry::Registration_Entry(int ID) {
    station_ID = ID;
}

bool Registration_Entry::Check_Busy() {
    return busy;
}

bool Registration_Entry::Check_Available() {
    return !busy;
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
    int op = 0;
    int V1 = 0;
    int V2 = 0;
    int Q1 = 0;
    int Q2 = 0;

    int V_store = 0;
    int Q_store = 0;

    bool busy = false;
}
