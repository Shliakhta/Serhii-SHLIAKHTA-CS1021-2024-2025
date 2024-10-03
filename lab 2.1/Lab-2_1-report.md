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

Для реалізації програми було створено два основні класи: Activity, який представляє фізичну активність, та ActivityTracker, який керує списком активностей за допомогою контейнера std::deque.

Клас Activity містить назву активності та її тривалість. У ньому реалізовані методи для отримання та форматування інформації про активність:
```
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
```
Клас ActivityTracker керує чергою фізичних активностей. Основні функції класу включають додавання, видалення активностей, підрахунок загальної тривалості та збереження/завантаження активностей у файл:
```
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

Для збирання проєкту необхідно використовувати CMake та створити відповідний файл конфігурації для компіляції проєкту, що використовує Clang або інший компілятор з підтримкою стандарту C++17 або вище.

### Висновки
Реалізована програма для стеження за фізичною активністю на основі контейнера std::deque. Програма дозволяє додавати, видаляти активності, зберігати їх у файл та підраховувати загальний час. Були використані сучасні можливості C++ для роботи з файлами та локалями.
