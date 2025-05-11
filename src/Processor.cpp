#include "Issue_Unit.h"
#include "Register_File.h"
#include "Register_Status.h"
#include "Memory_Unit.h"
#include "Functional_Unit.h"
#include "Registration_Entry.h"
#include "Registration_Station.h"
#include "Processor.h"
#include "iostream"
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

bool Processor::Check_Store_Buffer(int load_index){
    int temp[4] = {-1, -1, -1, -1};
    if(STORE_STAT.Check_Entry_Busy(0) && 
    STORE_STAT.Retrieve_Q_From_Entry(0, 1)==0 && STORE_STAT.Retrieve_Q_From_Entry(0, 2)==0){
        
        STORE_STAT.Retrieve_Instruction_From_Entry(0, temp);
        cout << temp[0]+temp[1] << endl;
        cout << instruct[0]+instruct[1] << endl;

        if(temp[0]+temp[1]==instruct[0]+instruct[1] && 
            STORE_STAT.Get_Entry_Address_Age(0) > LOAD_STAT.Get_Entry_Address_Age(load_index)){
            return true;
        }
    }

    if(STORE_STAT.Check_Entry_Busy(1) && STORE_STAT.Retrieve_Q_From_Entry(1, 1)==0 && STORE_STAT.Retrieve_Q_From_Entry(1, 2)==0){
        STORE_STAT.Retrieve_Instruction_From_Entry(1, temp);
        if(temp[0]+temp[1]==instruct[0]+instruct[1] && 
            STORE_STAT.Get_Entry_Address_Age(1) > LOAD_STAT.Get_Entry_Address_Age(load_index)){
            return true;
        }
    }

    return false;
}

bool Processor::Check_Load_Buffer(int store_index){
    int temp[4] = {-1, -1, -1, -1};
    if(LOAD_STAT.Check_Entry_Busy(0) && LOAD_STAT.Check_Entry_Ready(0)){
        LOAD_STAT.Retrieve_Instruction_From_Entry(0, temp);
        if(temp[0]+temp[1]==instruct[0]+instruct[1] && 
            LOAD_STAT.Get_Entry_Address_Age(0) > STORE_STAT.Get_Entry_Address_Age(store_index)){
            return true;
        }
    }

    if(LOAD_STAT.Check_Entry_Busy(1) && LOAD_STAT.Check_Entry_Ready(1)){
        LOAD_STAT.Retrieve_Instruction_From_Entry(1, temp);
        if(temp[0]+temp[1]==instruct[0]+instruct[1] && 
            LOAD_STAT.Get_Entry_Address_Age(1) > STORE_STAT.Get_Entry_Address_Age(store_index)){
            return true;
        }
    }

    return false;
}

void Processor::Check_Ready_Registration_Entry() {
    
    for(int i=0; i<3; i++) {
        std::fill(begin(instruct), end(instruct), -1);
        if(ALUs.Check_Entry_Busy(i) && ALUs.Check_Entry_Ready(i) && !ALUs.Check_Entry_Sent_To_Unit(i)) {
            if(ALU1.Check_Available() || ALU2.Check_Available()){
                ALUs.Retrieve_Instruction_From_Entry(i, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-1));
                ALUs.Set_Entry_Sent_To_Unit(i);
            }
        }
    }

    for (int j=0; j<2; j++){
        std::fill(begin(instruct), end(instruct), -1);
        if(MUL_DIV_STAT.Check_Entry_Busy(j) && MUL_DIV_STAT.Check_Entry_Ready(j) && !MUL_DIV_STAT.Check_Entry_Sent_To_Unit(j)){
            if(MUL_DIV.Check_Available()) {
                MUL_DIV_STAT.Retrieve_Instruction_From_Entry(j, instruct);
                Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-3));
                MUL_DIV_STAT.Set_Entry_Sent_To_Unit(j);
            }
        }
    }

    for (int k=0; k<2; k++){
        std::fill(begin(instruct), end(instruct), -1);
        if(LOAD_STAT.Check_Entry_Busy(k) && LOAD_STAT.Check_Entry_Ready(k) && !LOAD_STAT.Check_Entry_Sent_To_Unit(k)){
            if(LOAD_STORE.Check_Available()) {
                LOAD_STAT.Retrieve_Instruction_From_Entry(k, instruct);
                if (!Check_Store_Buffer(k)){
                    Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-5));
                    LOAD_STAT.Set_Entry_Sent_To_Unit(k);
                }
            }
            LOAD_STAT.Increment_Entry_Address_Age(k);
        }
    }

    for (int z=0; z<2; z++){
        std::fill(begin(instruct), end(instruct), -1);
        if(STORE_STAT.Check_Entry_Busy(z) && STORE_STAT.Check_Entry_Ready(z) && !STORE_STAT.Check_Entry_Sent_To_Unit(z)){
            if(LOAD_STORE.Check_Available()) {
                STORE_STAT.Retrieve_Instruction_From_Entry(z, instruct);
                if(!Check_Load_Buffer(z)){
                    Pass_Instruction_To_Unit(instruct[3], instruct[0], instruct[1], instruct[2], !(instruct[3]-5));
                    store_station_ID=STORE_STAT.Get_Entry_ID(z);
                    STORE_STAT.Set_Entry_Sent_To_Unit(z);
                }
            }
            
        }
        if(STORE_STAT.Check_Entry_Busy(z) && STORE_STAT.Retrieve_Q_From_Entry(z, 1)==0 && STORE_STAT.Retrieve_Q_From_Entry(z, 2)==0){
        STORE_STAT.Increment_Entry_Address_Age(z);
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

void Processor::Clear_ALU_Station_Entry(){
    for (int i=0; i<3; i++){
        if(ALUs.Get_Entry_ID(i)==result[1]){
            ALUs.Reset_Entry_I(i);
            break;
        }
    }
}

void Processor::Clear_MUL_DIV_Station_Entry(){
    for (int i=0; i<2; i++){
        if(MUL_DIV_STAT.Get_Entry_ID(i)==result[1]){
            MUL_DIV_STAT.Reset_Entry_I(i);
            break;
        }
    }
}

void Processor::Clear_LOAD_Station_Entry(){
    for (int i=0; i<2; i++){
        if(LOAD_STAT.Get_Entry_ID(i)==result[1]){
            LOAD_STAT.Reset_Entry_I(i);
            break;
        }
    }
}

void Processor::Clear_STORE_Station_Entry(int Station_ID){
    for (int i=0; i<2; i++){
        if(STORE_STAT.Get_Entry_ID(i)==Station_ID){
            STORE_STAT.Reset_Entry_I(i);
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
        Clear_ALU_Station_Entry();
        ALU1.Reset_Unit();
    }
    else if(CDB_Available && ALU2.Check_Output()) {
        CDB_Available = false;
        ALU2.Return_Result(result);
        Update_Stations_Entries();
        Update_Register_File();
        Clear_ALU_Station_Entry();
        ALU2.Reset_Unit();
    }
    else if(CDB_Available && MUL_DIV.Check_Output()) {
        CDB_Available = false;
        MUL_DIV.Return_Result(result);
        if(result[1]==-1){
            instruction_error = true;
            return;
        }
        Update_Stations_Entries();
        Update_Register_File();
        Clear_MUL_DIV_Station_Entry();
        MUL_DIV.Reset_Unit();
    }
    else if(CDB_Available && LOAD_STORE.Check_Output()) {
        CDB_Available = false;
        LOAD_STORE.Return_Result(result, &Memory);
        if(result[1]==-1){
            instruction_error = true;
            return;
        } else if (result[1]!=0){ //only the store instruction will set result[1] to 0, all others will set it to a value greater than 0 as all reservation entries have IDs > 1
            Update_Stations_Entries();
            Update_Register_File();
            Clear_LOAD_Station_Entry();
        } else {
            Clear_STORE_Station_Entry(store_station_ID);
            store_station_ID = -1;
        }
        LOAD_STORE.Reset_Unit();
    }
    CDB_Available = true;
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

int Processor::Get_Available_LOAD_Station() {
    for (int i=0; i<2; i++){
        if(LOAD_STAT.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

int Processor::Get_Available_STORE_Station(){
    for (int i=0; i<2; i++){
        if(STORE_STAT.Check_Entry_Available(i)){
            return i;
        }
    }
    return -1;
}

void Processor::Pass_Instruction_To_ALU_Station(int index){
    ALUs.Set_Operation_For_Entry(index, instruct_issue[0]);
    int temp = reg_stat_unit.Get_Register_Result(instruct_issue[2]);
    if(temp==0){
        ALUs.Set_Value_For_Entry(index, 1, reg_unit.Get_Register_Value(instruct_issue[2]));
    } else {
        ALUs.Set_Q_For_Entry(index, 1, temp);
    }

    temp = reg_stat_unit.Get_Register_Result(instruct_issue[3]);
    if(temp==0){
        ALUs.Set_Value_For_Entry(index, 2, reg_unit.Get_Register_Value(instruct_issue[3]));
    } else {
        ALUs.Set_Q_For_Entry(index, 2, temp);
    }

    reg_stat_unit.Set_Register_Result(instruct_issue[1], ALUs.Get_Entry_ID(index));
}

void Processor::Pass_Instruction_To_MUL_DIV_Station(int index){
    MUL_DIV_STAT.Set_Operation_For_Entry(index, instruct_issue[0]);
    int temp = reg_stat_unit.Get_Register_Result(instruct_issue[2]);
    if(temp==0){
        MUL_DIV_STAT.Set_Value_For_Entry(index, 1, reg_unit.Get_Register_Value(instruct_issue[2]));
    } else {
        MUL_DIV_STAT.Set_Q_For_Entry(index, 1, temp);
    }

    temp = reg_stat_unit.Get_Register_Result(instruct_issue[3]);
    if(temp==0){
        MUL_DIV_STAT.Set_Value_For_Entry(index, 2, reg_unit.Get_Register_Value(instruct_issue[3]));
    } else {
        MUL_DIV_STAT.Set_Q_For_Entry(index, 2, temp);
    }

    reg_stat_unit.Set_Register_Result(instruct_issue[1], MUL_DIV_STAT.Get_Entry_ID(index));
}

void Processor::Pass_Instruction_To_LOAD_Station(int index){
    LOAD_STAT.Set_Operation_For_Entry(index, instruct_issue[0]);
    LOAD_STAT.Set_Value_For_Entry(index, 1, instruct_issue[2]);

    int temp = reg_stat_unit.Get_Register_Result(instruct_issue[3]);
    if(temp==0){
        LOAD_STAT.Set_Value_For_Entry(index, 2, reg_unit.Get_Register_Value(instruct_issue[3]));
    } else {
        LOAD_STAT.Set_Q_For_Entry(index, 2, temp);
    }

    reg_stat_unit.Set_Register_Result(instruct_issue[1], LOAD_STAT.Get_Entry_ID(index));
}

void Processor::Pass_Instruction_To_STORE_Station(int index){
    STORE_STAT.Set_Operation_For_Entry(index, instruct_issue[0]);
    STORE_STAT.Set_Value_For_Entry(index, 1, instruct_issue[1]);

    int temp = reg_stat_unit.Get_Register_Result(instruct_issue[2]);
    if(temp==0){
        STORE_STAT.Set_Value_For_Entry(index, 2, reg_unit.Get_Register_Value(instruct_issue[2]));
    } else {
        STORE_STAT.Set_Q_For_Entry(index, 2, temp);
    }

    temp = reg_stat_unit.Get_Register_Result(instruct_issue[3]);
    if(temp==0){
        STORE_STAT.Set_Value_For_Entry(index, 3, reg_unit.Get_Register_Value(instruct_issue[3]));
    } else {
        STORE_STAT.Set_Q_For_Entry(index, 3, temp);
    }
}

//check program execution completed
bool Processor::Check_Execution_Complete(){
    bool busy = false;

    for(int i=0; i<3; i++){
        if(i==2){
            busy = busy || ALUs.Check_Entry_Busy(i);
        } else {
            busy = busy || ALUs.Check_Entry_Busy(i) ||
             MUL_DIV_STAT.Check_Entry_Busy(i) || LOAD_STAT.Check_Entry_Busy(i) || STORE_STAT.Check_Entry_Busy(i);
        }

        if(busy){
            return false;
        }
    }
    return true;
}

void Processor::Run_Simulation() {
    //first, check if any reservation station is ready to send its instruction to a functional unit
    //and pass any ready instruction to an available unit
    while(!execution_complete) {
        Check_Ready_Registration_Entry();

        //second, check if any functional unit finished execution and wants to write its results to functional units
        //and to the register file
        Write_Output_Ready_Func_Units();
        if(instruction_error){
            cout << "Error occured during instruction execution. Terminating the program.";
            return;
        }

        //third, we go over the functional units and update their status if there is a currently running instruction
        Update_Functional_Units_Status();

        //finally, we check if there is any instruction left to decode and try to issue it to a reservation station 
        int temp_ID = -1;
        if(!instruction_issue_complete){
            if(!instruction_issue_stall){
                Get_New_Instruction();
                if(instruction_error){
                    cout << "Error occured during instruction decoding. Terminating the program.";
                    return;
                }
            }

            switch (instruct_issue[0])
            {
            case 1:
            case 2:
                temp_ID = Get_Available_ALU_Station();
                if(temp_ID != -1){
                    Pass_Instruction_To_ALU_Station(temp_ID);
                    if(instruction_issue_stall){
                        instruction_issue_stall=false;
                    }
                }
                //cout << "\n" << endl;
                //cout << ALU2;
                break;
            
            case 3:
            case 4:
                temp_ID = Get_Available_MUL_DIV_Station();
                if(temp_ID != -1){
                    Pass_Instruction_To_MUL_DIV_Station(temp_ID);
                    if(instruction_issue_stall){
                        instruction_issue_stall=false;
                    }
                }
                break;
            
            case 5:
                temp_ID = Get_Available_STORE_Station();
                if(temp_ID != -1){
                    Pass_Instruction_To_STORE_Station(temp_ID);
                    if(instruction_issue_stall){
                        instruction_issue_stall=false;
                    }
                }
                break;
            
            case 6:
                temp_ID = Get_Available_LOAD_Station();
                if(temp_ID != -1){
                    Pass_Instruction_To_LOAD_Station(temp_ID);
                    if(instruction_issue_stall){
                        instruction_issue_stall=false;
                    }
                }
                break;

            default:
                break;
            }
        } else {
            //When all instructions are issued to reservation stations, check if all reservation stations are cleared
            execution_complete = Check_Execution_Complete();
        }
        total_clock_cycles++;
        
    }

    cout << "Total cycles: " << total_clock_cycles << endl;
    cout << reg_unit;
    
}