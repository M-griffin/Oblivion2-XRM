#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <dirent.h>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>


/**
 * There is no Standard Filesystem and directory methods for C++/11
 * At this time, 17 has some new expermental from Boost libs but
 * Most systems are not up to 17 yet.  Were using some C API
 * Which are not thread safe so we'll wrap them un mutex for now.
 */


/**
 * @class directory
 * @author Michael Griffin
 * @date 03/03/2018
 * @file directory.hpp
 * @brief Thread-Safe Directory Class for Parsing all Files in a folder.
 */
class Directory
{
public:
    explicit Directory(void)
        : m() // Mutex
    { }
    ~Directory() {}

    /**
     * @brief Returns the Extension of the current file name
     * @param value
     * @return
     */
    std::string getFileExtension(const std::string &value)
    {
        std::string::size_type idx = value.rfind(".");
        std::string extension = "";
        if (idx != std::string::npos)
        {
            extension = value.substr(idx+1);
        }
        return extension;
    }

    /**
     * @brief Retrieves All files in directory by Extension (ex.. .yaml)
     * @param dir
     * @return
     */
    std::vector<std::string> getAllFilesInDirectoryByExtension(const std::string &dir, std::string extension)
    {
        std::lock_guard<std::mutex> lock(m);

        // Check if were pulling by specific or all files with extensions.
        bool isAllExtenasions = false;
        if (extension.size() > 0)
        {
            isAllExtenasions = true;
        }

        std::vector<std::string> file_list;
        std::shared_ptr<DIR> local_directory_ptr(opendir(dir.c_str()), [] (DIR* directory)
        {
            directory && closedir(directory);
        });

        if (!local_directory_ptr)
        {
            std::cout << "Error dir opening: " << errno << dir << std::endl;
            return file_list;
        }

        struct dirent *dirent_ptr;
        while ((dirent_ptr = readdir(local_directory_ptr.get())) != nullptr)
        {
            // Skip All Directories "., .." in current folder.
            std::string file_name = dirent_ptr->d_name;
            if (file_name[file_name.size()-1] != '.')
            {
                // If File Extension matches then add to the list.
                std::string file_ext = getFileExtension(file_name);
                if (isAllExtenasions && file_ext == extension)
                {
                    // By Specific Extension
                    file_list.push_back(file_name);
                }
                else if(!isAllExtenasions && file_ext != "")
                {
                    // By All Extensions (Skip folders and executables)
                    file_list.push_back(file_name);
                }
            }
        }

        return file_list;
    }

    /**
     * @brief Get File Listing Helper Method
     * @param directory
     * @param extension
     * @return
     */
    std::vector<std::string> getFileListPerDirectory(std::string directory, std::string extension)
    {
        const auto& directory_path = directory;
        const auto& file_list = getAllFilesInDirectoryByExtension(directory_path, extension);
        return file_list;
    }

private:
    mutable std::mutex m;

};

typedef std::shared_ptr<Directory> directory_ptr;

#endif // DIRECTORY_HPP
