#include "Memory_Unit.h"

int Memory_Unit::Load_From_Memory(int Address) {
    return memory_array[Address];
}

void Memory_Unit::Store_To_Memory(int Address, int value) {
    memory_array[Address] = value;
}

int Memory_Unit::Get_Memory_Size() {
    return size;
}
