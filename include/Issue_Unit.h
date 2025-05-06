#ifndef ISSUE_H
#define ISSUE_H
#include <string>

class Issue_Unit {
    private:
        std::string file_name;
        bool instructions_over;
        int instruction_counter;

    public:
        Issue_Unit(std::string name);
        void Current_Program_Info();
};

#endif