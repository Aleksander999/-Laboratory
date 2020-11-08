МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

### Отчёт по лабораторной работе №5<br/> по дисциплине "Программирование"
<br/>

студента 1 курса группы ИВТ-б-0-192(2)
Менжелеев Александр Федорович
направления подготовки 09.03.01 "Информатика и вычислительная техника"  
<br/>

<table>
<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>
<br/><br/>

Симферополь, 2019

<br/><br/>

# Работа с текстовыми файлами

## Цель:
- Научиться работать с текстовыми файлами;
- Закрепить навыки работы со структурами.

## Ход работы

### Исходный код программы


```cpp
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <regex>
#include <iostream>

using namespace std;

struct Passanger {
    uint32_t pId;
    bool survival;
    short pClass;
    string name;
    string sex;
    uint16_t age;
    uint16_t sibsp;
    uint16_t parch;
    string ticket;
    float fare;
    string cabin;
    char embarked;
};

regex parseRegExp("((\".+\")+|[^\",]+|(,,)|(,$)|(^,))", std::regex::ECMAScript);


ifstream inputStream;
ofstream outputStream("source/result.md");

Passanger parseString(string strToParse) {
    smatch matches;

    auto itbegin = sregex_iterator(strToParse.begin(), strToParse.end(), parseRegExp);
    auto itend = std::sregex_iterator();

    vector<string> extracted;

    for (auto it = itbegin; it != itend; ++it){
        if (it->str() == "," || it->str() == ",,")
            extracted.push_back("");
        else
            extracted.push_back(it->str());
    }


    if (extracted.size() != 12)
        throw new std::invalid_argument("Invalid string for parse provided");

    Passanger parsedPassanger{
        atoi(extracted[0].c_str()),
        atoi(extracted[1].c_str()),
        atoi(extracted[2].c_str()),
        extracted[3],
        extracted[4],
        atoi(extracted[5].c_str()),
        atoi(extracted[6].c_str()),
        atoi(extracted[7].c_str()),
        extracted[8],
        atof(extracted[9].c_str()),
        extracted[10],
        extracted[11].c_str()[0]
    };

    return parsedPassanger;
}


template<typename T>
std::string joinStrVector(vector<T> vectorToJoin, std::string delimiter = ", ") {
    std::string t = "";
    for (auto it : vectorToJoin)
        t += to_string(it) + delimiter;
    return t;
}

std::string joinStrVector(vector<std::string> vectorToJoin, std::string delimiter = ", ") {
    std::string t = "";
    for (auto it : vectorToJoin)
        t += it + delimiter;
    return t;
}



int main(int argc, char** argv)
{
    if (argc < 3)
        return EXIT_FAILURE;

    setlocale(LC_ALL, "Russian");

    std::string pathToFile = argv[2];

    inputStream.open(pathToFile);

    if (!inputStream.is_open()) {
        cout << "Файл не был открыт. Проверьте указанный путь";
        return EXIT_FAILURE;
    }

    vector<Passanger> parsedPassanger;

    long counter = 0;

    try {
        std::string headerString;

        // Header line offset
        getline(inputStream, headerString, '\r');
        while (!inputStream.eof()){
            std::string nextString;
            getline(inputStream, nextString, '\r');
            auto nextPassanger = parseString(nextString);
            
            ++counter;
            parsedPassanger.push_back(nextPassanger);
        }
    }
    catch (exception e) {
        cout << "Строка [" << counter << "] не может быть прочтена должным образом.";
    }

    cout << counter << " строк прочтено из файла. Обработка начата..." << endl;

    struct {
        long totalSurv = 0;
        long firstClassSurv = 0;
        long secondClassSurv = 0;
        long thirdClassSurv = 0;
        long womensSurv = 0;
        long mansSurv = 0;
        long womensCount = 0;
        long mansCount = 0;
        struct {
            long passangers = 0;
            long womens = 0;
            long mans = 0;
        }averageAge;
        struct {
            long Cherbourg = 0;
            long Queenstown = 0;
            long Southampton = 0;
        }stateCounter;
        std::vector<long unsigned> underAgeList;
    }statistic;

    for (auto it: parsedPassanger) {
        if (it.survival) {
            ++statistic.totalSurv;
            if (it.pClass == 1)
                ++statistic.firstClassSurv;
            if (it.pClass == 2)
                ++statistic.secondClassSurv;
            if (it.pClass == 3)
                ++statistic.thirdClassSurv;
            if (it.sex == "female")
                ++statistic.womensSurv;
            if (it.sex == "male")
                ++statistic.mansSurv;
        }

        ++statistic.averageAge.passangers += it.age;

        if (it.sex == "female") {
            if (it.age) {
                ++statistic.womensCount;
                statistic.averageAge.womens += it.age;
            }
        }
        if (it.sex == "male") {
            if (it.age) {
                ++statistic.mansCount;
                statistic.averageAge.mans += it.age;
            }
        }

        if (it.age < 18) {
            statistic.underAgeList.push_back(it.pId);
        }

        switch (it.embarked)
        {
        case 'Q':
            ++statistic.stateCounter.Queenstown;
            break;
        case 'C':
            ++statistic.stateCounter.Cherbourg;
            break;
        case 'S':
            ++statistic.stateCounter.Southampton;
            break;
        }
    }

    int stateCounter = 0;
    std::string stateName = "";

    if (stateCounter < statistic.stateCounter.Cherbourg) {
        stateCounter = statistic.stateCounter.Cherbourg;
        stateName = "Cherbourg";
    }
    if (stateCounter < statistic.stateCounter.Queenstown) {
        stateCounter = statistic.stateCounter.Queenstown;
        stateName = "Queenstown";
    }
    if (stateCounter < statistic.stateCounter.Southampton) {
        stateCounter = statistic.stateCounter.Southampton;
        stateName = "Southampton";
    }


    std::vector<std::string> output = {
        "| Характеристика | Результат |",
        "|     :---:      |   :---:   |",
        "| Всего выживших |" + to_string(statistic.totalSurv) + "|",
        "| Выжившие 1 класс |" + to_string(statistic.firstClassSurv) + "|",
        "| Выжившие 2 класс |" + to_string(statistic.secondClassSurv) + "|",
        "| Выжившие 3 класс |" + to_string(statistic.thirdClassSurv) + "|",
        "| Выжившие женщины |" + to_string(statistic.womensSurv) + "|",
        "| Выжившие мужчины |" + to_string(statistic.mansSurv) + "|",
        "| Средний возраст пассажира |" + to_string(statistic.averageAge.passangers / parsedPassanger.size()) + "|",
        "| Средний возраст мужчин |" + to_string(statistic.averageAge.mans / statistic.mansCount) + "|",
        "| Средний возраст женщин |" + to_string(statistic.averageAge.womens / statistic.womensCount) + "|",
        "| Самый частый штат пассажиров | " + stateName + " |",
        "| Список идентификаторов |" + joinStrVector(statistic.underAgeList) + "|"
    };

    for (auto it : output) {
        cout << it << endl;
        outputStream << it << endl;
    }

    system("pause");
}
```

### Результаты обработки

Выходной файл также доступен по [этой ссылке](Code/source/result.md)


| Характеристика | Результат |
|     :---:      |   :---:   |
| Всего выживших |342|
| Выжившие 1 класс |136|
| Выжившие 2 класс |87|
| Выжившие 3 класс |119|
| Выжившие женщины |233|
| Выжившие мужчины |109|
| Средний возраст пассажира |24|
| Средний возраст мужчин |31|
| Средний возраст женщин |28|
| Самый частый штат пассажиров | Southampton |
| Список идентификаторов |6, 8, 10, 11, 15, 17, 18, 20, 23, 25, 27, 29, 30, 32, 33, 37, 40, 43, 44, 46, 47, 48, 49, 51, 56, 59, 60, 64, 65, 66, 69, 72, 77, 78, 79, 83, 85, 87, 88, 96, 102, 108, 110, 112, 115, 120, 122, 126, 127, 129, 139, 141, 148, 155, 157, 159, 160, 164, 165, 166, 167, 169, 172, 173, 177, 181, 182, 183, 184, 185, 186, 187, 194, 197, 199, 202, 206, 209, 215, 221, 224, 230, 234, 236, 238, 241, 242, 251, 257, 261, 262, 265, 267, 271, 275, 278, 279, 283, 285, 296, 298, 299, 301, 302, 304, 305, 306, 307, 308, 325, 330, 331, 334, 335, 336, 341, 348, 349, 352, 353, 355, 359, 360, 365, 368, 369, 375, 376, 382, 385, 387, 389, 390, 408, 410, 411, 412, 414, 416, 420, 421, 426, 429, 432, 434, 436, 445, 446, 447, 449, 452, 455, 458, 460, 465, 467, 469, 470, 471, 476, 480, 481, 482, 486, 490, 491, 496, 498, 501, 503, 505, 508, 512, 518, 523, 525, 528, 531, 532, 533, 534, 536, 539, 542, 543, 548, 550, 551, 553, 558, 561, 564, 565, 569, 574, 575, 579, 585, 590, 594, 597, 599, 602, 603, 612, 613, 614, 619, 630, 634, 635, 640, 643, 644, 645, 649, 651, 654, 657, 668, 670, 675, 681, 684, 687, 690, 692, 693, 698, 710, 712, 719, 721, 722, 728, 732, 733, 739, 740, 741, 747, 751, 752, 756, 761, 765, 767, 769, 774, 777, 778, 779, 781, 782, 784, 788, 789, 791, 792, 793, 794, 803, 804, 814, 816, 820, 825, 826, 827, 828, 829, 831, 832, 833, 838, 840, 842, 845, 847, 850, 851, 853, 854, 860, 864, 869, 870, 876, 879, 889, |

## Вывод

В ходе проделанной работы получен навыки работы с данными, файловыми структурами.  
Получены знания о работе STL-контейнеров и применены на практике.
Отработаны на практие знания о работе регулярных выражений и их имплементации в ЯП C++.
