// Условие
// 22. Имеется некоторое  алгебраическое выражение.  Операнды
// заданы идентификаторами,  операции выполняются по приоритетам,
// допускается использование круглых скобок. Возможны одноместные
// операции '+' и '-', а также операция возведения в степень '^'.
// Требуется построить и выдать в наглядном виде бинарное дерево,
// представляющее данное выражение (11).

// Автор: Кугелев Михаил ПС-21

// Среда выполнения:
// g++ (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0
// Copyright (C) 2022 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <iostream>
#include <fstream>
#include <string>

const std::string acceptableOperation[3] = {"+", "-", "^"};

const int maxLength = 20;
struct Tree
{
    char name[maxLength];
    Tree *father;
    Tree *left, *right;
};


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

bool contains(const std::string arr[], const int size, const std::string &value) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}

bool isNumber(const std::string& str) {
    for (const char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void readExpression(Stack *&operation, Stack *&operand, std::ifstream& inputFile)
{
    std::string str;
    while (std::getline(inputFile, str, ' '))
    {
        if (str == "(")
        {

        }
        else if (str == ")")
        {

        }
        else if (contains(acceptableOperation, std::size(acceptableOperation), str)
        {

        }
        else if (isNumber(str))
        {

        }
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
            Stack *operation = nullptr;
            Stack *operand = nullptr;
            readExpression(operation, operand, inputFile);

        }
        else
        {
            std::cerr << "Ошибка при открытии файла\n" << std::flush;
        }
    }
}