#ifndef REG_H
#define REG_H
#include "Registration_Entry.h"
#include <vector>

using namespace std;

class Registration_Station {
    private:
        int entry_numbers;
        vector<Registration_Entry> entries;
    
    public:
        Registration_Station(int size, int &current_id);
};

#endif