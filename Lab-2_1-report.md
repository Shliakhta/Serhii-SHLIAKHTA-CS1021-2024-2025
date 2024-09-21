<h3 style="text-align: center;">
Міністерство Освіти і Науки України</h3>
<h3 style="text-align: center;">
Тернопільський національний технічний університет імені Івана Пулюя</h3>

<br><br><br>

<p style="text-align: right;">
Факультет комп’ютерно-інформаційних систем</p>
<p style="text-align: right;">
та програмної інженерії</p>
<p style="text-align: right;">
Кафедра “Програмної інженерії”</p>

<br><br><br><br>

<h2 style="text-align: center; font-weight: bold;">
ЗВІТ
</h2>

<p style="text-align: center;">
З лабораторної роботи №1</p>
<p style="text-align: center;">
на тему: "Програмування на основі абстрактних типів даних"</p>
<p style="text-align: center;">
З дисципліни:</p>
<p style="text-align: center;">
"Об'єктно-орієнтоване програмування"</p>

<br><br><br><br><br><br><br><br>

<p style="text-align: right;">
Виконав:</p>
<p style="text-align: right;">
ст. гр. СПс-21</p>
<p style="text-align: right;">
Сергій ШЛЯХТА</p>

<br><br>

<p style="text-align: center;">
Тернопіль 2024</p>

<!--Page Break-->
<div style="page-break-after: always;"></div>


## Мета роботи

Метою лабораторної роботи є розробка застосунку на основі бібліотеки STL (Standard Template Library) для роботи з контейнерами та алгоритмами. Студентам слід ознайомитися з основними класами бібліотеки, реалізувати функціонал для обробки контейнерів, використовуючи ітератори та стандартні алгоритми.

## Завдання
18) Розробіть програму для стеження за фізичною активністю. Використовуйте клас `std::deque` для створення черги денного плану. Кожен елемент черги повинен містити інформацію про вид фізичної активності та час її проведення. Реалізуйте операції додавання та видалення активностей, виведення списку активностей та підрахунок загального часу активностей.
## Середовище розробки
Visual Studio Code
Microsoft Visual Studio 2022 
IDE CLion 2024.2.1

## Написання програми
```c++
#include <iostream>
#include <deque>
#include <string>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <windows.h>

std::string wstring_to_utf8(const std::wstring& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(str);
}

class Activity {
public:
    Activity(const std::wstring& name = L"", int duration = 0)
        : name(name), duration(duration) {}

    std::wstring getName() const { return name; }
    int getDuration() const { return duration; }

    std::wstring toString() const {
        std::wostringstream oss;
        oss << name << L"," << duration;
        return oss.str();
    }

    static Activity fromString(const std::wstring& str) {
        std::wistringstream iss(str);
        std::wstring name;
        int duration = 0;
        std::getline(iss, name, L',');
        if (!(iss >> duration)) {
            duration = 0;
        }
        return Activity(name, duration);
    }

private:
    std::wstring name;
    int duration;
};

class ActivityTracker {
public:
    void addActivity(const std::wstring& name, int duration) {
        activities.emplace_back(name, duration);
    }

    void removeActivity(const std::wstring& name) {
        auto it = std::remove_if(activities.begin(), activities.end(),
            [&name](const Activity& activity) {
                return activity.getName() == name;
            });
        if (it != activities.end()) {
            activities.erase(it, activities.end());
        }
    }

    void displayActivities() const {
        if (activities.empty()) {
            std::wcout << L"Список активностей порожній." << std::endl;
            return;
        }
        std::wcout << L"Список активностей:" << std::endl;
        for (const auto& activity : activities) {
            std::wcout << L"Активність: " << (activity.getName().empty() ? L"Невідома активність" : activity.getName())
                << L", Тривалість: " << activity.getDuration() << L" хв." << std::endl;
        }
    }

    int getTotalDuration() const {
        int total = 0;
        for (const auto& activity : activities) {
            total += activity.getDuration();
        }
        return total;
    }

    void saveToFile(const std::wstring& filename) const {
        std::wofstream ofs(wstring_to_utf8(filename));
        ofs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (!ofs) {
            std::wcerr << L"Помилка відкриття файлу для запису." << std::endl;
            return;
        }
        for (const auto& activity : activities) {
            ofs << activity.toString() << std::endl;
        }
    }

    void loadFromFile(const std::wstring& filename) {
        std::wifstream ifs(wstring_to_utf8(filename));
        ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (!ifs) {
            std::wcerr << L"Помилка відкриття файлу для читання." << std::endl;
            return;
        }
        activities.clear();
        std::wstring line;
        while (std::getline(ifs, line)) {
            if (!line.empty()) {
                activities.push_back(Activity::fromString(line));
            }
        }
    }

private:
    std::deque<Activity> activities;
};

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    std::wcin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    ActivityTracker tracker;
    int choice;
    std::wstring name;
    int duration;
    std::wstring filename = L"C:\\Users\\TYMCHYK\\Downloads\\Activity.txt";

    tracker.loadFromFile(filename);

    while (true) {
        std::wcout << L"\nМеню:\n";
        std::wcout << L"1. Додати активність\n";
        std::wcout << L"2. Видалити активність\n";
        std::wcout << L"3. Показати активності\n";
        std::wcout << L"4. Підрахувати загальну тривалість\n";
        std::wcout << L"5. Зберегти активності у файл\n";
        std::wcout << L"6. Вийти\n";
        std::wcout << L"Оберіть опцію: ";
        std::wcin >> choice;
        std::wcin.ignore();

        switch (choice) {
        case 1:
            std::wcout << L"Введіть назву активності: ";
            std::getline(std::wcin, name);
            std::wcout << L"Введіть тривалість активності (в хвилинах): ";
            std::wcin >> duration;
            tracker.addActivity(name, duration);
            break;

        case 2:
            std::wcout << L"Введіть назву активності для видалення: ";
            std::getline(std::wcin, name);
            tracker.removeActivity(name);
            break;

        case 3:
            tracker.displayActivities();
            break;

        case 4:
            std::wcout << L"Загальна тривалість активностей: "
                << tracker.getTotalDuration() << L" хв." << std::endl;
            break;

        case 5:
            tracker.saveToFile(filename);
            std::wcout << L"Активності збережено у файл " << filename << L"." << std::endl;
            break;

        case 6:
            tracker.saveToFile(filename);
            std::wcout << L"Зміни збережено у файл " << filename
                << L". Вихід з програми." << std::endl;
            return 0;

        default:
            std::wcout << L"Некоректний вибір, спробуйте знову." << std::endl;
        }
    }

    return 0;
}

```


## Висновки
Під час виконання лабораторної роботи було створено застосунок на основі бібліотеки STL (Standard Template Library) для роботи з контейнерами та алгоритмами. Ознайомився з основними класами бібліотеки, принципами роботи контейнерів, ітераторів та функцій-об'єктів (функторів). Реалізував функціонал для роботи з контейнерами, які містять об'єкти користувацького типу (екземпляри певного класу).