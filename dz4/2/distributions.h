#pragma once
#include <random>

// Четыре закона распределения случайной величины s
enum class DistKind { Uniform, Binomial, Geometric, Triangular };

class SDistribution {
public:
    DistKind kind = DistKind::Uniform;

    // Параметры (используются по-разному в зависимости от kind)
    int a = -1, b = 2;   // Uniform, Triangular
    int nBin = 5;        // Binomial
    double p = 0.5;      // Binomial, Geometric
    int maxVal = 10;     // Geometric
    int c = 0;           // Triangular (вершина)

    std::mt19937 rng;

    SDistribution();

    // Одно случайное значение s
    int sample();
};