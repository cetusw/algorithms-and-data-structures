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

struct Tree
{
    std::string data;
    Tree *father;
    Tree *left, *right;
    Tree() : data(""), father(nullptr), left(nullptr), right(nullptr) {}
};

void printDown(const Tree *p, const int level)
{
    if (p == nullptr)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        std::cout << '.';
    }
    std::cout << p->data << std::endl;

    printDown(p->left, level + 1);
    printDown(p->right, level + 1);
}


int findLowestPriorityOperatorIndex(const std::string &expression)
{
    int lowestPriorityIndex = -1;
    int lowestPriority = 3;
    int parenthesesLevel = 0;

    for (int i = 0; i < expression.size(); ++i)
    {
        const char ch = expression[i];

        if (ch == '(')
        {
            ++parenthesesLevel;
        }
        if (ch == ')')
        {
            --parenthesesLevel;
        }

        if (parenthesesLevel == 0)
        {
            int currentPriority = 3;

            if (ch == '+' || ch == '-')
            {
                currentPriority = 1;
            }
            else if (ch == '^')
            {
                currentPriority = 2;
            }

            if (currentPriority < lowestPriority)
            {
                lowestPriority = currentPriority;
                lowestPriorityIndex = i;
            }
        }
    }

    return lowestPriorityIndex;
}

std::string createSubstring(const std::string &str, const int start, const int end)
{
    std::string substr;
    for (int i = start; i <= end; ++i)
    {
        substr += str[i];
    }

    return substr;
}

Tree* buildExpressionTree(const std::string &expression)
{
    if (expression[0] == '(' && expression[expression.length() - 1] == ')')
    {
        bool hasOuterParentheses = true;
        int parenthesesLevel = 0;

        for (int i = 0; i < expression.length(); ++i)
        {
            if (expression[i] == '(')
            {
                ++parenthesesLevel;
            }
            if (expression[i] == ')')
            {
                --parenthesesLevel;
            }

            if (parenthesesLevel == 0 && i < expression.length() - 2)
            {
                hasOuterParentheses = false;
                break;
            }
        }

        if (hasOuterParentheses)
        {
            return buildExpressionTree(createSubstring(expression, 1, static_cast<int>(expression.length() - 1)));
        }
    }

    const int index = findLowestPriorityOperatorIndex(expression);

    if (index == -1)
    {
        Tree* node = new Tree();
        node->data = expression[0];
        return node;
    }

    Tree* node = new Tree();
    node->data = std::string(1, expression[index]);

    const std::string leftExpression = createSubstring(expression, 0, index - 1);
    const std::string rightExpression = createSubstring(expression, index + 1, static_cast<int>(expression.length()) - 1);

    node->left = buildExpressionTree(leftExpression);
    node->right = buildExpressionTree(rightExpression);

    return node;
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
            std::string expression;
            std::getline(inputFile, expression);
            const Tree* root = buildExpressionTree(expression);
            printDown(root, 0);
        }
        else
        {
            std::cerr << "Ошибка при открытии файла\n" << std::flush;
        }
    }
}