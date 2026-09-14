#pragma once
#include <vector>
#include <string>
#include <random>

// Состояния человека
enum State { HEALTHY = 0, SICK = 1, RECOVERED = 2 };

// Модель распространения заболевания
class EpidemicModel {
public:
    int N = 0;
    std::vector<std::vector<int>> adj;  // список знакомств
    std::vector<int> state;             // текущее состояние людей

    double p1 = 0.3;                    // вероятность заразиться
    double p2 = 0.1;                    // вероятность исцелиться

    int startIndex = 0;                 // индекс первого заражённого
    std::mt19937 rng;

    EpidemicModel();

    // Загрузка данных из файла
    // Формат: N M / M строк "a b" / индекс первого заражённого
    bool load(const std::string& filename);

    // Сброс в начальное состояние (один заражённый = startIndex)
    void reset();

    // Один шаг моделирования
    void step();

    // Служебные
    int count(State s) const;
    bool hasSick() const;

    // Поиски по результатам
    std::vector<int> neverSick() const;
    std::vector<int> recovered() const;
    std::vector<int> recoveredWithNotRecoveredNeighbours() const;
    std::vector<int> healthyWithAllSickNeighbours() const;
};