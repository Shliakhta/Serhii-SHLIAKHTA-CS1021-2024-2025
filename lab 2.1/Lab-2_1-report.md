### Міністерство Освіти і Науки України
### Тернопільський національний технічний університет імені Івана Пулюя
<br><br><br>
Факультет комп’ютерно-інформаційних систем <br>
та програмної інженерії <br>
Кафедра “Програмної інженерії” <br>
<br><br><br>
### Звіт
З лабораторної роботи №2.1

на тему: "Контейнери бібліотеки STL"

З дисципліни:

"Об'єктно-орієнтоване програмування"
<br><br><br>
Виконав:

ст. гр. СПc-21

Сергій ШЛЯХТА <br><br>
Варіант 18
<br><br><br><br><br><br>
Тернопіль 2024
<br><br><br><br>

### Мета роботи

- розробка застосунку на основі бібліотеки STL (Standard Template Library) для роботи з контейнерами та алгоритмами. Студентам слід ознайомитися з основними класами бібліотеки, реалізувати функціонал для обробки контейнерів, використовуючи ітератори та стандартні алгоритми.

### Завдання

1. Ієрархія класів:

    o Створити базовий клас з абстрактними методами та властивостями.
    o Розробити принаймні два похідні класи, які успадковують властивості та методи від базового класу.
   
 3. Операції з контейнером:

    o Розробити методи для додавання, видалення та пошуку об'єктів в контейнері.
    
    o Забезпечити можливість сортування об'єктів за різними критеріями.
    
 5. Запис та зчитування з файлу:
    
    o Реалізувати методи для запису та зчитування об'єктів з файлу.
    
Додатково:

4. Функціональні можливості:
   
    o Розширений функціонал, який поліпшує користувацький досвід.
   
    o Використання додаткових бібліотек чи інструментів для розширення функціональності (графіки, робота з базою даних тощо).
   
5. Логування та обробка винятків:
     
    o Додати логування подій та обробку винятків для покращення надійності програми.

### Варіант 18

Розробіть програму для стеження за фізичною активністю. Використовуйте клас `std::deque` для створення черги денного плану. Кожен елемент черги повинен містити інформацію про вид фізичної активності та час її проведення. Реалізуйте операції додавання та видалення активностей, виведення списку активностей та підрахунок загального часу активностей.

### Написання програми

Для реалізації програми було створено три основні класи: ActivityBase, який є абстрактним базовим класом для фізичних активностей, StandardActivity та SportActivity, які представляють стандартну та спортивну активності відповідно. Клас ActivityTracker керує списком активностей за допомогою контейнера std::deque.

Клас ActivityBase містить чисті віртуальні методи для отримання інформації про активність, такі як назва, тривалість та форматування інформації:
```
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
```
Клас StandardActivity є похідним від ActivityBase і реалізує методи для стандартних активностей. Він зберігає назву активності та її тривалість, а також надає методи для форматування та парсингу даних:
```
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
```
Клас SportActivity також є похідним від ActivityBase і реалізує додаткову інформацію про тип спорту:
```
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
```
Клас ActivityTracker управляє чергою фізичних активностей, забезпечуючи функції для додавання, видалення активностей, підрахунку загальної тривалості та збереження/завантаження активностей з файлу:
```
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
```
Основні функціональні можливості
Програма дозволяє користувачам:

1. Додавати активність: користувач вводить назву активності та її тривалість у хвилинах.
2. Видаляти активність: користувач може видалити активність за назвою.
3. Виводити список активностей: виводиться список усіх доданих активностей.
4. Підраховувати загальну тривалість: програма підраховує сумарний час усіх активностей.
5. Зберігати активності у файл: активності зберігаються у текстовий файл.
6. Завантажувати активності з файлу: програма завантажує активності з файлу при запуску.

### Тестування
Програма протестована з використанням різних активностей, таких як біг, плавання та тренування. Тестування включало додавання, видалення активностей, а також збереження і завантаження списку активностей з файлу.

### Завдання на захист


### Висновки
Реалізована програма для стеження за фізичною активністю на основі контейнера std::deque. Програма дозволяє додавати, видаляти активності, зберігати їх у файл та підраховувати загальний час. Були використані сучасні можливості C++ для роботи з файлами та локалями.
