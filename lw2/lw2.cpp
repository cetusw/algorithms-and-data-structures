// Условие
// 21. В   некотором   компиляторе   ПАСКАЛя  текст  программы
// включает примечания,  выделенные  фигурными  скобками  '{','}'
// либо  парами  символов  '(*'  и  '*)'.  Примечания  могут быть
// вложенными друг в друга.  Если примечание открыто знаком  '{',
// то оно должно быть закрыто знаком '}'.  Аналогично примечание,
// начинающееся с символов '(*'  должно  заканчиваться  символами
// '*)'. Требуется:
//    1) проверить правильность вложенности примечаний;
// 2) переписать   файл   с   исходным   текстом   так,  чтобы
// отсутствовала  вложенность  комментариев  при  сохранении   их
// содержания  и  в  качестве  ограничивающих  символов  остались
// только  фигурные  скобки.   Учесть   случай,   когда   символы
// примечаний находятся в апострофах. При некорректности  указать
// номера строки и позиции первой ошибки (10).

// Автор: Кугелев Михаил

// Среда выполнения:
// g++ (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0
// Copyright (C) 2022 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <iostream>
#include <fstream>
#include <string>

int position = 0;
int line = 1;
bool error = false;

struct Stack
{
    std::string key{};
    Stack *next{};
};

void push(Stack *&p, const std::string &element)
{
    const auto top = new Stack;
    top->key = element;
    top->next = p;
    p = top;
}

void pop(Stack *&p)
{
    const Stack *top = p;
    p = p->next;
    delete top;
}

std::string top(Stack *&p)
{
    const Stack *top = p;
    if (p == nullptr)
    {
        return "nullptr";
    }
    return top->key;
}

void clearOutput()
{
    std::ofstream bufFile("../buf.txt", std::ofstream::trunc);

    if (!bufFile.is_open()) {
        std::cerr << "Ошибка открытия буфера!" << std::endl;
    }

    bufFile.close();
}

void writeFromOutput()
{
    std::ifstream outputFile("../output.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "Ошибка открытия буфера!" << std::endl;
        return;
    }

    char ch;
    while (outputFile.get(ch))
    {
        std::cout << ch;
    }

    outputFile.close();
}

void checkEndOfLine(const char ch)
{
    if (ch == '\n')
    {
        position = 0;
        line++;
    }
}

void writeOpenBrace(Stack *&p, std::ofstream& bufFile)
{
    if (top(p) == "\'")
    {
        bufFile << '{';
        return;
    }
    if (top(p) == "{" || top(p) == "(")
    {
        push(p, std::to_string(position));
        push(p, std::to_string(line));
        push(p, "{");

    }
    else
    {
        bufFile << '{';
        push(p, std::to_string(position));
        push(p, std::to_string(line));
        push(p, "{");
    }
}

void writeCloseBrace(Stack *&p, std::ofstream& bufFile)
{
    if (top(p) == "\'")
    {
        bufFile << "}";
        return;
    }
    if (top(p) == "{")
    {
        pop(p);
        pop(p);
        pop(p);
        if (top(p) != "{" && top(p) != "(")
        {
            bufFile << '}';
        }
    }
    else if (top(p) != "(")
    {
        bufFile << '}';
        std::cout << "\nОшибка на " << line << ":" << position << " : нет соответствующей открывающей скобки для '}'" << std::endl;
        error = true;
    }
}

void writeOpenBracket(Stack *&p, std::ofstream& bufFile)
{
    if (top(p) == "\'")
    {
        bufFile << "(*";
        return;
    }
    if (top(p) == "(" || top(p) == "{")
    {
        push(p, std::to_string(position));
        push(p, std::to_string(line));
        push(p, "(");
    }
    else
    {
        bufFile << '{';
        push(p, std::to_string(position));
        push(p, std::to_string(line));
        push(p, "(");
    }
}

void writeCloseBracket(Stack *&p, std::ofstream& bufFile)
{
    if (top(p) == "\'")
    {
        bufFile << "*)";
        return;
    }
    if (top(p) == "(")
    {
        pop(p);
        pop(p);
        pop(p);
        if (top(p) != "(" && top(p) != "{")
        {
            bufFile << '}';
        }
    }
    else if (top(p) != "{")
    {
        bufFile << "*)";
        std::cout << "\nОшибка на " << line << ":" << position << " : нет соответствующей открывающей скобки для '*)'" << std::endl;
        error = true;
    }
}

void writeSingleQuote(Stack *&p)
{
    if (top(p) == "\'")
    {
        pop(p);
    }
    else
    {
        push(p, "\'");
    }
}

bool checkBracesAndBrackets(Stack *&p, std::ifstream& inputFile, std::ofstream& bufFile, const char ch1)
{
    if (ch1 == '{')
    {
        writeOpenBrace(p, bufFile);
        return true;
    }
    if (ch1 == '}')
    {
        writeCloseBrace(p, bufFile);
        return true;
    }
    if (ch1 == '(')
    {
        char ch2;
        inputFile.get(ch2);
        position++;
        checkEndOfLine(ch2);
        if (ch2 == '*')
        {
            writeOpenBracket(p, bufFile);
        }
        else if (ch2 == '(')
        {
            bufFile << ch1;
            checkBracesAndBrackets(p, inputFile, bufFile, ch2);
        }
        else
        {
            bufFile << ch1 << ch2;
        }
        return true;
    }
    if (ch1 == '*')
    {
        char ch2;
        inputFile.get(ch2);
        position++;
        checkEndOfLine(ch2);
        if (ch2 == ')')
        {
            writeCloseBracket(p, bufFile);
        }
        else if (ch2 == '*')
        {
            bufFile << ch1;
            checkBracesAndBrackets(p, inputFile, bufFile, ch2);
        }
        else
        {
            bufFile << ch1 << ch2;
        }
        return false;
    }
    if (ch1 == '\'')
    {
        writeSingleQuote(p);
    }
    checkEndOfLine(ch1);
    bufFile << ch1;
    return false;
}

void formatFile(Stack *&p, std::ifstream& inputFile)
{
    char ch1;
    position = 0;
    line = 1;
    std::ofstream outputFile("../output.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
    }

    while (inputFile.get(ch1))
    {
        position++;
        checkBracesAndBrackets(p, inputFile, outputFile, ch1);
        if (error)
        {
            clearOutput();
            break;
        }
    }

    if (p != nullptr)
    {
        pop(p);
        std::cout << "\nОшибка на " << top(p) << ":";
        pop(p);
        std::cout << top(p) << " : не все скобки были закрыты" << std::endl;
        clearOutput();
        error = true;
    }

    outputFile.close();
    if (!error)
    {
        writeFromOutput();
        std::cout << "\n\nОшибок нет, все вложенные скобки были удалены!" << std::endl;
    }
}

int main()
{
    while (true)
    {
        std::string fileName;

        std::cout << "\nВведите имя файла (exit, чтобы выйти): ";
        std::cin >> fileName;

        if (fileName == "exit")
        {
            break;
        }

        std::ifstream inputFile("../" + fileName);

        if (inputFile.is_open())
        {
            Stack *top = nullptr;
            formatFile(top, inputFile);
        }
        else
        {
            std::cerr << "Ошибка при открытии файла\n" << std::flush;
        }
    }

}