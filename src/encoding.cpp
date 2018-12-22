#include "encoding.hpp"

#include <string>
#include <vector>
#include <locale>
#include <map>
#include <clocale>  // locale
#include <cwchar>   // wchar_t wide characters
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#endif


/**
 * CP437 -> UTF-8 Character Translation Table
 */

/**
 * @brief  Global Singleton Instance, needed to initalize the class.
 */
Encoding* Encoding::m_global_encoding_instance = nullptr;

#ifdef TARGET_OS_MAC
const std::string Encoding::ENCODING_TEXT_UTF8  = "en_US.UTF-8";
#else
const std::string Encoding::ENCODING_TEXT_UTF8  = "en_US.utf8";
#endif
const std::string Encoding::ENCODING_TEXT_CP437 = "CP437";

static std::map<wchar_t, uint8_t> map_wide_to_cp437;

// UCS2 Table Taranslations.
wchar_t CP437_TABLE[] =
{
    L'\u0000', L'\u263A', L'\u263B', L'\u2665', L'\u2666', L'\u2663',
    L'\u2660', L'\u2022', L'\u0008', L'\u0009', L'\u000A', L'\u2642',
    L'\u2640', L'\u000D', L'\u266C', L'\u263C', L'\u25BA', L'\u25C4',
    L'\u2195', L'\u203C', L'\u00B6', L'\u00A7', L'\u25AC', L'\u21A8',
    L'\u2191', L'\u2193', L'\u2192', L'\u001B', L'\u221F', L'\u2194',
    L'\u25B2', L'\u25BC', L'\u0020', L'\u0021', L'\u0022', L'\u0023',
    L'\u0024', L'\u0025', L'\u0026', L'\u0027', L'\u0028', L'\u0029',
    L'\u002A', L'\u002B', L'\u002C', L'\u002D', L'\u002E', L'\u002F',
    L'\u0030', L'\u0031', L'\u0032', L'\u0033', L'\u0034', L'\u0035',
    L'\u0036', L'\u0037', L'\u0038', L'\u0039', L'\u003A', L'\u003B',
    L'\u003C', L'\u003D', L'\u003E', L'\u003F', L'\u0040', L'\u0041',
    L'\u0042', L'\u0043', L'\u0044', L'\u0045', L'\u0046', L'\u0047',
    L'\u0048', L'\u0049', L'\u004A', L'\u004B', L'\u004C', L'\u004D',
    L'\u004E', L'\u004F', L'\u0050', L'\u0051', L'\u0052', L'\u0053',
    L'\u0054', L'\u0055', L'\u0056', L'\u0057', L'\u0058', L'\u0059',
    L'\u005A', L'\u005B', L'\u005C', L'\u005D', L'\u005E', L'\u005F',
    L'\u0060', L'\u0061', L'\u0062', L'\u0063', L'\u0064', L'\u0065',
    L'\u0066', L'\u0067', L'\u0068', L'\u0069', L'\u006A', L'\u006B',
    L'\u006C', L'\u006D', L'\u006E', L'\u006F', L'\u0070', L'\u0071',
    L'\u0072', L'\u0073', L'\u0074', L'\u0075', L'\u0076', L'\u0077',
    L'\u0078', L'\u0079', L'\u007A', L'\u007B', L'\u007C', L'\u007D',
    L'\u007E', L'\u007F', L'\u00C7', L'\u00FC', L'\u00E9', L'\u00E2',
    L'\u00E4', L'\u00E0', L'\u00E5', L'\u00E7', L'\u00EA', L'\u00EB',
    L'\u00E8', L'\u00EF', L'\u00EE', L'\u00EC', L'\u00C4', L'\u00C5',
    L'\u00C9', L'\u00E6', L'\u00C6', L'\u00F4', L'\u00F6', L'\u00F2',
    L'\u00FB', L'\u00F9', L'\u00FF', L'\u00D6', L'\u00DC', L'\u00A2',
    L'\u00A3', L'\u00A5', L'\u20A7', L'\u0192', L'\u00E1', L'\u00ED',
    L'\u00F3', L'\u00FA', L'\u00F1', L'\u00D1', L'\u00AA', L'\u00BA',
    L'\u00BF', L'\u2310', L'\u00AC', L'\u00BD', L'\u00BC', L'\u00A1',
    L'\u00AB', L'\u00BB', L'\u2591', L'\u2592', L'\u2593', L'\u2502',
    L'\u2524', L'\u2561', L'\u2562', L'\u2556', L'\u2555', L'\u2563',
    L'\u2551', L'\u2557', L'\u255D', L'\u255C', L'\u255B', L'\u2510',
    L'\u2514', L'\u2534', L'\u252C', L'\u251C', L'\u2500', L'\u253C',
    L'\u255E', L'\u255F', L'\u255A', L'\u2554', L'\u2569', L'\u2566',
    L'\u2560', L'\u2550', L'\u256C', L'\u2567', L'\u2568', L'\u2564',
    L'\u2565', L'\u2559', L'\u2558', L'\u2552', L'\u2553', L'\u256B',
    L'\u256A', L'\u2518', L'\u250C', L'\u2588', L'\u2584', L'\u258C',
    L'\u2590', L'\u2580', L'\u03B1', L'\u00DF', L'\u0393', L'\u03C0',
    L'\u03A3', L'\u03C3', L'\u00B5', L'\u03C4', L'\u03A6', L'\u0398',
    L'\u03A9', L'\u03B4', L'\u221E', L'\u03C6', L'\u03B5', L'\u2229',
    L'\u2261', L'\u00B1', L'\u2265', L'\u2264', L'\u2320', L'\u2321',
    L'\u00F7', L'\u2248', L'\u00B0', L'\u2219', L'\u00B7', L'\u221A',
    L'\u207F', L'\u00B2', L'\u25A0', L'\u00A0'
};


Encoding::Encoding()
{
    // Populate UCS2 to CP437 Translation Mapping.
    for(unsigned int char_value = 0; char_value < 256; char_value++)
    {
        map_wide_to_cp437.insert(std::make_pair(CP437_TABLE[char_value], char_value));
    }
}

Encoding::~Encoding()
{
}


/**
 * Windows apperently needs to use it's own WINAPI methods for Wide to Multi-bytes translations
 * Where as Linux can do it with default c++ libs and settin the locale.
 *
 */
#ifdef _WIN32

// Convert an UTF8 string to a wide Unicode String
std::wstring Encoding::multibyte_to_wide(const char* mbstr)
{
    int str_size = strlen(mbstr);

    if(str_size == 0) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &mbstr[0], str_size, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &mbstr[0], str_size, &wstrTo[0], size_needed);
    return wstrTo;
}

// Convert a wide Unicode string to an UTF8 string
std::string Encoding::wide_to_multibyte(const std::wstring &wstr)
{
    if(wstr.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

#elif TARGET_OS_MAC

std::wstring Encoding::multibyte_to_wide(const char* mbstr) 
{
	/*
	std::string incoming_data = std::string(
                                    reinterpret_cast<const char *>(mbstr),
                                    strlen((const char *)mbstr)
                                );*/	
								
    // the UTF-8 / UTF-16 standard conversion facet
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;    
	
	std::wstring ucs2 = L"";
    try 
	{
        ucs2 = utf16conv.from_bytes(mbstr);	
    } 
	catch(const std::range_error& e) 
	{
        std::cout << "UCS2 failed after producing " << std::dec << ucs2.size()<<" characters:\n";        
    }
	
	return ucs2;
}

std::string Encoding::wide_to_multibyte(const std::wstring& wstr) 
{	
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;	
	
	std::string utf8 = "";
    try 
	{
        utf8 = ucs2conv.to_bytes(wstr);	
    } 
	catch(const std::range_error& e) 
	{
        std::cout << "UTF8 failed after producing " << std::dec << utf8.size()<<" characters:\n";        
    }
	return utf8;
}

#else

/**
 * @brief Used for printing output multibyte (Unicode Translations)
 * @param wide_string
 */
// Multi-Byte to WIDE (UTF-8 to UTF-16/UCS2)
std::wstring Encoding::multibyte_to_wide(const char* mbstr)
{
    //std::setlocale(LC_ALL, "");
    std::setlocale(LC_ALL, Encoding::ENCODING_TEXT_UTF8.c_str());
    //std::locale::global(std::locale(Encoding::ENCODING_TEXT_UTF8.c_str()));
    std::cout.imbue(std::locale());

    std::wstring result = L"";
    std::mbstate_t state = std::mbstate_t();
    std::size_t len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
    std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);

    for(unsigned int i = 0; i < wstr.size(); i++)
    {
        //std::wcout << "Wide string: " << wstr[i] << '\n'
        //           << "The length, including '\\0': " << wstr.size() << '\n';
        result += wstr[i];
    }

    return result;
}

/**
 * @brief Used for printing output multibyte (Unicode Translations)
 * @param wide_string
 */

// Wide To Multi-Byte (UTF-16/UCS2 to UTF-8)
std::string Encoding::wide_to_multibyte(const std::wstring &wide_string)
{
    //std::setlocale(LC_ALL, "");
    std::setlocale(LC_ALL, Encoding::ENCODING_TEXT_UTF8.c_str());
    //std::locale::global(std::locale(Encoding::ENCODING_TEXT_UTF8.c_str()));
    std::cout.imbue(std::locale());

    std::string output = "";

    std::mbstate_t state = std::mbstate_t();

    for(wchar_t wc : wide_string)
    {
        std::string mb(MB_CUR_MAX, '\0');
        int ret = std::wcrtomb(&mb[0], wc, &state);

        if(ret == 0)
        {
            break;
        }

        // Skip any Trailing / Embedded null from Wide -> multibtye
        // Conversion, don't send NULL's to the screen.
        for(char ch: mb)
        {
            if(ch != '\0')
            {
                //std::cout << ch << flush;
                output += ch;
            }
        }
    }

    return output;
}

#endif

/**
 * @brief Translation from CP437 to UTF-8 MultiByte Characters
 * @param standard_string
 */
std::string Encoding::utf8Encode(const std::string &standard_string)
{
    std::string output = "";
    std::wstring wide_string = L"";
    int ascii_value = 0;

    // Loop and write out after translation to Unicode
    for(std::string::size_type i = 0; i < standard_string.size(); i++)
    {
        ascii_value = std::char_traits<char>().to_int_type(standard_string[i]);

        if(ascii_value < 256)
        {
            wide_string += CP437_TABLE[ascii_value];
        }
        else
        {
            wide_string += standard_string[i];
        }
    }

    output += wide_to_multibyte(wide_string);
    return output;
}

/**
 * @brief Translation from CP437 to UTF-8 MultiByte Characters
 * @param standard_string
 */
std::string Encoding::utf8Decode(const std::string &standard_string)
{
    std::string output = "";
    const std::wstring wide_string = multibyte_to_wide(standard_string.c_str());

    // loop through UCS2/16 Bit and convert to Single Byte CP437.
    unsigned char c = '\0';

    for(wchar_t usc2_char : wide_string)
    {
        if(map_wide_to_cp437.find(usc2_char) == map_wide_to_cp437.end())
        {
            // Null Characters should be excluded, extra byes in transformation to USC2/UTF-16.
            if (usc2_char == '\0')
            {
                continue;
            }

            std::wcout << "Invalid usc2_char: " << usc2_char << " : " << std::hex << usc2_char << std::endl;
            c  = '?';
        }
        else
            c = map_wide_to_cp437.find(usc2_char)->second;

        output += std::string(1, c);
    }

    return output;
}
