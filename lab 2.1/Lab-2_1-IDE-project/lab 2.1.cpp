#include <iostream>
#include <deque>
#include <string>
#include <fstream>   
#include <sstream>  
#include <locale>  
#include <codecvt>
#include <algorithm> 
#include <windows.h>  

class ActivityBase {
public:
    virtual ~ActivityBase() = default;

    virtual std::wstring getName() const = 0;
    virtual int getDuration() const = 0;
    virtual std::wstring toString() const = 0;

    static std::wstring wstring_to_utf8(const std::wstring& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(str);
    }
};

class StandardActivity : public ActivityBase {
public:
    StandardActivity(const std::wstring& name = L"", int duration = 0)
        : name(name), duration(duration) {}

    std::wstring getName() const override { return name; }
    int getDuration() const override { return duration; }

    std::wstring toString() const override {
        std::wostringstream oss;
        oss << name << L"," << duration;
        return oss.str();
    }

    static StandardActivity fromString(const std::wstring& str) {
        std::wistringstream iss(str);
        std::wstring name;
        int duration = 0;
        std::getline(iss, name, L',');
        if (!(iss >> duration)) {
            duration = 0;
        }
        return StandardActivity(name, duration);
    }

private:
    std::wstring name;
    int duration;
};

class SportActivity : public ActivityBase {
public:
    SportActivity(const std::wstring& name = L"", int duration = 0, const std::wstring& sportType = L"")
        : name(name), duration(duration), sportType(sportType) {}

    std::wstring getName() const override { return name; }
    int getDuration() const override { return duration; }
    std::wstring getSportType() const { return sportType; }

    std::wstring toString() const override {
        std::wostringstream oss;
        oss << name << L"," << duration << L"," << sportType;
        return oss.str();
    }

    static SportActivity fromString(const std::wstring& str) {
        std::wistringstream iss(str);
        std::wstring name, sportType;
        int duration = 0;
        std::getline(iss, name, L',');
        if (!(iss >> duration)) {
            duration = 0;
        }
        std::getline(iss, sportType, L',');
        return SportActivity(name, duration, sportType);
    }

private:
    std::wstring name;
    int duration;
    std::wstring sportType;
};

class ActivityTracker {
public:
    void addActivity(ActivityBase* activity) {
        activities.emplace_back(activity);
    }

    void removeActivity(const std::wstring& name) {
        auto it = std::remove_if(activities.begin(), activities.end(),
            [&name](const std::unique_ptr<ActivityBase>& activity) {
                return activity->getName() == name;
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
            std::wcout << L"Активність: " << activity->getName()
                << L", Тривалість: " << activity->getDuration() << L" хв." << std::endl;
        }
    }

    int getTotalDuration() const {
        int total = 0;
        for (const auto& activity : activities) {
            total += activity->getDuration();
        }
        return total;
    }

    void saveToFile(const std::wstring& filename) const {
        std::wofstream ofs(ActivityBase::wstring_to_utf8(filename));
        ofs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (!ofs) {
            std::wcerr << L"Помилка відкриття файлу для запису." << std::endl;
            return;
        }
        for (const auto& activity : activities) {
            ofs << activity->toString() << std::endl;
        }
    }

    void loadFromFile(const std::wstring& filename) {
        std::wifstream ifs(ActivityBase::wstring_to_utf8(filename));
        ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (!ifs) {
            std::wcerr << L"Помилка відкриття файлу для читання." << std::endl;
            return;
        }
        activities.clear();
        std::wstring line;
        while (std::getline(ifs, line)) {
            if (!line.empty()) {
                if (line.find(L",") != std::wstring::npos && line.find(L",", line.find(L",") + 1) != std::wstring::npos) {
                    activities.push_back(std::make_unique<SportActivity>(SportActivity::fromString(line)));
                }
                else {
                    activities.push_back(std::make_unique<StandardActivity>(StandardActivity::fromString(line)));
                }
            }
        }
    }

private:
    std::deque<std::unique_ptr<ActivityBase>> activities;
};

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001); 
    std::wcin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    ActivityTracker tracker;
    int choice;
    std::wstring name, sportType;
    int duration;
    std::wstring filename = L"C:\\Users\\TYMCHYK\\Downloads\\Activity.txt";

    tracker.loadFromFile(filename);

    while (true) {
        std::wcout << L"\nМеню:\n";
        std::wcout << L"1. Додати стандартну активність\n";
        std::wcout << L"2. Додати спортивну активність\n";
        std::wcout << L"3. Видалити активність\n";
        std::wcout << L"4. Показати активності\n";
        std::wcout << L"5. Підрахувати загальну тривалість\n";
        std::wcout << L"6. Зберегти активності у файл\n";
        std::wcout << L"7. Вийти\n";
        std::wcout << L"Оберіть опцію: ";
        std::wcin >> choice;
        std::wcin.ignore();

        switch (choice) {
        case 1:
            std::wcout << L"Введіть назву активності: ";
            std::getline(std::wcin, name);
            std::wcout << L"Введіть тривалість активності (в хвилинах): ";
            std::wcin >> duration;
            tracker.addActivity(new StandardActivity(name, duration));
            break;

        case 2:
            std::wcout << L"Введіть назву спортивної активності: ";
            std::getline(std::wcin, name);
            std::wcout << L"Введіть тривалість активності (в хвилинах): ";
            std::wcin >> duration;
            std::wcout << L"Введіть тип спорту: ";
            std::wcin.ignore();
            std::getline(std::wcin, sportType);
            tracker.addActivity(new SportActivity(name, duration, sportType));
            break;

        case 3:
            std::wcout << L"Введіть назву активності для видалення: ";
            std::getline(std::wcin, name);
            tracker.removeActivity(name);
            break;

        case 4:
            tracker.displayActivities();
            break;

        case 5:
            std::wcout << L"Загальна тривалість активностей: "
                << tracker.getTotalDuration() << L" хв." << std::endl;
            break;

        case 6:
            tracker.saveToFile(filename);
            std::wcout << L"Активності збережено у файл " << filename << L"." << std::endl;
            break;

        case 7:
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
