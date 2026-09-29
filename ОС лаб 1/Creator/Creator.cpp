#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc != 3) {
        std::cerr << "Использование: Creator <имя файла> <кол-во записей>" << std::endl;
        return 1;
    }

    const char* fileName = argv[1];
    int count = atoi(argv[2]);

    if (count <= 0) {
        std::cerr << "Количество записей должно быть > 0." << std::endl;
        return 1;
    }

    std::ofstream out(fileName, std::ios::binary);
    if (!out) {
        std::cerr << "Не удалось создать файл: " << fileName << std::endl;
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        employee emp;
        std::cout << "Запись " << (i + 1) << std::endl;

        std::cout << "  Номер сотрудника: ";
        std::cin >> emp.num;

        std::cout << "  Имя сотрудника: ";
        std::cin >> emp.name;  

        std::cout << "  Отработано часов: ";
        std::cin >> emp.hours;

        out.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    out.close();
    std::cout << "Файл \"" << fileName << "\" создан. Записей: " << count << std::endl;
    return 0;
}