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