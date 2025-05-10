#ifndef ISSUE_H
#define ISSUE_H
#include <string>

class Issue_Unit {
    private:
        std::string file_name;
        bool instructions_over;
        int instruction_counter;

    public:
        Issue_Unit();
        Issue_Unit(std::string name);

        void Reset_program(std::string name="");

        bool Fetch_Decode_Instruction(int (&instruct)[4]);

        void Decode_Instruction(std::string instruction, int (&instruct)[4]);

        void Current_Program_Info();
};

#endif