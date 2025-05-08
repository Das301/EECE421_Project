#include "Functional_Unit.h"
#include "Memory_Unit.h"
#include <iostream>

using namespace std;

Functional_Unit::Functional_Unit(int type, int op1_cycles, int op2_cycles) {
    this->type = type;
    this->op1_cycles = op1_cycles;
    this->op2_cycles = op2_cycles;

    busy = false;
    output_ready = false;
    curr_cycle=0;

    operand1=0;
    operand2=0;
    operand3=0;

    operation = false;
}

void Functional_Unit::Set_Operation(int operand1, int operand2, int operand3, bool operation){
    this->operand1=operand1;
    this->operand2=operand2;
    this->operand3=operand3;
    this->operation = operation;

    busy = true;
}

void Functional_Unit::Update_Operation_Status(){
    if(operation && curr_cycle > op1_cycles) {
        output_ready==true;
    } else if (!operation && curr_cycle > op2_cycles) {
        output_ready==true;
    } else {
        curr_cycle++;
    }
}

bool Functional_Unit::Check_Available() {
    return !busy;
}

bool Functional_Unit::Check_Output() {
    return output_ready;
}

void Functional_Unit::Return_Result(int (&arr_result)[2], Memory_Unit* memory) {
    switch (type)
    {
    case 1:
        if(operation) {
            arr_result[0] = operand1 + operand2;
        } else {
            arr_result[0] = operand1 - operand2;
        }
        arr_result[1] = operand3;
        break;
    case 2:
        if(operation) {
            arr_result[0] = operand1 * operand2;
        } else {
            arr_result[0] = operand1 / operand2;
        }
        arr_result[1] = operand3;
        break;
    case 3:
        if(operation) {
            //store operand value in memory
            arr_result[0] = 0;
            arr_result[1] = 0;
            if(operand1+operand2 > memory->Get_Memory_Size() || operand1+operand2 <0) {
                cout << "Memory address out of range!";
                arr_result[1] = -1;
                break;
            }
            memory->Store_To_Memory(operand1+operand2, operand3);
        } else {
            //load value from memory
            arr_result[1] = operand3;
            if(operand1+operand2 > memory->Get_Memory_Size() || operand1+operand2 <0) {
                cout << "Memory address out of range!";
                arr_result[1] = -1;
                break;
            }
            arr_result[0] = memory->Load_From_Memory(operand1+operand2);
        }
        break;
    default:
        break;
    }
}

void Functional_Unit::Reset_Unit(){
    busy = false;
    output_ready = false;
    curr_cycle=0;

    operand1=0;
    operand2=0;
    operand3=0;

    operation = false;
}
