#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <vector>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    setlocale(LC_ALL, "Russian");

    std::string binFileName;
    int count;

    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binFileName;

    std::cout << "Введите количество записей: ";
    std::cin >> count;

    std::string creatorCmd = "Creator.exe " + binFileName + " " + std::to_string(count);

    STARTUPINFOA siCreator;
    PROCESS_INFORMATION piCreator;
    ZeroMemory(&siCreator, sizeof(siCreator));
    siCreator.cb = sizeof(siCreator);
    ZeroMemory(&piCreator, sizeof(piCreator));

    std::vector<char> creatorCmdLine(creatorCmd.begin(), creatorCmd.end());
    creatorCmdLine.push_back('\0');

    if (!CreateProcessA(
        NULL,
        creatorCmdLine.data(),
        NULL, NULL, FALSE, 0, NULL, NULL,
        &siCreator, &piCreator))
    {
        std::cerr << "Ошибка запуска Creator: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(piCreator.hProcess, INFINITE);

    DWORD creatorExitCode = 0;
    GetExitCodeProcess(piCreator.hProcess, &creatorExitCode);
    CloseHandle(piCreator.hProcess);
    CloseHandle(piCreator.hThread);

    if (creatorExitCode != 0) {
        std::cerr << "Creator завершился с ошибкой. Код: " << creatorExitCode << std::endl;
        return 1;
    }

    std::cout << "\nСодержимое бинарного файла \"" << binFileName << "\n";
    std::ifstream in(binFileName, std::ios::binary);
    if (!in) {
        std::cerr << "Не удалось открыть бинарный файл." << std::endl;
        return 1;
    }

    employee emp;
    std::cout << std::left
        << std::setw(12) << "Номер"
        << std::setw(15) << "Имя"
        << std::setw(12) << "Часы" << std::endl;

    while (in.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << std::left
            << std::setw(12) << emp.num
            << std::setw(15) << emp.name
            << std::setw(12) << std::fixed << std::setprecision(2) << emp.hours
            << std::endl;
    }
    in.close();

    std::string reportFileName;
    double payPerHour;

    std::cout << "\nВведите имя файла отчета: ";
    std::cin >> reportFileName;

    std::cout << "Введите оплату за час работы: ";
    std::cin >> payPerHour;

    std::string reporterCmd = "Reporter.exe " + binFileName + " " + reportFileName + " " + std::to_string(payPerHour);

    STARTUPINFOA siReporter;
    PROCESS_INFORMATION piReporter;
    ZeroMemory(&siReporter, sizeof(siReporter));
    siReporter.cb = sizeof(siReporter);
    ZeroMemory(&piReporter, sizeof(piReporter));

    std::vector<char> reporterCmdLine(reporterCmd.begin(), reporterCmd.end());
    reporterCmdLine.push_back('\0');

    if (!CreateProcessA(
        NULL,
        reporterCmdLine.data(),
        NULL, NULL, FALSE, 0, NULL, NULL,
        &siReporter, &piReporter))
    {
        std::cerr << "Ошибка запуска Reporter: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(piReporter.hProcess, INFINITE);

    DWORD reporterExitCode = 0;
    GetExitCodeProcess(piReporter.hProcess, &reporterExitCode);
    CloseHandle(piReporter.hProcess);
    CloseHandle(piReporter.hThread);

    if (reporterExitCode != 0) {
        std::cerr << "Reporter завершился с ошибкой. Код: " << reporterExitCode << std::endl;
        return 1;
    }

    std::cout << "\nСодержимое отчета \"" << reportFileName << "\n";
    std::ifstream report(reportFileName);
    if (!report) {
        std::cerr << "Не удалось открыть файл отчета." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(report, line)) {
        std::cout << line << std::endl;
    }
    report.close();

    std::cout << "\nРабота программы Main завершена." << std::endl;
    return 0;
}