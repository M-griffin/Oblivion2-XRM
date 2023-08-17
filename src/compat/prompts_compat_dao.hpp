#ifndef PROMPT_DAO_HPP
#define PROMPT_DAO_HPP

#include "../model-sys/structures.hpp"
#include "../model-sys/struct_compat.hpp"
#include "../logging.hpp"

#include <string>

/**
 * @class PromptDao
 * @author Michael Griffin
 * @date 3/18/2016
 * @file prompt_dao.hpp
 * @brief Holds Binary functions for reading MENUPROMPTS.DAT Original OBV/2
 */
class PromptDao
{
public:

    explicit PromptDao()
    { }

    ~PromptDao()
    { }

    /**
     * @brief Appending forward or backslash to path
     * @param path
     */
    void pathSeperator(std::string &path)
    {
#ifdef _WIN32
        path.append("\\");
#else
        path.append("/");
#endif
    }

    /*
     * Menu Info is a header record in each menu. It should be read first.
     * All following menu options must skip past the initial MenuInfo Size.
     */

    /**
     * @brief Writing Binary Data Records to Disk
     * @param t
     * @param filename
     * @param idx
     * @return
     */
    template <typename T>
    int recordWrite(T *t, std::string filename, int idx)
    {
        std::string path = "";
        path.append(GLOBAL_DATA_PATH);
        pathSeperator(path);
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");

        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
            stream = fopen(path.c_str(), "wb");

            if(stream == nullptr)
            {
                Logging &log = Logging::getInstance();
                log.write<Logging::ERROR_LOG>("Error writing file=", filename, __FILE__, __LINE__);
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
    int recordRead(T *t, std::string filename, int idx)
    {
        std::string path = "";
        path.append(GLOBAL_DATA_PATH);
        pathSeperator(path);
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");

        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
            Logging &log = Logging::getInstance();
            log.write<Logging::ERROR_LOG>("Error Reading/creating file=", filename, __FILE__, __LINE__);
            stream = fopen(path.c_str(), "wb");

            if(stream == nullptr)
            {
                log.write<Logging::ERROR_LOG>("Error Reading file=", filename, __FILE__, __LINE__);
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

};

#endif // PROMPT_DAO_HPP
