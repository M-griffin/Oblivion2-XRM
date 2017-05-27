#include "menu_system.hpp"

#include "model-sys/struct_compat.hpp"

#include <boost/locale.hpp>

// Needed for RPI GCC 4.9
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#include <stdint.h>
#include <string>
#include <vector>

/**
 * @Brief This Is an Extension of the Menu System to include but branch off the editor
 */


/**
 * @brief Menu Editor, Read and Modify Menus
 * Remake of the orignal Menu Editor Screen
 */
std::string MenuBase::displayMenus()
{
    // Setup Extended ASCII Box Drawing characters.
    char top_left  = (char)214; // ╓
    char bot_left  = (char)211; // ╙
    char row       = (char)196; // ─
    char top_right = (char)183; // ╖
    char bot_right = (char)189; // ╜

    char mid_top   = (char)210; // ╥
    char mid_bot   = (char)208; // ╨
    char mid       = (char)186; // ║

    namespace fs = boost::filesystem;
    fs::path someDir(GLOBAL_MENU_PATH);   // Add to menu path from config!
    fs::directory_iterator end_iter;

    typedef std::vector<std::string> result_set_t;
    typedef std::vector<std::string>::iterator iterator;
    result_set_t result_set;

    if(fs::exists(someDir) && fs::is_directory(someDir))
    {
        for(fs::directory_iterator dir_iter(someDir); dir_iter != end_iter; ++dir_iter)
        {
            if(dir_iter->path().extension() == ".MNU")
            {
                if(fs::is_regular_file(dir_iter->status()))
                {
                    result_set.push_back(dir_iter->path().filename().string());
                    //result_set_t::value_type(fs::last_write_time( dir_iter->path() ) ) ); // *dir_iter));
                }
            }
        }
    }
    // Sort Menu's in accending order
    std::sort(result_set.begin(), result_set.end());

    // iterate through and print out
    int total_rows = result_set.size() / 8;
    int remainder = result_set.size() % 8;

    // Add for Header and Footer Row!
    total_rows += 2;
    if(remainder > 0)
        ++total_rows;

    // Could re-calc this on screen width lateron.
    int max_cols  = 73; // out of 80

    // Vector or Menus, Loop through
    iterator i = result_set.begin();
    std::string menu_name;
    std::string buffer = "\r\n";
    for(int rows = 0; rows < total_rows; rows++)
    {
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                // std::cout << top_left << std::flush;
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                //std::cout << top_right << std::flush;
                buffer += top_right;
            }
            else if(rows == 0 && cols % 9 == 0)
            {
                //std::cout << mid_top << std::flush;
                buffer += mid_top;
            }
            else if(rows == 0)
            {
                //std::cout << row << std::flush;
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                //std::cout << bot_left << std::flush;
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                //std::cout << bot_right << std::flush;
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 9 == 0)
            {
                //std::cout << mid_bot << std::flush;
                buffer += mid_bot;
            }
            else if(rows == total_rows-1)
            {
                //std::cout << row << std::flush;
                buffer += row;
            }
            else if(cols % 9 == 0)
            {
                //std::cout << mid << std::flush;
                buffer += mid;
            }
            else
            {
                // Here we insert the Menu name and pad through to 8 characters.
                if(cols % 10 == 0 || cols == 1)
                {
                    if(i != result_set.end())
                    {
                        // Strip Extension, then pad 8 characters.
                        menu_name = i->substr(0, i->size()-4);
                        menu_name = m_common_io.rightPadding(menu_name, 8);
                        //std::cout << menu_name << std::flush;
                        buffer += menu_name;
                        ++i;
                    }
                    else
                    {
                        // Empty, 8 Spaces.
                        //std::cout << "        " << std::flush;
                        buffer += "        ";
                    }
                }
            }
        }
        //std::cout << std::endl;
        buffer += "\r\n";
    }

    // Display the prompt, then for wait for next command.
    //std::cout << "A/dd Menu C/hange Menu D/elete Menu Q/uit : " << std::flush;
    buffer += "A/dd Menu C/hange Menu D/elete Menu Q/uit : ";
    return (buffer);
}

