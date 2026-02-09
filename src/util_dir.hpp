#ifndef UTIL_DIR_HPP
#define UTIL_DIR_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <system_error>

class UtilDir {
public:

    // 1. List filenames only (non-recursive)
    static std::vector<std::string>
    listFiles(const std::filesystem::path &directory,
              const std::string &extension = {});

    // 2. List full paths (non-recursive)
    static std::vector<std::filesystem::path>
    listWithFullPath(const std::filesystem::path &directory,
                          const std::string &extension = {});

    // 3. Case-insensitive extension matching (Windows-style)
    static std::vector<std::filesystem::path>
    listCaseInsensitive(const std::filesystem::path &directory,
                             const std::string &extension);

    // 4. Recursive directory listing
    static std::vector<std::filesystem::path>
    listRecursive(const std::filesystem::path &directory,
                       const std::string &extension = {});

    // 5. Recursive listing with symlink handling
    static std::vector<std::filesystem::path>
    listRecursiveWithSymlinks(const std::filesystem::path &directory,
                                   const std::string &extension = {});

    // 6. Non-throwing variant (returns error code)
    static std::vector<std::filesystem::path>
    listNoThrow(const std::filesystem::path &directory,
                     const std::string &extension,
                     std::error_code &ec);
};

#endif
