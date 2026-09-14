#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

// ============== Общая модель шифрования ==============
//
// Алфавит: строчные латинские буквы 'a'..'z', код 0..25
// Шифрование: c = (m + k) mod 26   (шифр Виженера для одной буквы)
// Здесь для демонстрации пунктов 1-3 берём маленькое подпространство:
//   T = {"a","b","c"}  с вероятностями
//   K = {"x","y"}      с вероятностями

struct CryptoModel {
    vector<string> texts;         // открытые тексты
    vector<double> pText;         // их вероятности
    vector<string> keys;          // ключи
    vector<double> pKey;          // их вероятности

    // Одна буква -> код
    static int code(char c) { return c - 'a'; }
    static char letter(int x) { return 'a' + (x % 26 + 26) % 26; }

    // Шифрование одной буквы
    static char encrypt(char m, char k) {
        return letter(code(m) + code(k));
    }

    // Расшифрование (для атаки)
    static char decrypt(char c, char k) {
        return letter(code(c) - code(k));
    }

    // Пункт 1: P(Cipher = c)
    double pCipher(char c) const {
        double res = 0.0;
        for (size_t j = 0; j < texts.size(); j++)
            for (size_t i = 0; i < keys.size(); i++)
                if (encrypt(texts[j][0], keys[i][0]) == c)
                    res += pText[j] * pKey[i];
        return res;
    }

    // Пункт 2: P(Cipher = c | M = m)
    double pCipherGivenText(char c, const string& m) const {
        double res = 0.0;
        for (size_t i = 0; i < keys.size(); i++)
            if (encrypt(m[0], keys[i][0]) == c)
                res += pKey[i];
        return res;
    }

    // Пункт 3: P(M = m | Cipher = c) — формула Байеса
    double pTextGivenCipher(const string& m, char c) const {
        double pc = pCipher(c);
        if (pc == 0) return 0.0;
        // P(M=m)
        double pm = 0.0;
        for (size_t j = 0; j < texts.size(); j++)
            if (texts[j] == m) pm = pText[j];
        return pm * pCipherGivenText(c, m) / pc;
    }
};

// ============== Частотный анализ для Виженера ==============
//
// Стандартные частоты букв английского языка (приближение)
static const double ENG_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Найти ключ Виженера для одной позиции по частотному анализу
char crackVigenereShift(const string& cipherLetters) {
    double bestScore = -1e18;
    int bestShift = 0;

    // Перебираются все сдвиги 0..25
    for (int shift = 0; shift < 26; shift++) {
        // Гипотетические частоты открытого текста
        int cnt[26] = {0};
        for (char c : cipherLetters) {
            int m = (c - 'a' - shift + 26) % 26;
            cnt[m]++;
        }
        // Скалярное произведение с эталонными частотами (упрощённое правдоподобие)
        double score = 0;
        for (int i = 0; i < 26; i++) {
            double freq = cnt[i] * 100.0 / cipherLetters.size();
            score += freq * ENG_FREQ[i];
        }
        if (score > bestScore) {
            bestScore = score;
            bestShift = shift;
        }
    }
    return 'a' + bestShift;
}

// Атака на Виженера: перебор длин ключа + частотный анализ по позициям
string attackVigenere(const string& cipher, int maxKeyLen) {
    int bestLen = 1;
    double bestIC = -1;

    // Индекс совпадений для разных длин ключа
    for (int len = 1; len <= maxKeyLen; len++) {
        double ic = 0;
        int total = 0;
        for (int pos = 0; pos < len; pos++) {
            int cnt[26] = {0};
            int n = 0;
            for (int i = pos; i < (int)cipher.size(); i += len) {
                cnt[cipher[i] - 'a']++;
                n++;
            }
            // IC = сумма cnt*(cnt-1) / (n*(n-1))
            for (int i = 0; i < 26; i++)
                ic += cnt[i] * (cnt[i] - 1);
            total += n;
        }
        if (total > 1) {
            ic /= (total * (total - 1.0) / 26.0);
            if (ic > bestIC) { bestIC = ic; bestLen = len; }
        }
    }

    // Ключ по найденной длине
    string key(bestLen, 'a');
    for (int pos = 0; pos < bestLen; pos++) {
        string col;
        for (int i = pos; i < (int)cipher.size(); i += bestLen)
            col += cipher[i];
        key[pos] = crackVigenereShift(col);
    }
    return key;
}

// ============== Моделирование пунктов 1-3 ==============

void demoProbabilityModel() {
    cout << "=== Part 1: probability model (tiny example) ===\n";

    CryptoModel model;
    model.texts = {"a", "b", "c"};
    model.pText = {0.5, 0.3, 0.2};
    model.keys  = {"x", "y"};
    model.pKey  = {0.6, 0.4};

    cout << "Encryption table E(k, m):\n";
    cout << "      ";
    for (auto& m : model.texts) cout << setw(6) << m;
    cout << "\n";
    for (auto& k : model.keys) {
        cout << "k=" << k << ": ";
        for (auto& m : model.texts)
            cout << setw(6) << CryptoModel::encrypt(m[0], k[0]);
        cout << "\n";
    }

    cout << "\nP(Cipher = c):\n";
    for (char c = 'a'; c <= 'e'; c++) {
        double p = model.pCipher(c);
        if (p > 0)
            cout << "  P(C=" << c << ") = " << p << "\n";
    }

    cout << "\nP(Cipher = c | M = m):\n";
    for (auto& m : model.texts) {
        for (char c = 'a'; c <= 'e'; c++) {
            double p = model.pCipherGivenText(c, m);
            if (p > 0)
                cout << "  P(C=" << c << " | M=" << m << ") = " << p << "\n";
        }
    }

    cout << "\nP(M = m | Cipher = c):\n";
    for (char c = 'a'; c <= 'e'; c++) {
        for (auto& m : model.texts) {
            double p = model.pTextGivenCipher(m, c);
            if (p > 0)
                cout << "  P(M=" << m << " | C=" << c << ") = " << p << "\n";
        }
    }
}

// ============== Атака на Виженера ==============

void demoVigenere() {
    cout << "\n=== Part 2: Vigenere cipher attack ===\n";

    // Открытый текст на английском
    string plain =
        "thequickbrownfoxjumpsoverthelazydogandthendoessomethingelse";

    string key = "secret";
    cout << "Plain:  " << plain << "\n";
    cout << "True key: " << key << "\n";

    // Шифрование
    string cipher(plain.size(), ' ');
    for (size_t i = 0; i < plain.size(); i++) {
        int m = plain[i] - 'a';
        int k = key[i % key.size()] - 'a';
        cipher[i] = 'a' + (m + k) % 26;
    }
    cout << "Cipher: " << cipher << "\n";

    // Атака
    string guessed = attackVigenere(cipher, 10);
    cout << "Guessed key: " << guessed << "\n";

    // Расшифровка найденным ключом
    string decrypted(cipher.size(), ' ');
    for (size_t i = 0; i < cipher.size(); i++) {
        int c = cipher[i] - 'a';
        int k = guessed[i % guessed.size()] - 'a';
        decrypted[i] = 'a' + (c - k + 26) % 26;
    }
    cout << "Decrypted: " << decrypted << "\n";
    cout << "Match with original: "
         << (decrypted == plain ? "YES" : "no") << "\n";
}

// ============== Атака на Вернама ==============

void demoVernam() {
    cout << "\n=== Part 3: Vernam cipher (one-time pad) ===\n";

    // Открытый текст
    string plain = "attackatdawn";
    cout << "Plain:  " << plain << "\n";

    // Ключ той же длины, случайный
    mt19937 rng(random_device{}());
    string key(plain.size(), ' ');
    for (size_t i = 0; i < plain.size(); i++)
        key[i] = 'a' + rng() % 26;
    cout << "Key:    " << key << "\n";

    // Шифрование
    string cipher(plain.size(), ' ');
    for (size_t i = 0; i < plain.size(); i++) {
        int m = plain[i] - 'a';
        int k = key[i] - 'a';
        cipher[i] = 'a' + (m + k) % 26;
    }
    cout << "Cipher: " << cipher << "\n";

    // Попытка атаки: перебор всех ключей длины plain.size() невозможен
    // (26^12 ≈ 9.5e16), поэтому демонстрируется статистический анализ:
    // при равномерном ключе распределение шифртекста совпадает с равномерным,
    // и любой открытый текст равновероятен.
    int cnt[26] = {0};
    for (char c : cipher) cnt[c - 'a']++;

    cout << "Cipher letter frequencies (should be near uniform):\n";
    for (int i = 0; i < 26; i++)
        if (cnt[i] > 0)
            cout << "  " << char('a' + i) << ": " << cnt[i] << "\n";

    // Демонстрация абсолютной стойкости:
    // для любого гипотетического открытого текста есть ровно один ключ,
    // который превращает шифртекст в этот текст.
    cout << "\nFor any hypothetical plaintext m, there exists exactly one key k\n"
            "such that E(k, m) = cipher. So P(M=m | C=c) = P(M=m).\n";

    string hyp = "attackatdusk";  // другая гипотеза
    string k2(hyp.size(), ' ');
    for (size_t i = 0; i < hyp.size(); i++) {
        int c = cipher[i] - 'a';
        int m = hyp[i] - 'a';
        k2[i] = 'a' + (c - m + 26) % 26;
    }
    cout << "Hypothetical plaintext: " << hyp << "\n";
    cout << "Key that would produce it: " << k2 << "\n";
    cout << "=> this plaintext is just as likely as any other.\n";
}

// ============== main ==============

int main() {
    cout << fixed << setprecision(4);

    demoProbabilityModel();
    demoVigenere();
    demoVernam();

    return 0;
}