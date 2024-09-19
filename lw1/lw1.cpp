// Условие:
// Вступительные испытания в некоторый вуз состоят из  трех 
// экзаменов:  математика  (максимальный  балл  – 9),  информатика 
// (максимальный балл – 9), литература  (максимальный балл – 5). В
// текстовом вход имеются  сведения  о  сдаче  абитуриентами  этих
// экзаменов. Строки файла имеют следующий формат:
// <Фамилия> <оценка1> <оценка2> <оценка3>, где <Фамилия> –строка,
// состоящая не более, чем из 20 символов, оценки – числа от 0  до
// максимальной оценки по предмету соответственно. Ноль ставится в
// случае, если экзамен не сдавался, например, после полученной на
// предыдущем экзамене двойки. Все  баллы,  большие  2,  считаются 
// удовлетворительными). Пример входных строк:
// Иванов 8 9 3
// Петров 2 0 0
// Задается количество мест K,  на которые претендуют абитуриенты.
// Требуется определить список абитуриентов с полупроходным баллом
// или  сообщить,  что  такой   балл   отсутствует.  Полупроходным 
// считается такой балл, что лишь  часть  абитуриентов,  набравших 
// этот балл и не получивших ни одной неудовлетворительной оценки,
// попадает в K лучших, которые должны быть зачислены

// Автор: Кугелев Михаил

// Среда выполнения:
// g++ (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0
// Copyright (C) 2022 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define minGrade 2
#define maxGradeFirst 9
#define maxGradeSecond 5

struct PassedApplicant 
{
    std::string surname;
    int grade;           
};

struct SemiPassingApplicant 
{
    std::string surname;
    int grade;  
};

std::string readGrades(std::ifstream &inputFile, std::vector<PassedApplicant> &passedApplicants)
{
    std::string surname; 
    int grade;
    std::string line;

    while (std::getline(inputFile, line)) 
    {
        std::istringstream stream(line);
        stream >> surname;

        int gradeSum = 0;
        int countGrade = 0;
        std::string state = "passable";

        while (stream >> grade)
        {
            countGrade++;
            
            if (countGrade < 3 && grade > maxGradeFirst)
            {
                state = "incorrect-values";
            }
            else if (countGrade == 3 && grade > maxGradeSecond) 
            {
                state = "incorrect-values";
            } 
            else if (countGrade > 3)
            {
                state = "overflow";
            }
            
            if (grade <= minGrade) 
            {
                state = "impassable";
                break; 
            }

            gradeSum += grade;
        }

        if (countGrade < 3 && state != "impassable")
        {
            state = "shortfall";
        }

        if (state == "passable" && countGrade == 3) 
        {
            passedApplicants.push_back({surname, gradeSum});
        }
        else if (state != "impassable") 
        {
            std::cout << "Абитуриент " << surname << ":" << std::endl;
            return state;
        }
    }

    return "ok";
}

void sortStudents(std::vector<PassedApplicant> &passedApplicants) 
{
    const size_t n = passedApplicants.size();
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - 1 - i; j++) 
        {
            if (passedApplicants[j].grade < passedApplicants[j + 1].grade) 
            {
                PassedApplicant temp = passedApplicants[j];
                passedApplicants[j] = passedApplicants[j + 1];
                passedApplicants[j + 1] = temp;
            }
        }
    }
}

void findSemiPassingApplicants(const std::vector<PassedApplicant> &passedApplicants, std::vector<SemiPassingApplicant> &semiPassingApplicants, int currentStudentIndex)
{
    if (!passedApplicants.empty())
    {
        const int semiPassingGrade = passedApplicants[currentStudentIndex].grade;
        const size_t numberOfStudents = passedApplicants.size();

        if (passedApplicants[currentStudentIndex + 1].grade == semiPassingGrade) 
        {
            for (int i = 0; i < numberOfStudents; i++) 
            {
                if (passedApplicants[i].grade == semiPassingGrade)
                {
                    semiPassingApplicants.push_back({passedApplicants[i].surname, passedApplicants[i].grade});
                }
            }
        }    
    }    
}

void showListOfSemiPassing(std::vector<SemiPassingApplicant> &semiPassingApplicants)
{
    if (semiPassingApplicants.size() > 1) 
    {
        std::cout << "Список абитуриентов с полупроходным баллом: " << std::endl;
        for (const auto &semiPassingApplicant : semiPassingApplicants) 
        {
            std::cout << " " << semiPassingApplicant.surname << " " << semiPassingApplicant.grade << std::endl;
        }  
        std::cout << std::endl;
    }    
    else
    {
        std::cout << "Нет студентов с полупроходным баллом!\n" << std::endl;      
    }
}

void showListOfApplicants(std::vector<PassedApplicant> &passedApplicants)
{
    if (!passedApplicants.empty())
    {
        std::cout << "Список абитуриентов, прошедших вступительные экзамены: " << std::endl;
        for (const auto &passedApplicant : passedApplicants) 
        {
            std::cout << " " << passedApplicant.surname << " " << passedApplicant.grade << std::endl;
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Нет студентов прошедших вступительные экзамены!\n" << std::endl;      
    }    
}

int main() 
{
    while (true)
    {
        std::string fileName;
        int places;

        std::cout << "Введите имя файла (exit, чтобы выйти): ";
        std::cin >> fileName;   
        
        if (fileName == "exit") 
        {
            break;
        }

        std::ifstream inputFile("../" + fileName);

        std::cout << "Введите количество мест: ";
        std::cin >> places;  
        std::cout << std::endl;

        if (inputFile.is_open()) 
        {
            std::vector<PassedApplicant> passedApplicants;
            std::vector<SemiPassingApplicant> semiPassingApplicants;
            std::string readState;
            readState = readGrades(inputFile, passedApplicants);
            if (readState == "incorrect-values")
            {
                std::cerr << "Некорректные значения баллов\n" << std::endl;
                continue;
            }
            if (readState == "overflow")
            {
                std::cerr << "У абитуриента больше трёх баллов\n" << std::endl;
                continue;
            }
            if (readState == "shortfall")
            {
                std::cerr << "У абитуриента меньше трёх баллов\n" << std::endl;
                continue;
            }
            sortStudents(passedApplicants);
            showListOfApplicants(passedApplicants);
            findSemiPassingApplicants(passedApplicants, semiPassingApplicants, places - 1);
            showListOfSemiPassing(semiPassingApplicants);
            inputFile.close();
        } 
        else 
        {
            std::cerr << "Не удалось открыть файл" << std::endl;
        }
    }
}