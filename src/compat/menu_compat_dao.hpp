#ifndef MENU_COMPAT_DAO_HPP
#define MENU_COMPAT_DAO_HPP

#include "../model-sys/structures.hpp"
#include <string>

/**
 * @class Compat
 * @author Michael Griffin
 * @date 10/6/2015
 * @file menu_data.hpp
 * @brief Holds Binary funcations for reading .MNU Data Files. Original OBV/2
 */
class MenuCompatDao
{
public:
    
    explicit MenuCompatDao()
    { }

    ~MenuCompatDao()
    { }

    /**
     * @brief Appending forward or backspash to path
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
    int recordWriteInfo(T *t, std::string filename, int idx)
    {
        std::string path = "";
        // Use to read from either Menu or Data Records
        if(std::is_same<T, MenuCompatInfo>::value)
        {
            path.append(GLOBAL_MENU_PATH);
        }
        else
        {
            // Else we want to read MENUPROMPT.DAT in DATA Folder
            path.append(GLOBAL_DATA_PATH);
        }

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
                std::cout << "Error writing " << filename << std::endl;
                return x;
            }
        }
        if(fseek(stream, (int)idx * sizeof(T), SEEK_SET) == 0)
            x = fwrite(t, sizeof(T), 1, stream);
        fclose(stream);
        return x;
    }

    template <typename T>
    int recordWriteOption(T *t, std::string filename, int idx)
    {
        std::string path = GLOBAL_MENU_PATH;
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
                std::cout << "Error writing " << filename << std::endl;
                return x;
            }
        }
        if(fseek(stream, sizeof(MenuCompatInfo) + ((int)idx * sizeof(T)), SEEK_SET) == 0)
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
    int recordReadInfo(T *t, std::string filename, int idx)
    {
        std::string path = "";
        // Use to read from either Menu or Data Records
        if(std::is_same<T, MenuCompatInfo>::value)
        {
            path.append(GLOBAL_MENU_PATH);
        }
        else
        {
            // Else we want to read MENUPROMPT.DAT in DATA Folder
            path.append(GLOBAL_DATA_PATH);
        }

        pathSeperator(path);
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");
        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
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

    template <typename T>
    int recordReadOption(T *t, std::string filename, int idx)
    {
        std::string path = GLOBAL_MENU_PATH;
        pathSeperator(path);
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");
        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
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
        if(fseek(stream, sizeof(MenuCompatInfo) + ((int)idx * sizeof(T)), SEEK_SET) == 0)
            x = fread(t, sizeof(T), 1, stream);
        fclose(stream);
        return x;
    }
};

#endif // MENU_DATA_HPP
