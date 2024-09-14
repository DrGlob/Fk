//
//  main.cpp
//  QQQQQ
//
//  Created by Gleb Izotov on 14.09.2024.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <math.h>

double roundN(double& n) {
    return floor(n*100)/100.0;
}

using namespace std;
class Currency {
private:
    std::pair<double, double>* course;
    std::pair<std::string, std::string> exc;
public:
    Currency(string cur1, string cur2, double cour) {
        exc.first = cur1;
        exc.second = cur2;
        course = new std::pair<double, double>(floor(cour * 100)/100.0, floor(1/cour * 100)/100.0);
    }

    void rateUpdate() {
        double emb = (double)std::rand()/RAND_MAX*(10.0) - 5;
        course->first = floor((course->first * (100+emb)/100.0)*100)/100.0;
        course->second = floor((course->second * 1/course->first)*100)/100.0;
    }
    void print() {
        cout <<exc.first << " " << exc.second << " " << course->first << "\n";
    }
    double returnMoney(std::string s1, std::string s2, double con) {
        if (exc.first == s1 && exc.second == s2) {
            return floor(con*course->first * 100)/100.0;
        } else if (exc.first == s2 && exc.second == s1) {
            return floor(con*course->second * 100)/100.0;
        } else {
            return -1.0;
        }
    }
};

class Bank {
private:
    std::vector<Currency> allAvaiblePairs;
//    unordered_map<string, double> currs;
public:
    
//    bool availableCurrencyInPair(std::string s) {
//        for (int i = 0; i < allAvaiblePairs.size(); i++) {
//            if (allAvaiblePairs[i].exc.first == s) {
//                
//            } else if (allAvaiblePairs[i].exc.second == s) {
//                
//            }
//        }
//        return 0;
//    }
    void addNewPair(Currency newCur) {
        allAvaiblePairs.push_back(newCur);
    }
    void printAllPairs() {
        for (int i = 0; i < allAvaiblePairs.size(); i++) {
            allAvaiblePairs[i].print();
        }
    }
    void changeCourse() {
        for (int i = 0; i < allAvaiblePairs.size(); i++) {
            allAvaiblePairs[i].rateUpdate();
        }
    }
    double exchange(std::string s1, std::string s2, double con) {
        for (int i = 0; i < allAvaiblePairs.size(); i++) {
            double newCon = floor(allAvaiblePairs[i].returnMoney(s1, s2, con)*100)/100.0;
            if (newCon > 0) {
                changeCourse();
                return newCon;
            }
        }
        changeCourse();
        return -1;
    }
};

class Client {
    unordered_map<string, double> currsIn;
public:
    void add(string cur, double count) {
        currsIn[cur] = count;
    }
    void change(string cur, double e) {
        currsIn[cur] = floor((currsIn[cur]+e)*100)/100.0;
    }
};

class ATD : public Bank {
private:
    Client Cl;
    unordered_map<string, double>* currs;
public:
    ATD(){
        currs = new unordered_map<string, double>;
    }

    
    bool checkBalance(string cur, double i) {
        
        if ((*currs)[cur] >= i) {
            return 1;
        }
        cout << "Не достаточно средств в банкомате, простите за неудобства(:\n";
        return 0;
    }
    
    bool checkCurrencyCorrrct(std::string s) {
        if ((*currs).contains(s)) {
            return 1;
        }
        return 0;
    }
    
    double exchCourse(std::string s1, std::string s2, double con) {
        double count = floor(exchange(s1, s2, con)*100)/100.0;
//        cout << "\n" << count << "\n";
        if (checkBalance(s2, count) > 0) {
            cout << "Вы получили: ";
            (*currs)[s1] += con;
            (*currs)[s2] -= count;
            Cl.change(s2, count);
            Cl.change(s1, -con);
            changeCourse();
            return count;
//            printBalance();
        }
        changeCourse();
        return 0;
    }
    void addMopney(std::string s1, double con) {
        (*currs)[s1] = con;
    }
    void printBalance() {
        for (auto& [s, c] : (*currs)) {
            cout << s << " " << c << "\n";
        }
    }
    
};

int main(int argc, const char * argv[]) {
    
    Currency RU = Currency("RUB", "USD", 90.0);
    Currency RE = Currency("RUB", "EUR", 87.0);
    Currency UE = Currency("USD", "EUR", 0.75);
    Currency UUT = Currency("USD", "USDT", 1);
    Currency UB = Currency("USD", "BTC", 1000.0);
    ATD atd;
    atd.addNewPair(RU);
    atd.addNewPair(RE);
    atd.addNewPair(UE);
    atd.addNewPair(UUT);
    atd.addNewPair(UB);
    atd.addMopney("RUB", 10000.0);
    atd.addMopney("USD", 1000.0);
    atd.addMopney("EUR", 1000.0);
    atd.addMopney("USDT", 1000.0);
    atd.addMopney("BTC", 1.5);
    
    while (1) {
        std::cout << "Доступные валютные пары для обмена:\n";
        atd.printAllPairs();
        string s1, s2;
        double x;
        while (1) {
            cout << "Выберите валюту и сумму перевода: ";
            cin >> s1 >> x;
            if (!atd.checkCurrencyCorrrct(s1)) {
                cout << "Некорректный ввод валюты\n";
            } else {
                break;
            }
        }
        while (1) {
            cout << "Выберите валюту в которую хотите перевести: ";
            cin >> s2;
            if (!atd.checkCurrencyCorrrct(s2)) {
                cout << "Некорректный ввод валюты\n";
            } else {
                break;
            }
        }
        double c = atd.exchCourse(s1, s2, x);
        if (c > 0) {
            cout << c << "\n";
        }
        cout << "Провести ещё один обмен? ";
        std::string checkForContinue;
        while (1) {
            cin >> checkForContinue;
            if (checkForContinue == "YES") {
                break;
            } else if (checkForContinue == "NO") {
                cout << "OFF";
                return 0;
            } else {
                cout << "Введите YES/NO: ";
            }
        }
        atd.printBalance();
    }
    return 0;
}
