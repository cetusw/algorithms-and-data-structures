// Условие
// При  заданном  четном  N  (N  <=  16)  перечислить  все
// правильные скобочные формы длины N из скобок '(', ')', '[',']',
// '{','}'. Скобки '{}' не должны быть внутри '[]' и '()', а скобки
// '[]' не могут быть внутри '()'. Время счета не более 2 сек. (11).
// Например, для N=4 правильные формы:
// (())
// ()()
// ()[]
// (){}
// [()]
// [[]]
// []()
// [][]
// []{}
// {()}
// {[]}
// {{}}
// {}()
// {}[]
// {}{}

// Автор: Кугелев Михаил ПС-21

// Среда выполнения:
// g++ (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0
// Copyright (C) 2022 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <iostream>
#include <stack>

char s[17]{};

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
        return 'n';
    }
    return top->key;
}

int size(Stack *p)
{
    int count = 0;
    while (p != nullptr)
    {
        count++;
        p = p->next;
    }
    return count;
}

Stack *st = nullptr;

void findAllCombinations(const int& n, const int i = 0) {
    if (i == n) {
        std::cout << s << '\n';
        return;
    }
    if (size(st) < n - i) {
        s[i] = '(';
        push(st, '(');
        findAllCombinations(n, i + 1);
        pop(st);
    }
    if (size(st) < n - i && (size(st) == 0 || top(st) != '(')) {
        s[i] = '[';
        push(st, '[');
        findAllCombinations(n, i + 1);
        pop(st);
    }
    if (size(st) < n - i && (size(st) == 0 || (top(st) != '(' && top(st) != '['))) {
        s[i] = '{';
        push(st, '{');
        findAllCombinations(n, i + 1);
        pop(st);
    }
    if (size(st) != 0) {
        const char t = top(st);
        if (t == '(')
        {
            s[i] = ')';
        }
        else if (t == '[')
        {
            s[i] = ']';
        }
        else if (t == '{')
        {
            s[i] = '}';
        }
        pop(st);
        findAllCombinations(n, i + 1);
        push(st, t);
    }
}

int main()
{
    while (true)
    {
        int n = 0;
        std::clog << "Введите количество скобок (чётное число от 2 до 16): ";
        std::cin >> n;
        if (n < 0 || n > 16 || n % 2 != 0)
        {
            std::clog << "\nНекорректное значение, попробуйте ещё раз";
            continue;
        }
        findAllCombinations(n);
        break;
    }
}
