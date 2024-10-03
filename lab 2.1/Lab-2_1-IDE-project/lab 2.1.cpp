#include <iostream>
#include <deque>
#include <string>
#include <fstream>    // ��� ������ � �������
#include <sstream>    // ��� ������ � �������
#include <locale>     // ��� ������������ �����
#include <codecvt>    // ��� ������ � ����������
#include <algorithm>  // ��� std::remove_if
#include <windows.h>  // ��� ������� SetConsoleCP �� SetConsoleOutputCP

// ������� ��� ������������ std::wstring �� std::string � ������������� ������ UTF-8
std::string wstring_to_utf8(const std::wstring& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(str);
}

// ���� ��� ��������� ���������� ��� ���������
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

// ���� ��� ��������� ������� �����������
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
            std::wcout << L"������ ����������� �������." << std::endl;
            return;
        }
        std::wcout << L"������ �����������:" << std::endl;
        for (const auto& activity : activities) {
            std::wcout << L"���������: " << (activity.getName().empty() ? L"������� ���������" : activity.getName())
                << L", ���������: " << activity.getDuration() << L" ��." << std::endl;
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
            std::wcerr << L"������� �������� ����� ��� ������." << std::endl;
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
            std::wcerr << L"������� �������� ����� ��� �������." << std::endl;
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
    SetConsoleCP(65001); // ��� �������� � ������
    SetConsoleOutputCP(65001); // ��� ��������� � �������
    std::wcin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    ActivityTracker tracker;
    int choice;
    std::wstring name;
    int duration;
    std::wstring filename = L"C:\\Users\\TYMCHYK\\Downloads\\Activity.txt";

    tracker.loadFromFile(filename);

    while (true) {
        std::wcout << L"\n����:\n";
        std::wcout << L"1. ������ ���������\n";
        std::wcout << L"2. �������� ���������\n";
        std::wcout << L"3. �������� ���������\n";
        std::wcout << L"4. ϳ��������� �������� ���������\n";
        std::wcout << L"5. �������� ��������� � ����\n";
        std::wcout << L"6. �����\n";
        std::wcout << L"������ �����: ";
        std::wcin >> choice;
        std::wcin.ignore();

        switch (choice) {
        case 1:
            std::wcout << L"������ ����� ���������: ";
            std::getline(std::wcin, name);
            std::wcout << L"������ ��������� ��������� (� ��������): ";
            std::wcin >> duration;
            tracker.addActivity(name, duration);
            break;

        case 2:
            std::wcout << L"������ ����� ��������� ��� ���������: ";
            std::getline(std::wcin, name);
            tracker.removeActivity(name);
            break;

        case 3:
            tracker.displayActivities();
            break;

        case 4:
            std::wcout << L"�������� ��������� �����������: "
                << tracker.getTotalDuration() << L" ��." << std::endl;
            break;

        case 5:
            tracker.saveToFile(filename);
            std::wcout << L"��������� ��������� � ���� " << filename << L"." << std::endl;
            break;

        case 6:
            tracker.saveToFile(filename);
            std::wcout << L"���� ��������� � ���� " << filename
                << L". ����� � ��������." << std::endl;
            return 0;

        default:
            std::wcout << L"����������� ����, ��������� �����." << std::endl;
        }
    }

    return 0;
}
