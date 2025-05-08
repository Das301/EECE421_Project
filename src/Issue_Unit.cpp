#include "Issue_Unit.h"
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

Issue_Unit::Issue_Unit(std::string name)
{
    file_name = name;
    instructions_over = false;
    instruction_counter = 1;
}

void Issue_Unit::Reset_program(std::string name) {
    if(name!=""){
        file_name = name;
    }
    instructions_over = false;
    instruction_counter = 1;
}

void Issue_Unit::Decode_Instruction(std::string instruction, int (&instruct)[4]) {
    unordered_map<string, int> ops = {
        {"ADD", 1}, {"SUB", 2}, {"MUL", 3}, {"DIV", 4}, {"STORE", 5}, {"LOAD", 6}
    };

    unordered_map<string, int> registers = {
        {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}
    };

    bool load_op = false;
    bool store_op = false;
    string temp = "";
    std::unordered_map<std::string, int>::iterator it;

    int i=0;
    //identifying the operation type
    for(i; i<instruction.length(); i++) {
        if(instruction[i]==' ') {
            it = ops.find(temp);
            if(it==ops.end()) {
                cout << "Invalid Instruction type at line " << instruction_counter << endl;
                return;
            }

            instruct[0] = it->second;

            switch (it->second)
            {
            case 5:
                load_op=true;
                break;
            case 6:
                store_op=true;
                break;
            default:
                load_op=false;
                store_op=false;
                break;
            }

            temp = "";
            i++;
            break;
        }
        temp = temp + instruction[i];
    }
    
    if(instruct[0]==-1) {
        cout << "Invalid instruction type at line " << instruction_counter << endl;
        return;
    }

    int curr_operand = 1;
    //getting the operands for ADD, SUB, MUl, DIV operations
    if(!load_op && !store_op) {
        for(i; i<=instruction.length(); i++) {
            if(i==instruction.length() || instruction[i]==','){
                it = registers.find(temp);
                if(it==registers.end()) {
                    cout << "Invalid register at line " << instruction_counter << endl;
                    return;
                }
                
                instruct[curr_operand] = it->second;
                temp="";
                curr_operand++;
                if (curr_operand > 3 && i!=instruction.length()) {
                    cout << "Invalid instruction at line " << instruction_counter << ". Too many registers passed." << endl;
                    return;
                }
            } else if(instruction[i]!=' ') {
                temp = temp + instruction[i];
            }
        }
        
        // cout << "Instruction: " << instruction_counter << endl;
        // cout << "Operation: " << instruct[0] << endl;
        // cout << "Destination: " << instruct[1] << endl;
        // cout << "Source 1: " << instruct[2] << endl;
        // cout << "Source 2: " << instruct[3] << endl;
    } else if(load_op && !store_op) { //getting the operands for LOAD operation
        for(i; i<instruction.length(); i++) {
            if (curr_operand==1 || curr_operand==3) {
                if(instruction[i]==',' || instruction[i]==')'){
                    it = registers.find(temp);
                    if(it==registers.end()) {
                        cout << "Invalid register at line " << instruction_counter << endl;
                        return;
                    }
                    
                    instruct[curr_operand] = it->second;
                    temp="";
                    curr_operand++;
                    if (curr_operand > 3 && i!=instruction.length()-1) {
                        cout << "Invalid instruction at line " << instruction_counter << ". Too many registers passed." << endl;
                        return;
                    }
                }
            } else if(curr_operand==2) {
                if(instruction[i]=='('){
                    try {
                        instruct[curr_operand] = stoi(temp);
                        if (instruct[curr_operand] < 0) {
                            cout << "Invalid offset. Must be positive integer. Error at line " << instruction_counter << endl;
                            return;
                        }
                        temp="";
                        curr_operand++;
                    } catch (const std::invalid_argument&) {
                            cout << "Invalid offset. Expected positive integer but got invalid type. Error at line " << instruction_counter << endl;
                            return;
                    }
                }
            }
            
            if (instruction[i] != ' ' && instruction[i] != ')' && instruction[i] != '(' && instruction[i] != ',') {
                temp = temp + instruction[i];
            }
        }  
        // cout << "Instruction: " << instruction_counter << endl;
        // cout << "Operation: " << instruct[0] << endl;
        // cout << "Destination: " << instruct[1] << endl;
        // cout << "Offset: " << instruct[2] << endl;
        // cout << "Base: " << instruct[3] << endl;
    } else if(!load_op && store_op) { //getting the operands for STORE operation
        for(i; i<=instruction.length(); i++) {
            if(curr_operand==1) {
                if(instruction[i]=='('){
                    try {
                        instruct[curr_operand] = stoi(temp);
                        if (instruct[curr_operand] < 0) {
                            cout << "Invalid offset. Must be positive integer. Error at line " << instruction_counter << endl;
                            return;
                        }
                        temp="";
                        curr_operand++;
                    } catch (const std::invalid_argument&) {
                        cout << "Invalid offset. Expected positive integer but got invalid type. Error at line " << instruction_counter << endl;
                        return;
                    }
                }
            } else if (curr_operand>=2) {
                if(i==instruction.length() || instruction[i]==')'){
                    it = registers.find(temp);
                    if(it==registers.end()) {
                        cout << "Invalid register at line " << instruction_counter << endl;
                        return;
                    }
                    
                    instruct[curr_operand] = it->second;
                    temp="";
                    curr_operand++;
                    if (curr_operand > 3 && i!=instruction.length()) {
                        cout << "Invalid instruction at line " << instruction_counter << ". Too many registers passed." << endl;
                        return;
                    }
                }
            }
        
            if (instruction[i] != ' ' && instruction[i] != ')' && instruction[i] != '(' && instruction[i] != ',') {
                temp = temp + instruction[i];
            }
        }
        
        // cout << "Instruction: " << instruction_counter << endl;
        // cout << "Operation: " << instruct[0] << endl;
        // cout << "Offset: " << instruct[1] << endl;
        // cout << "Base: " << instruct[2] << endl;
        // cout << "Source: " << instruct[3] << endl;
    }
    
    if(instruct[0] == -1 || instruct[1] == -1 || instruct[2] == -1 || instruct[3] == -1) {
        cout << "Invalid instruction. Error at instruction: " << instruction_counter << endl;
        return;
    }
}

void Issue_Unit::Fetch_Decode_Instruction(int (&instruct)[4]) {
    string Instruction;
    int count = 1;
    bool done = true;

    ifstream MyCode(file_name);
    while (getline(MyCode, Instruction)) {
        if (count==instruction_counter) {
            Decode_Instruction(Instruction, instruct);
            instruction_counter++;
            done = false;
            break;
        }
        count++;
    }
    MyCode.close();

    if(done) {
        cout << "Issued all instructions." << endl;
        instructions_over = true;
    }

}


void Issue_Unit::Current_Program_Info() 
{
    cout << file_name << endl;
    cout << instructions_over << endl;
    cout << instruction_counter << endl;
}