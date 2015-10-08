
#include "struct.hpp"

#include <cstdio>
#include <string>
#include <iostream>
#include <stdint.h>

/*
 This file will hold basic template functios for read/writting
 Data Structures.
*/

/**
 * @brief Writing Binary Data Records to Disk
 * @param t
 * @param filename
 * @param idx
 * @return
 */
template <typename T>
int record_write(T *t, std::string filename, int idx)
{
    std::string path = DATA_PATH;
    path.append("\\");
    path.append(filename);

    int x = 0;
    FILE *stream = fopen(path.c_str(), "rb+");
    if(stream == nullptr)
    {
        stream = fopen(path.c_str(), "wb");
        if(stream == nullptr)
        {
            std::cout << "Error writting " << filename << std::endl;
            return x;
        }
    }
    if(fseek(stream, (int)idx * sizeof(T), SEEK_SET) == 0)
        x = fwrite(t, sizeof(T), 1, stream);
    fclose(stream);
    return x;
}

/**
 * @brief Reading Binary Data Records from Disk.
 * @param t
 * @param filename
 * @param idx
 * @return
 */
template <typename T>
int record_read(T *t, std::string filename, int idx)
{
    std::string path = DATA_PATH;
    path.append("\\");
    path.append(filename);

    int x = 0;
    FILE *stream = fopen(path.c_str(), "rb+");
    if(stream == nullptr)
    {
        std::cout << "Error Reading, Re-creating file. " << filename << std::endl;
        stream = fopen(path.c_str(), "wb");
        if(stream == nullptr)
        {
            std::cout << "Error Reading " << filename << std::endl;
            return x;
        }
    }
    fclose(stream);

    stream = fopen(path.c_str(), "rb");
    if(fseek(stream, (int)idx * sizeof(T), SEEK_SET) == 0)
        x = fread(t, sizeof(T), 1, stream);
    fclose(stream);
    return x;
}

/**
 * @brief Count Total Records on File.
 * @return
 */
template <typename T>
int record_count(T, std::string filename)
{
    T t;
    int i = 0;
    while(record_read(&t, filename, i++))
    ++i;

    (i < 1) ? i = -1 : i--;
    return(i);
}


