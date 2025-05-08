#include "Registration_Station.h"
#include "Registration_Entry.h"
#include <vector>

using namespace std;

Registration_Station::Registration_Station(int size, int &curr_id) {
    entry_numbers = size;

    for(int i=0; i<entry_numbers; i++) {
        entries.emplace_back(curr_id);
        curr_id++;
    }
}