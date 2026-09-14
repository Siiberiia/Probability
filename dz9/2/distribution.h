#pragma once
#include <vector>
#include <random>
#include <string>

// Дискретная случайная величина: набор (значение, вероятность)
class DiscreteRV {
public:
    std::vector<double> values;   // значения (скорости шага)
    std::vector<double> probs;    // вероятности

    std::mt19937 rng;

    DiscreteRV();

    // Загрузка из текстового файла: строки "значение вероятность"
    bool load(const std::string& filename, std::string& error);

    // Одно случайное значение
    double sample();

    // Нормализация вероятностей (сумма = 1)
    void normalize();
};