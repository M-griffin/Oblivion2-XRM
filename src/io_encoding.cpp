#include "io_encoding.hpp"
#include <utf8.h>
#include <functional>
#include <regex>

IoEncoding::IoEncoding() {
    buildReverseMap();
}

void IoEncoding::buildReverseMap() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (uint16_t i = 0; i < cp437ToUnicode.size(); ++i)
        unicodeToCp437[cp437ToUnicode[i]] = static_cast<uint8_t>(i);
}

bool IoEncoding::isValidUtf8(const std::string &input) {
    return utf8::is_valid(input.begin(), input.end());
}

IoEncoding::CharSet IoEncoding::detectEncoding(const std::string &data) {
    if (isValidUtf8(data))
        return CharSet::UTF8;

    for (unsigned char c: data) {
        if (c >= 0x80) {
            if (unicodeToCp437.find(cp437ToUnicode[c]) != unicodeToCp437.end())
                return CharSet::CP437;
            else
                return CharSet::ISO8859_1;
        }
    }
    return CharSet::UTF8;
}

// ANSI Escape Preserving Helper
/*
 *
std::string Encoding::preserveAnsiConvert(
    const std::string &input,
    const std::function<std::string(const std::string &)> &converter) const {
    std::string output;
    std::regex ansi_regex("(\x1B\\[[0-9;]*[A-Za-z])");
    std::sregex_iterator iter(input.begin(), input.end(), ansi_regex);
    std::sregex_iterator end;

    size_t last_pos = 0;
    while (iter != end) {
        // Convert preceding normal text
        auto match_pos = iter->position();
        if (match_pos > last_pos) {
            output += converter(input.substr(last_pos, match_pos - last_pos));
        }
        // Append ANSI sequence as-is
        output += iter->str();
        last_pos = match_pos + iter->length();
        ++iter;
    }
    // Convert any remaining text
    if (last_pos < input.size())
        output += converter(input.substr(last_pos));

    return output;
}*/

// ANSI Escape Preserving Helper
/* Remove Regex for better Performance.
 */
std::string IoEncoding::preserveAnsiConvert(
    const std::string &input,
    const std::function<std::string(const std::string &)> &converter) const
{
    std::string output;
    std::string buffer; // printable text to convert

    auto flushBuffer = [&]() {
        if (!buffer.empty()) {
            output += converter(buffer);
            buffer.clear();
        }
    };

    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = input[i];

        // ANSI escape sequence
        if (c == 0x1B && i + 1 < input.size() && input[i + 1] == '[') {
            flushBuffer();

            size_t start = i;
            i += 2; // skip ESC [

            while (i < input.size()) {
                unsigned char ch = input[i];
                // ANSI sequences end with A–Z or a–z
                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                    ++i;
                    break;
                }
                ++i;
            }

            output.append(input.substr(start, i - start));
            --i;
            continue;
        }

        // Control characters (C0 + DEL)
        if (c < 0x20 || c == 0x7F) {
            flushBuffer();
            output.push_back(c);
            continue;
        }

        // Printable byte → convert later
        buffer.push_back(c);
    }

    flushBuffer();
    return output;
}

std::string IoEncoding::cp437ToUtf8(const std::string &input) const {
    return preserveAnsiConvert(input, [this](const std::string &s) {
        std::string out;
        for (unsigned char c: s)
            utf8::append(cp437ToUnicode[c], std::back_inserter(out));
        return out;
    });
}

std::string IoEncoding::utf8ToCp437(const std::string &input) const {
    return preserveAnsiConvert(input, [this](const std::string &s) {
        std::string out;
        try {
            utf8::unchecked::iterator<std::string::const_iterator> it(s.begin()), end(s.end());
            for (; it != end; ++it) {
                char32_t cp = *it;
                auto found = unicodeToCp437.find(cp);
                if (found != unicodeToCp437.end())
                    out.push_back(found->second);
                else {
                    out.push_back('?');
                    m_log.log(UtilLog::LogLevel::Debug, "Unmapped Unicode in CP437", cp);
                }
            }
        } catch (...) { m_log.log(UtilLog::LogLevel::Error, "UTF8->CP437 conversion failed"); }
        return out;
    });
}

std::string IoEncoding::iso8859_1ToUtf8(const std::string &input) const {
    return preserveAnsiConvert(input, [](const std::string &s) {
        std::string out;
        for (unsigned char c: s)
            utf8::append(static_cast<char32_t>(c), std::back_inserter(out));
        return out;
    });
}

std::string IoEncoding::utf8ToIso8859_1(const std::string &input) const {
    return preserveAnsiConvert(input, [](const std::string &s) {
        std::string out;
        try {
            utf8::unchecked::iterator<std::string::const_iterator> it(s.begin()), end(s.end());
            for (; it != end; ++it) {
                char32_t cp = *it;
                out.push_back(cp <= 0xFF ? static_cast<char>(cp) : '?');
            }
        } catch (...) {
        }
        return out;
    });
}

std::string IoEncoding::utf8Encode(const std::string &s) const {
    return cp437ToUtf8(s);
}

std::string IoEncoding::utf8Decode(const std::string &s) const {
    return utf8ToCp437(s);
}
