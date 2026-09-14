#pragma once
#include <vector>
#include <random>
#include <string>

// Режим передачи: по одному человеку или группе из N человек
enum Mode { ONE = 0, GROUP = 1 };

// Тип события, которым завершился эксперимент
enum Outcome {
    RETURN_TO_FIRST = 0,  // сообщение вернулось к тому, кто узнал первым
    REPEAT = 1,           // повторное сообщение кому-либо
    OK = 2                // r раз передано без проблем
};

// Модель распространения слухов
class RumourModel {
public:
    int n = 10;   // всего людей = n + 1
    int r = 5;    // сколько раз новость должна быть передана
    int N = 1;    // размер группы при передаче
    Mode mode = ONE;

    std::mt19937 rng;

    RumourModel();

    // Один эксперимент, возвращает исход
    Outcome run();

    // Строка с описанием исхода
    static std::string outcomeName(Outcome o);
};