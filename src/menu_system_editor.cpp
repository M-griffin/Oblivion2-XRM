#include "menu_system.hpp"
#include "model-sys/struct_compat.hpp"
#include "directory.hpp"

#include <stdint.h>
#include <string>
#include <vector>

/**
 * @Brief This Is an Extension of the Menu System to include but branch off the editor
 * Note, not yet setup for .YAML Files, needs some rework.
 */


/**
 * @brief Menu Editor, Read and Modify Menus
 * Remake of the orignal Menu Editor Screen
 */
std::string MenuBase::displayOfMenus()
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
   
    std::vector<std::string> result_set = m_directory->getFileListPerDirectory(GLOBAL_MENU_PATH, "yaml");

    // check result set, if no menu then return gracefully.
    if(result_set.size() == 0)
    {
        std::cout << "\r\n*** No Menus .yaml files found!" << std::endl;
        return "No Menu Files found!";
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
    std::vector<std::string>::iterator i = result_set.begin();
    std::string menu_name;
    std::string buffer = "\r\n";
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "   "; // 3 Leading spaces per row.
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
                        menu_name = i->substr(0, i->size()-5);
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
    
    buffer += "A/dd Menu C/hange Menu D/elete Menu Q/uit : ";
    //std::string prompt_buffer = "A/dd Menu C/hange Menu D/elete Menu Q/uit : ";
    return (buffer);    
}

