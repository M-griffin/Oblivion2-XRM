#include "config_dao.hpp"
#include "config.hpp"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <iostream>
#include <fstream>
#include <string>

ConfigDAO::ConfigDAO(config_ptr config, std::string path)
    : m_config(config)
    , m_path(path)
{ }

ConfigDAO::~ConfigDAO()
{ }

/**
 * @brief Saves Current Config Settings to XML
 */
void ConfigDAO::saveConfig()
{
    std::string path = m_path;
    path.append("xrm-config.xml");
    std::ofstream ofs(path);

    boost::archive::xml_oarchive oa(ofs);
    oa.register_type<Config>();
    oa << BOOST_SERIALIZATION_NVP(m_config);
}

/**
 * @brief Loads Config Data from XML
 */
void ConfigDAO::loadConfig()
{
    std::string path = m_path;
    path.append("xrm-config.xml");
    std::ifstream ifs(path);

    boost::archive::xml_iarchive ia(ifs);
    ia.register_type<Config>();
    ia >> BOOST_SERIALIZATION_NVP(m_config);
}

