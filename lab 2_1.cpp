#include <iostream>
#include <deque>
#include <string>
#include <fstream>    // для роботи з файлами
#include <sstream>    // для роботи з рядками
#include <locale>     // для встановлення локалі
#include <codecvt>    // для роботи з кодуванням
#include <windows.h>  // для функцій SetConsoleCP та SetConsoleOutputCP

// Клас для зберігання інформації про активність
class Activity {
public:
    // Конструктор класу, який ініціалізує назву активності та тривалість
    Activity(const std::wstring& name = L"", int duration = 0)
        : name(name), duration(duration) {}

    // Метод для отримання назви активності
    std::wstring getName() const { return name; }

    // Метод для отримання тривалості активності
    int getDuration() const { return duration; }

    // Метод для перетворення інформації про активність у рядок (для запису у файл)
    std::wstring toString() const {
        std::wostringstream oss;
        oss << name << L"," << duration;
        return oss.str();
    }

    // Метод для створення об'єкта активності з рядка (під час читання з файлу)
    static Activity fromString(const std::wstring& str) {
        std::wistringstream iss(str);
        std::wstring name;
        int duration;
        // Читаємо назву до першої коми
        std::getline(iss, name, L',');
        if (name.empty()) {
            name = L"Без назви"; // Якщо назва порожня, задаємо дефолтну назву
        }
        iss >> duration;
        return Activity(name, duration);
    }

private:
    std::wstring name;
    int duration; // тривалість в хвилинах
};

// Клас для керування списком активностей
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
        activities.erase(it, activities.end());
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
        std::wofstream ofs(filename);
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
        std::wifstream ifs(filename);
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
    SetConsoleCP(65001); // Для введення з консолі
    SetConsoleOutputCP(65001); // Для виведення в консоль
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
