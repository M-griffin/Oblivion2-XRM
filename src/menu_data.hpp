#ifndef MENU_DATA_HPP
#define MENU_DATA_HPP

#include "structures.hpp"
#include <string>

/**
 * @class MenuData
 * @author Michael Griffin
 * @date 10/6/2015
 * @file menu_data.hpp
 * @brief Halds Binary funcations for reading .MNU Data Files.
 */
class MenuData
{
public:
    MenuData()
    { }

    ~MenuData()
    { }

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
        if(std::is_same<T, MenuInfo>::value)
        {
            path.append(MENU_PATH);
        }
        else
        {
            // Else we want to read MENUPROMPT.DAT in DATA Folder
            path.append(DATA_PATH);
        }
#ifdef _WIN32
        path.append("\\");
#else
        path.append("/");
#endif
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
        std::string path = MENU_PATH;
#ifdef _WIN32
        path.append("\\");
#else
        path.append("/");
#endif
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
        if(fseek(stream, sizeof(MenuInfo) + ((int)idx * sizeof(T)), SEEK_SET) == 0)
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
        if(std::is_same<T, MenuInfo>::value)
        {
            path.append(MENU_PATH);
        }
        else
        {
            // Else we want to read MENUPROMPT.DAT in DATA Folder
            path.append(DATA_PATH);
        }
#ifdef _WIN32
        path.append("\\");
#else
        path.append("/");
#endif
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
        std::string path = MENU_PATH;
#ifdef _WIN32
        path.append("\\");
#else
        path.append("/");
#endif
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
        if(fseek(stream, sizeof(MenuInfo) + ((int)idx * sizeof(T)), SEEK_SET) == 0)
            x = fread(t, sizeof(T), 1, stream);
        fclose(stream);
        return x;
    }
};

#endif // MENU_DATA_HPP
