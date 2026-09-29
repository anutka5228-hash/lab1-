#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc != 4) {
        std::cerr << "Использование: Reporter <исходный файл> <файл отчета> <оплата за час>" << std::endl;
        return 1;
    }

    const char* binFileName = argv[1];
    const char* reportFileName = argv[2];
    double payPerHour = atof(argv[3]);

    std::ifstream in(binFileName, std::ios::binary);
    if (!in) {
        std::cerr << "Не удалось открыть бинарный файл: " << binFileName << std::endl;
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    in.close();

    std::sort(employees.begin(), employees.end(),
        [](const employee& a, const employee& b) {
            return a.num < b.num;
        });

    std::ofstream out(reportFileName);
    if (!out) {
        std::cerr << "Не удалось создать файл отчета: " << reportFileName << std::endl;
        return 1;
    }

    out << "Отчет по файлу \"" << binFileName << "\"" << std::endl;
    out << std::left
        << std::setw(12) << "Номер"
        << std::setw(15) << "Имя"
        << std::setw(12) << "Часы"
        << std::setw(15) << "Зарплата" << std::endl;

    for (const auto& e : employees) {
        double salary = e.hours * payPerHour;
        out << std::left
            << std::setw(12) << e.num
            << std::setw(15) << e.name
            << std::setw(12) << std::fixed << std::setprecision(2) << e.hours
            << std::setw(15) << std::fixed << std::setprecision(2) << salary << std::endl;
    }

    out.close();
    std::cout << "Отчет \"" << reportFileName << "\" создан." << std::endl;
    return 0;
}