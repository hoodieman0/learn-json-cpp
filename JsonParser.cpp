#include "JsonParser.hpp"

JsonParser::JsonParser(std::ifstream& file) : file(file)
{
    ParseJsonFromFile();

    for (; !lex.empty(); lex.pop())
        std::cout << lex.front().Identifier << ',' << lex.front().Value << "\n";
    std::cout << '\n';
    std::cout << std::flush;
}

JsonParser::~JsonParser()
{
    file.close();
}

void JsonParser::ParseJsonFromFile()
{
    ReadObject();
    // while (true)
    // {
    //     int c = (char)file.peek();
    //     if (c == EOF) throw std::exception();
    //     switch (c)
    //     {
    //     case syntax::OBJECT_START: ReadObject();
    //     case syntax::OBJECT_END: file >> c; return; // end
    //     case syntax::PAIR_SEP: file >> c; continue;
    //     case syntax::NAME_DELIM: ReadNameValuePair(); break;            
    //     default:
    //         if (syntax::IsWhitespace((char)c)) continue;
    //         else throw std::exception();
    //     }
    // }
}

void JsonParser::ReadObject()
{
    int c = file.get();
    if ((char) c != syntax::OBJECT_START) throw std::exception();
    lex.push(TokenValue{Token::OBJECT_START, "{"});
    RemoveWhitespace();
    c = file.peek();
    if ((char) c == syntax::OBJECT_END) 
    {
        lex.push(TokenValue{Token::OBJECT_END, "}"});
        return;
    }
    else if ((char) c == syntax::NAME_DELIM)
    {
        ReadNameValuePair();
        c = file.peek();
        while ((char) c == syntax::PAIR_SEP)
        {
            // Name/value pair must follow PAIR_SEP
            file >> c;
            RemoveWhitespace();
            ReadNameValuePair();
        }
        if ((char) c != syntax::OBJECT_END) throw std::exception();
        lex.push(TokenValue{Token::OBJECT_END, "}"});
    }
    else throw std::exception();
}

void JsonParser::ReadArray()
{
    char c;
    file >> c;
    // start of string
    if (c != syntax::ARRAY_START) throw std::exception();
    lex.push(TokenValue{Token::ARRAY_START, "["});

    RemoveWhitespace();
    c = file.peek();
    if (c == syntax::ARRAY_END)
    {
        lex.push(TokenValue{Token::ARRAY_END, "]"});
        return; // empty array
    } 
    else 
    {
        ReadValue();
        c = file.peek();
        while (c == syntax::ARRAY_VAL_SEP)
        {
            file >> c;
            ReadValue();
            c = file.peek();
        }
        if (c != syntax::ARRAY_END) throw std::exception();
        lex.push(TokenValue{Token::ARRAY_END, "]"});
    }
}

// keep reading until the next char is not a JSON whitespace character
void JsonParser::RemoveWhitespace()
{
    int c = file.peek();
    while (c != EOF && syntax::IsWhitespace((char)c))
    {
        file.get();
        c = file.peek();
    }
}

void JsonParser::ReadNameValuePair()
{
    lex.push(TokenValue{Token::NAME, "See next string"});
    ReadString();
    RemoveWhitespace();
    char c;
    file >> c;
    if (c != syntax::NAME_VAL_SEP) throw std::exception();
    ReadValue();
}

void JsonParser::ReadValue()
{
    RemoveWhitespace();

    char c = file.peek();
    switch (c)
    {
    case syntax::STRING_DELIM: ReadString(); break;
    case syntax::OBJECT_START: ReadObject(); break;
    case syntax::ARRAY_START: ReadArray(); break;
    case 't': ReadTrue(); break;
    case 'f': ReadFalse(); break;
    case 'n': ReadNull(); break;
    default:
        if (syntax::IsNumberStartChar(c)) ReadNumber();
        else throw std::exception();
        break;
    }

    RemoveWhitespace();
}

void JsonParser::ReadString()
{
    char c;
    file >> c;
    // start of string
    if (c != syntax::NAME_DELIM) throw std::exception();
    file >> c;

    std::string result;
    while (c != syntax::NAME_DELIM)
    {
        if (c == EOF) throw std::exception();
        if (c == syntax::ESCAPE_SEQUENCE)
            c = ReadDelimitedValue();
        
        result += c;
        file >> c;
    }

    lex.push(TokenValue{Token::STRING, result});
}

// excepts
char JsonParser::ReadDelimitedValue()
{
    char c;
    std::string hex;
    file >> c;
    switch (c)
    {
    case '"': return '"';
    case '\\': return '\\';
    case '/': return '/';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'u': 
        for (int i = 0; i < 4; i++)
        {
            file >> c;
            hex += c;
        }
        return std::stoi(hex, nullptr, 16);
    default: throw std::exception();
    }
}

void JsonParser::ReadNumber()
{
    std::string result = "";
    char c;
    file >> c;
    if (c == '0') 
    {
        result += c;
        c = file.peek();
    }
    else if (c == '-')
    {
        result += c;
        c = file.peek();
        if (!syntax::IsNonZero(c)) throw std::exception();
        while (syntax::IsDigit(c))
        {
            file >> c;
            result += c;
            c = file.peek();
        }
    }
    else if (syntax::IsNonZero(c))
    {
        while (syntax::IsDigit(c))
        {
            file >> c;
            result += c;
            c = file.peek();
        }
    }
    else throw std::exception();

    // fractions
    if (c == '.')
    {
        file >> c;
        result += c;
        c = file.peek();
        while (syntax::IsDigit(c))
        {
            file >> c;
            result += c;
            c = file.peek();
        }
    }

    // exponents
    if (c == 'e' || c == 'E')
    {
        file >> c;
        result += c;
        c = file.peek();

        if (c == '+' || c == '-')
        {
            file >> c;
            result += c;
            c = file.peek();
        }

        while (syntax::IsDigit(c))
        {
            file >> c;
            result += c;
            c = file.peek();
        }
    }

    lex.push(TokenValue{Token::NUMBER, result});
}

void JsonParser::ReadTrue()
{
    std::string read;
    for (int i = 0; i < 4; i++)
    {
        char c;
        file >> c;
        read += c;
    }
    if (read != "true") throw std::exception();
    lex.push(TokenValue{Token::TRUE, "true"});
}

void JsonParser::ReadFalse()
{
    std::string read;
    for (int i = 0; i < 5; i++)
    {
        char c;
        file >> c;
        read += c;
    }
    if (read != "false") throw std::exception();
    lex.push(TokenValue{Token::FALSE, "false"});
}

void JsonParser::ReadNull()
{
    std::string read;
    for (int i = 0; i < 4; i++)
    {
        char c;
        file >> c;
        read += c;
    }
    if (read != "null") throw std::exception();
    lex.push(TokenValue{Token::NNNN, "null"});
}