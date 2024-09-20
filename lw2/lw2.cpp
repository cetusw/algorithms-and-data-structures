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

#include <iostream>
#include <fstream>
#include <string>

int position = 0;
int line = 1;
int lastPosition = 0;
int lastLine = 1;
bool error = false;

struct Stack
{
    char key{};
    Stack *next{};
};

void push(Stack *&p, const char &element)
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

char top(Stack *&p)
{
    const Stack *top = p;
    if (p == nullptr)
    {
        return '\0';
    }
    return top->key;
}

void writeLastPosition()
{
    lastPosition = position;
    lastLine = line;
}

void checkEndOfLine(const char ch)
{
    if (ch == '\n')
    {
        position = 0;
        line++;
    }
}

void writeOpenBrace(Stack *&p, const char ch1)
{
    if (top(p) == '{' || top(p) == '(')
    {
        push(p, '{');
    }
    else
    {
        std::cout << ch1 << std::flush;
        push(p, '{');
    }
}

void writeCloseBrace(Stack *&p, const char ch1)
{
    if (top(p) == '{')
    {
        pop(p);
        if (top(p) != '{')
        {
            std::cout << ch1 << std::flush;
        }
    }
    else if (top(p) != '(')
    {
        std::cout << ch1 << std::flush;
        std::cout << "\nОшибка на " << line << ":" << position << " : нет соответствующей открывающей скобки для '}'" << std::endl;
        error = true;
    }
}

void writeOpenBracket(Stack *&p, const char ch1)
{
    if (top(p) == '(' || top(p) == '{')
    {
        push(p, '(');
    }
    else
    {
        std::cout << '{' << std::flush;
        push(p, '(');
    }
}

void writeCloseBracket(Stack *&p, const char ch1)
{
    if (top(p) == '(')
    {
        pop(p);
        if (top(p) != '(')
        {
            std::cout << '}' << std::flush;
        }
    }
    else if (top(p) != '{')
    {
        std::cout << "*)" << std::flush;
        std::cout << "\nОшибка на " << line << ":" << position << " : нет соответствующей открывающей скобки для '*)'" << std::endl;
        error = true;
    }
}

bool checkBracesAndBrackets(Stack *&p, std::ifstream& inputFile, const char ch1)
{
    if (ch1 == '{')
    {
        writeLastPosition();
        writeOpenBrace(p, ch1);
        return true;
    }
    if (ch1 == '}')
    {
        writeCloseBrace(p, ch1);
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
            writeLastPosition();
            writeOpenBracket(p, ch1);
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
            writeCloseBracket(p, ch1);
            return true;
        }
        return false;
    }
    checkEndOfLine(ch1);
    std::cout << ch1 << std::flush;
    return false;
}

void formatFile(Stack *&p, std::ifstream& inputFile)
{
    char ch1;
    position = 0;
    line = 1;
    while (inputFile.get(ch1))
    {
        position++;
        checkBracesAndBrackets(p, inputFile, ch1);
        if (error)
        {
            break;
        }
    }

    if (p != nullptr)
    {
        std::cout << "\nОшибка на " << lastLine << ":" << lastPosition << " : не все '{' были закрыты" << std::endl;
        error = true;
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