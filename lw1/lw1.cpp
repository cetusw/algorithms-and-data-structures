#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

void writeDescription()
{
    std::cout << "Условие: Определить список абитуриентов с полупроходным баллом\nили  сообщить,  что  такой   балл   отсутствует.  Полупроходным\nсчитается такой балл, что лишь  часть  абитуриентов,  набравших\nэтот балл и не получивших ни одной неудовлетворительной оценки,\nпопадает в K лучших, которые должны быть зачислены\n";
    std::cout << "Автор: Михаил Кугелев\n";
    std::cout << "Среда выполнения: Visual Studio Code\n\n";
}

std::string readGrades(std::ifstream &inputFile, std::vector<PassedApplicant> &passedApplicants)
{
    std::string surname;
    int minGrade = 2; 
    int grade;
    int gradeSum;
    int countGrade;
    std::string line;

    while (std::getline(inputFile, line)) 
    {
        std::istringstream stream(line);
        stream >> surname;

        gradeSum = 0;    
        countGrade = 0;
        std::string state = "passable";

        while (stream >> grade)
        {
            countGrade++;
            
            if (countGrade < 3 && grade > 9) 
            {
                state = "incorrect-values";
            } 
            else if (countGrade == 3 && grade > 5) 
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
    int n = passedApplicants.size();
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

void findSemiPassingApplicants(std::vector<PassedApplicant> &passedApplicants, std::vector<SemiPassingApplicant> &semiPassingApplicants, int currentStudentIndex)
{
    if (passedApplicants.size() > 0) 
    {
        int semiPassingGrade = passedApplicants[currentStudentIndex].grade;
        int numberOfStudents = passedApplicants.size();

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
    }    
    else
    {
        std::cout << "Нет студентов с полупроходным баллом!\n" << std::endl;      
    }
}

void showListOfApplicants(std::vector<PassedApplicant> &passedApplicants)
{
    if (passedApplicants.size() > 0)
    std::cout << "Список абитуриентов, прошедших вступительные экзамены: " << std::endl;
    for (const auto &passedApplicant : passedApplicants) 
    {
        std::cout << " " << passedApplicant.surname << " " << passedApplicant.grade << std::endl;
    }
}

int main() 
{
    writeDescription();

    while (true)
    {
        std::vector<PassedApplicant> passedApplicants;
        std::vector<SemiPassingApplicant> semiPassingApplicants;
        std::string fileName;
        std::string readState;
        int places;

        std::cout << "Введите имя файла (exit, чтобы выйти): ";
        std::cin >> fileName;   
        std::ifstream inputFile(fileName);
        
        if (fileName == "exit") 
        {
            break;
        }

        std::cout << "Введите количество мест: ";
        std::cin >> places;  
        std::cout << std::endl;

        if (inputFile.is_open()) 
        {
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