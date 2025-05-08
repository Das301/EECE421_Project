#ifndef MEMORY_H
#define MEMORY_H

class Memory_Unit {
    private:
        int size = 1024;
        int memory_array[1024] = {0};
    public:
        int Load_From_Memory(int Address);
        void Store_To_Memory(int Address, int value);
        int Get_Memory_Size();
};

#endif