#include "util_dir.hpp"

#include <algorithm>

namespace {
    std::string toLower(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
    }

    bool matchesExtension(const std::filesystem::path &path,
                          const std::string &extension) {
        if (extension.empty()) {
            return true;
        }
        return path.extension() == extension;
    }

    bool matchesExtensionCaseInsensitive(const std::filesystem::path &path,
                                         const std::string &extension) {
        return toLower(path.extension().string()) ==
               toLower(extension);
    }
} // namespace


std::vector<std::string> UtilDir::listFiles(const std::filesystem::path &directory,
                                            const std::string &extension) {
    std::vector<std::string> files;

    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
        return files;
    }

    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (!matchesExtension(entry.path(), extension)) {
            continue;
        }

        files.emplace_back(entry.path().filename().string());
    }

    return files;
}

std::vector<std::filesystem::path> UtilDir::listWithFullPath(const std::filesystem::path &directory,
                                                             const std::string &extension) {
    std::vector<std::filesystem::path> files;

    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
        return files;
    }

    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (!matchesExtension(entry.path(), extension)) {
            continue;
        }

        files.emplace_back(entry.path());
    }

    return files;
}

std::vector<std::filesystem::path> UtilDir::listCaseInsensitive(const std::filesystem::path &directory,
                                                                const std::string &extension) {
    std::vector<std::filesystem::path> files;

    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
        return files;
    }

    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (!matchesExtensionCaseInsensitive(entry.path(), extension)) {
            continue;
        }

        files.emplace_back(entry.path());
    }

    return files;
}

std::vector<std::filesystem::path> UtilDir::listRecursive(const std::filesystem::path &directory,
                                                          const std::string &extension) {
    std::vector<std::filesystem::path> files;

    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
        return files;
    }

    for (const auto &entry:
         std::filesystem::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (!matchesExtension(entry.path(), extension)) {
            continue;
        }

        files.emplace_back(entry.path());
    }

    return files;
}

std::vector<std::filesystem::path> UtilDir::listRecursiveWithSymlinks(const std::filesystem::path &directory,
                                                                      const std::string &extension) {
    std::vector<std::filesystem::path> files;

    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
        return files;
    }

    auto options = std::filesystem::directory_options::follow_directory_symlink;

    for (const auto &entry:
         std::filesystem::recursive_directory_iterator(directory, options)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (!matchesExtension(entry.path(), extension)) {
            continue;
        }

        files.emplace_back(entry.path());
    }

    return files;
}

std::vector<std::filesystem::path> UtilDir::listNoThrow(const std::filesystem::path &directory,
                                                        const std::string &extension,
                                                        std::error_code &ec) {
    std::vector<std::filesystem::path> files;

    ec.clear();

    std::filesystem::directory_iterator it(directory, ec);
    if (ec) {
        return files;
    }

    for (const auto &entry: it) {
        if (entry.is_regular_file(ec) && !ec) {
            if (matchesExtension(entry.path(), extension)) {
                files.emplace_back(entry.path());
            }
        }
        if (ec) {
            break;
        }
    }

    return files;
}
