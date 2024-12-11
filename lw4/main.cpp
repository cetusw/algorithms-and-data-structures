// Условие
// Задан взвешенный ориентированный граф. С помощью алгоритма
// Беллмана-Форда найти кратчайший путь между заданными вершинами.
// Проверить наличие циклов отрицательной длины. В случае обнаружения
// вывести один из них (10).

// Автор: Кугелев Михаил ПС-21

// Среда выполнения:
// g++ (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0
// Copyright (C) 2022 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

constexpr int EXPECTED_NUMBER_OF_VALUES_IN_THE_STRING = 3;
long INFINITY = 9999999999999999;

std::vector<std::vector<int>> graph = {};
int vertexes = 0;
bool error = false;

bool isInputStringCorrect(const size_t size)
{
    if (size == EXPECTED_NUMBER_OF_VALUES_IN_THE_STRING)
    {
        return true;
    }
    std::cout << "Неправильно передана таблица, в строке должно быть три значения" << std::endl;
    error = true;
    return false;
}

void calculateNumberOfVertexes(const int &n)
{
    if (n > vertexes)
    {
        vertexes = n;
    }
}

void readGraph(std::ifstream &inputFile)
{
    while (!inputFile.eof())
    {
        std::vector<int> value = {};
        std::string line;
        std::getline(inputFile, line);
        std::istringstream lineStream(line);
        std::string token;

        int i = 0;
        while (lineStream >> token)
        {
            i++;
            value.emplace_back(std::stoi(token));
            if (i < 3)
            {
                calculateNumberOfVertexes(std::stoi(token));
            }
        }

        if (isInputStringCorrect(value.size()))
        {
            graph.emplace_back(value);
        }
    }
}

void printGraph()
{
    for (const std::vector<int> &row : graph)
    {
        for (const int &value : row)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printDistance(const std::vector<int> &distance)
{
    for (int i = 0; i < distance.size(); i++)
    {
        std::cout << i << " " << distance[i] << std::endl;
    }
}

void printDistanceBetween(const std::vector<int> &distance, const int &firstVertex, const int &secondVertex)
{
    if (distance[secondVertex] != INFINITY)
    {
        std::cout << "Кратчайший путь от " << firstVertex << " до " << secondVertex << " вершины: " << distance[secondVertex] << std::endl;
    }
    else
    {
        std::cout << "Нет пути между вершинами " << firstVertex << " и " << secondVertex << std::endl;
        error = true;
    }
}

void handleGraph(std::vector<int> &distance)
{
    for (const std::vector<int> &value : graph)
    {
        if (distance[value[0]] != INFINITY && distance[value[0]] + value[2] < distance[value[1]])
        {
            distance[value[1]] = distance[value[0]] + value[2];
        }
    }
}

void isNegativeCycle(std::vector<int> &distance)
{
    std::vector predecessor(vertexes + 1, -1);

    int lastUpdatedVertex = -1;

    for (int i = 0; i < vertexes; i++)
    {
        lastUpdatedVertex = -1;
        for (const std::vector<int> &value : graph)
        {
            if (distance[value[0]] != INFINITY && distance[value[0]] + value[2] < distance[value[1]])
            {
                distance[value[1]] = distance[value[0]] + value[2];
                predecessor[value[1]] = value[0];
                lastUpdatedVertex = value[1];
            }
        }
    }

    if (lastUpdatedVertex == -1) {
        error = false;
        return;
    }

    for (int i = 0; i < vertexes; ++i) {
        lastUpdatedVertex = predecessor[lastUpdatedVertex];
    }

    std::vector<int> cycle;
    for (int v = lastUpdatedVertex;; v = predecessor[v]) {
        cycle.emplace_back(v);
        if (v == lastUpdatedVertex && cycle.size() > 1) {
            break;
        }
    }

    std::ranges::reverse(cycle);

    std::cout << "Отрицательный цикл найден: ";
    for (const int v : cycle) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    error = true;
}

void findShortestPathBetweenVertexes(const int firstVertex, const int secondVertex)
{
    std::vector<int> distance = {};
    for (int i = 0; i <= vertexes; i++)
    {
        distance.emplace_back(INFINITY);
    }
    distance[firstVertex] = 0;

    for (int i = 0; i < vertexes; i++)
    {
        handleGraph(distance);
    }

    isNegativeCycle(distance);

    if (error)
    {
        return;
    }

    printDistanceBetween(distance, firstVertex, secondVertex);
}
int main()
{
    while (true)
    {
        std::string fileName;

        std::cout << "\nВведите имя файла с графом (exit, чтобы выйти): ";
        std::cin >> fileName;

        if (fileName == "exit")
        {
            break;
        }

        std::ifstream inputFile("../" + fileName);

        if (inputFile.is_open())
        {
            int firstVertex;
            int secondVertex;
            std::cout << "\nВведите номер первой вершины: ";
            std::cin >> firstVertex;
            std::cout << "\nВведите номер второй вершины: ";
            std::cin >> secondVertex;
            readGraph(inputFile);
            if (error)
            {
                continue;
            }
            printGraph();
            findShortestPathBetweenVertexes(firstVertex, secondVertex);
        }
        else
        {
            std::cerr << "Ошибка при открытии файла\n" << std::flush;
        }
    }
}
