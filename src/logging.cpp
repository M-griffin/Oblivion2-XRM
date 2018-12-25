#include "logging.hpp"
#include "model-sys/config.hpp"

#include <yaml-cpp/yaml.h>
#include <string>

Logging* Logging::m_global_logging_instance = nullptr;

Logging::Logging()
{
}

Logging::~Logging()
{
}

void Logging::console_level()
{

}

void Logging::info_level()
{

}

void Logging::debug_level()
{

}

void Logging::error_level()
{

}

void Logging::all_levels()
{

}

void Logging::writeOutYamlLog(std::string path, std::string file_name)
{
    std::string log_file;
    log_file.append(path);
    log_file.append(file_name);

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    out << YAML::Key << "DateTime" << YAML::Value << "10pm etc.. ";
    out << YAML::Key << "Detail";
    out << YAML::Value << YAML::BeginSeq << "INFO" << "Testing Info Level" << YAML::EndSeq;

    out << YAML::EndMap;


    // Setup file to Write out File.
    std::ofstream ofs(log_file, std::ios_base::app);

    if(!ofs.is_open())
    {
        std::cout << "Error, unable to open file: " << log_file << std::endl;
        return;
    }

    ofs << out.c_str();
    ofs.close();

    return;

}
