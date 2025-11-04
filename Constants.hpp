#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace json::syntax
{
    const char OBJECT_START = '{';
    const char OBJECT_END = '}';
    const char PAIR_SEP = ',';
    const char NAME_VAL_SEP = ':';
    const char NAME_DELIM = '"';
    const char STRING_DELIM = '"';
    const char ESCAPE_SEQUENCE = '\\';
    const char ARRAY_START = '['; 
    const char ARRAY_END = ']'; 
    const char ARRAY_VAL_SEP = ',';


    inline bool IsWhitespace(char c)
    {
        return c == ' '
            || c == '\r'
            || c == '\n'
            || c == '\t';
    }

    inline bool IsNumberStartChar(char c)
    {
        switch (c)
        {
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': return true;
        default: return false;
        }
    }

    inline bool IsDigit(char c)
    {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': return true;
        default: return false;
        }
    }

    inline bool IsNonZero(char c)
    {
        switch (c)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': return true;
        default: return false;
        }
    }
} // namespace json

#endif