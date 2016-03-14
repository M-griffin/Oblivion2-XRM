#include "config_dao.hpp"
#include "config.hpp"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <mutex>

ConfigDao::ConfigDao(config_ptr config, std::string path)
    : m_config(config)
    , m_path(path)
{ }

ConfigDao::~ConfigDao()
{
    std::cout << "~ConfigDAO" << std::endl;
}

/**
 * @brief Saves Current Config Settings to XML
 */
bool ConfigDao::saveConfig()
{
    std::lock_guard<std::mutex> lock(m);

    std::string path = m_path;
    path.append("xrm-config.xml");
    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "Error config: " << path << std::endl;
        return false;
    }

    // Probably add try catch here!
    boost::archive::xml_oarchive oa(ofs);
    oa.register_type<Config>();
    oa << BOOST_SERIALIZATION_NVP(m_config);
    return true;
}

/**
 * @brief Loads Config Data from XML
 */
bool ConfigDao::loadConfig()
{
    std::string path = m_path;
    path.append("xrm-config.xml");
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        // Save Default
        return saveConfig();
    }

    std::lock_guard<std::mutex> lock(m);

    // Probably add try catch here!
    boost::archive::xml_iarchive ia(ifs);
    ia.register_type<Config>();
    ia >> BOOST_SERIALIZATION_NVP(m_config);
    return true;
}

