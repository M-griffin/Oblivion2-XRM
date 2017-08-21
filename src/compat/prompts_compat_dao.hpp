#ifndef PROMPT_DAO_HPP
#define PROMPT_DAO_HPP

#include "../model-sys/structures.hpp"
#include "../model-sys/struct_compat.hpp"
#include <string>

/**
 * @class PromptDao
 * @author Michael Griffin
 * @date 3/18/2016
 * @file prompt_dao.hpp
 * @brief Holds Binary funcations for reading MENUPROMPTS.DAT Original OBV/2
 */
class PromptDao
{
public:
    
    explicit PromptDao()
    { }

    ~PromptDao()
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
                std::cout << "Error writing " << filename << std::endl;
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

    /*
    void exportToXML()
    {
        // Testing!
        TextPrompt tp;
        PromptDao  prompt;
        CommonIO   common_io;

        //m_session_data->deliver(m_session_io.pipe2ansi("|CS|07"));
        std::string filename = "PROMPTS.DAT";


        // Loop each Option after Reading the Menu.
        int u = 0;

        std::string path = GLOBAL_DATA_PATH;
        path.append("\\");
        path.append("prompts.xml");
        std::ofstream ofs(path);

        while(prompt.recordRead(&tp, filename, u++))
        {
            // Convert Pascal to C Strings.
            common_io.PascalToCString(tp.Desc);
            common_io.PascalToCString(tp.Prompt);

            ofs << "<prompt>" << std::endl;
            ofs << "<id>" << u << "</id>" << std::endl;
            ofs << "<description>" << tp.Desc << "</description>" << std::endl;
            ofs << "<text>" << tp.Prompt << "</text>" << std::endl;
            ofs << "</prompt>" << std::endl;
            //if (u == 25) break;
        }
        ofs.close();
    }*/
};

#endif // PROMPT_DAO_HPP
