#include "Issue_Unit.h"
#include "Register_File.h"
#include "Register_Status.h"
#include "Memory_Unit.h"
#include "Functional_Unit.h"
#include "Registration_Entry.h"
#include "Registration_Station.h"
#include "Processor.h"
#include <string>
#include <algorithm>

using namespace std;

//step 1 functions
void Processor::Pass_Instruction_To_Unit(int type, int operand1, int operand2, int operand3, bool operation) {
    switch (type)
    {
    case 1:
    case 2:
        if(ALU1.Check_Available()) {
            ALU1.Set_Operation(operand1, operand2, operand3, operation);
        } else if (ALU2.Check_Available()) {
            ALU2.Set_Operation(operand1, operand2, operand3, operation);
        }
        break;

    case 3:
    case 4:
        MUL_DIV.Set_Operation(operand1, operand2, operand3, operation);
        break;
    
    case 5:
    case 6:
        LOAD_STORE.Set_Operation(operand1, operand2, operand3, operation);
        break;

    default:
        break;
    }
}

void Processor::Check_Ready_Registration_Entry() {
    
    for(int i=0; i<3; i++) {
        std::fill(begin(instruct), end(instruct), -1);
        if(ALUs.Check_Entry_Busy(i) && ALUs.Check_Entry_Ready(i)) {
            if(ALU1.Check_Available() || ALU2.Check_Available()){
                ALUs.Retrieve_Instruction_From_Entry(i, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-1));
            }
        }
    }

    for (int j=0; j<2; j++){
        std::fill(begin(instruct), end(instruct), -1);
        if(MUL_DIV_STAT.Check_Entry_Busy(j) && MUL_DIV_STAT.Check_Entry_Ready(j)){
            if(MUL_DIV.Check_Available()) {
                MUL_DIV_STAT.Retrieve_Instruction_From_Entry(j, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-3));
            }
        }
    }

    for (int k=0; k<2; k++){
        std::fill(begin(instruct), end(instruct), -1);
        if(LOAD_STAT.Check_Entry_Busy(k) && LOAD_STAT.Check_Entry_Ready(k)){
            if(LOAD_STORE.Check_Available()) {
                LOAD_STAT.Retrieve_Instruction_From_Entry(k, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-5));
            }
        }
    }

    for (int z=0; z<2; z++){
        std::fill(begin(instruct), end(instruct), -1);
        if(STORE_STAT.Check_Entry_Busy(z) && STORE_STAT.Check_Entry_Ready(z)){
            if(LOAD_STORE.Check_Available()) {
                STORE_STAT.Retrieve_Instruction_From_Entry(z, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-1));
            }
        }
    }
}

//step 2 functions
void Processor::Update_Stations_Entries() {
    for(int i=0; i<3; i++){
        if(ALUs.Retrieve_Q_From_Entry(i, 1)==result[1]){
            ALUs.Set_Value_For_Entry(i, 1, result[0]);
        }
        if(ALUs.Retrieve_Q_From_Entry(i, 2)==result[1]){
            ALUs.Set_Value_For_Entry(i, 2, result[0]);
        }
    }

    for(int j=0; j<2; j++){
        if(MUL_DIV_STAT.Retrieve_Q_From_Entry(j, 1)==result[1]){
            MUL_DIV_STAT.Set_Value_For_Entry(j, 1, result[0]);
        }
        if(MUL_DIV_STAT.Retrieve_Q_From_Entry(j, 2)==result[1]){
            MUL_DIV_STAT.Set_Value_For_Entry(j, 2, result[0]);
        }
    }

    for(int k=0; k<2; k++){
        if(LOAD_STAT.Retrieve_Q_From_Entry(k, 1)==result[1]){
            LOAD_STAT.Set_Value_For_Entry(k, 1, result[0]);
        }
        if(LOAD_STAT.Retrieve_Q_From_Entry(k, 2)==result[1]){
            LOAD_STAT.Set_Value_For_Entry(k, 2, result[0]);
        }
    }

    for(int z=0; z<2; z++){
        if(STORE_STAT.Retrieve_Q_From_Entry(z, 1)==result[1]){
            STORE_STAT.Set_Value_For_Entry(z, 1, result[0]);
        }
        if(STORE_STAT.Retrieve_Q_From_Entry(z, 2)==result[1]){
            STORE_STAT.Set_Value_For_Entry(z, 2, result[0]);
        }
        if(STORE_STAT.Retrieve_Q_From_Entry(z, 3)==result[1]){
            STORE_STAT.Set_Value_For_Entry(z, 3, result[0]);
        }
    }
    
}

void Processor::Update_Register_File() {
    for(int i=0; i<8; i++){
        if(reg_stat_unit.Get_Register_Result(i)==result[1]) {
            reg_unit.Set_Register_Value(i, result[0]);
            reg_stat_unit.Set_Register_Result(i, 0);
            break;
        }
    }
}

void Processor::Write_Output_Ready_Func_Units() {
    std::fill(begin(result), end(result), -1);
    if(CDB_Available && ALU1.Check_Output()) {
        CDB_Available = false;
        ALU1.Return_Result(result);
        Update_Stations_Entries();
        Update_Register_File();
    }
    else if(CDB_Available && ALU2.Check_Output()) {
        CDB_Available = false;
        ALU2.Return_Result(result);
        Update_Stations_Entries();
        Update_Register_File();
    }
    else if(CDB_Available && MUL_DIV.Check_Output()) {
        CDB_Available = false;
        MUL_DIV.Return_Result(result);
        Update_Stations_Entries();
        Update_Register_File();
    }
    else if(CDB_Available && LOAD_STORE.Check_Output()) {
        CDB_Available = false;
        LOAD_STORE.Return_Result(result);
        if(result[1]==-1){
            instruction_error = true;
            return;
        } else if (result[1]!=0){ //only the store instruction will set result[1] to 0, all others will set it to a value greater than 0 as all reservation entries have IDs > 1
            Update_Stations_Entries();
            Update_Register_File();
        }
    }
}

//step 3 functions
void Processor::Update_Functional_Units_Status(){
    if(ALU1.Check_Busy()){
        ALU1.Update_Operation_Status();
    }

    if(ALU2.Check_Busy()){
        ALU2.Update_Operation_Status();
    }
    
    if(MUL_DIV.Check_Busy()){
        MUL_DIV.Update_Operation_Status();
    }

    if(LOAD_STORE.Check_Busy()){
        LOAD_STORE.Update_Operation_Status();
    }
}

//step 4 functions
void Processor::Get_New_Instruction(){
    std::fill(begin(instruct_issue), end(instruct_issue), -1);
    if(!unit.Fetch_Decode_Instruction(instruct_issue)){
        if(instruct_issue[0] == -1 || instruct_issue[1] == -1 || instruct_issue[2] == -1 || instruct_issue[3] == -1){
            instruction_error = true;
        }
    } else {
        instruction_issue_complete = true;
    }
}

int Processor::Get_Available_ALU_Station(){
    for (int i=0; i<3; i++){
        if(ALUs.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

int Processor::Get_Available_MUL_DIV_Station() {
    for (int i=0; i<2; i++){
        if(MUL_DIV_STAT.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

int Processor::Get_Available_LAOD_Station() {
    for (int i=0; i<3; i++){
        if(LOAD_STAT.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

int Processor::Get_Available_STORE_Station(){
    for (int i=0; i<3; i++){
        if(STORE_STAT.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

void Processor::Run_Simulation() {
    //first, check if any reservation station is ready to send its instruction to a functional unit
    //and pass any ready instruction to an available unit

    //second, check if any functional unit finished execution and wants to write its results to functional units
    //and to the register file

    //third, we go over the functional units and update their status if there is a currently running instruction

    //finally, we check if there is any instruction left to decode and try to issue it to a reservation station 
}