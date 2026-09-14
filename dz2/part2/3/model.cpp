#include "model.h"

using namespace std;

RumourModel::RumourModel() : rng(random_device{}()) {}

string RumourModel::outcomeName(Outcome o) {
    switch (o) {
        case RETURN_TO_FIRST: return "returned to first";
        case REPEAT:          return "repeated to someone";
        case OK:              return "ok";
    }
    return "?";
}

Outcome RumourModel::run() {
    // Всего людей n + 1, индексы 0..n
    // Человек 0 - тот, кто узнал новость первым
    int total = n + 1;

    // known[i] = true, если человек i уже знает новость
    vector<bool> known(total, false);
    known[0] = true;

    // "Активные" - те, кто на текущем шаге передаёт новость
    vector<int> active;
    active.push_back(0);

    // Выполняется r передач
    for (int step = 0; step < r; step++) {
        vector<int> nextActive;

        if (mode == ONE) {
            // Один человек передаёт одному случайно выбранному из n
            for (int from : active) {
                // Случайно выбирается получатель среди n людей (кроме самого from)
                int to;
                do {
                    to = rng() % total;
                } while (to == from);

                if (to == 0) {
                    // Вернулось к первому
                    return RETURN_TO_FIRST;
                }
                if (known[to]) {
                    // Уже знал
                    return REPEAT;
                }
                known[to] = true;
                nextActive.push_back(to);
            }
        } else {
            // Групповой режим: каждый активный передаёт N случайно выбранным
            for (int from : active) {
                for (int k = 0; k < N; k++) {
                    // Случайный получатель среди всех кроме from
                    int to;
                    do {
                        to = rng() % total;
                    } while (to == from);

                    if (to == 0) return RETURN_TO_FIRST;
                    if (known[to]) return REPEAT;

                    known[to] = true;
                    nextActive.push_back(to);
                }
            }
        }

        active = nextActive;
        if (active.empty()) break;
    }

    return OK;
}