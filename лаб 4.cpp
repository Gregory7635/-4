#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

class String {
protected:
    char* str;
    int length;

public:
    String() : str(nullptr), length(0) {}

    String(const char* s) {
        length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
    }

    String(char c) {
        length = 1;
        str = new char[2];
        str[0] = c;
        str[1] = '\0';
    }

    void clear() {
        if (str != nullptr) {
            delete[] str;
            str = nullptr;
            length = 0;
        }
    }

    virtual ~String() {
        clear();
    }

    int getLength() const {
        return length;
    }

    const char* getStr() const {
        return str;
    }

    // Оператор присваивания
    String& operator=(const String& other) {
        if (this != &other) {
            clear();
            length = other.length;
            str = new char[length + 1];
            strcpy(str, other.str);
        }
        return *this;
    }

    // Оператор сложения
    unsigned sum(unsigned int a, unsigned int b) {
        unsigned s = 0;
        unsigned carryin = 0;
        unsigned k = 1;
        unsigned temp_a = a;
        unsigned temp_b = b;

        while (temp_a || temp_b) {
            unsigned ak = a & k, bk = b & k;
            unsigned carryout = (ak & bk) | (ak & carryin) | (bk & carryin);

            s |= (ak ^ bk ^ carryin);
            carryin = carryout << 1;
            k <<= 1;

            temp_a >>= 1;
            temp_b >>= 1;
        }

        return s | carryin;
    }

    // Оператор сравнения на равенство
    bool operator==(const String& other) const {
        if (length != other.length) {
            return false;
        }

        for (int i = 0; i < length; ++i) {
            if (str[i] != other.str[i]) {
                return false;
            }
        }

        return true;
    }
};

class BitString : public String {
public:
    BitString(const char* s) : String() {
        int len = strlen(s);
        bool valid = true;
        for (int i = 0; i < len; ++i) {
            if (s[i] != '0' && s[i] != '1') {
                valid = false;
                break;
            }
        }

        if (valid) {
            length = len;
            str = new char[length + 1];
            strcpy(str, s);
        }
        else {
            clear(); // Освобождаем память, если строка недопустима
        }
    }

    ~BitString() {}

    void changeSign() {
        for (int i = 0; i < length; ++i) {
            str[i] = (str[i] == '0') ? '1' : '0';
        }
    }

    // Переопределение оператора присваивания
    BitString& operator=(const BitString& other) {
        if (this != &other) {
            String::operator=(other);
        }
        return *this;
    }


    BitString operator+(const BitString& other) {
        int maxLength = std::max(length, other.length);
        char* result = new char[maxLength + 1];

        char carry = '0';
        for (int i = maxLength - 1; i >= 0; --i) {
            char bit1 = (i < length) ? str[i] : '0';
            char bit2 = (i < other.length) ? other.str[i] : '0';

            if (bit1 == '0' && bit2 == '0') {
                result[i] = carry;
                carry = '0';
            }
            else if ((bit1 == '0' && bit2 == '1') || (bit1 == '1' && bit2 == '0')) {
                result[i] = (carry == '1') ? '0' : '1';
            }
            else {
                result[i] = carry;
                carry = '1';
            }
        }

        result[maxLength] = '\0';
        return BitString(result);
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    BitString validBitString("01010101");
    cout << "Valid BitString: " << validBitString.getStr() << endl;

   
    // Тестирование операций
    BitString bitString1("1010");
    BitString bitString2("0110");


    BitString bitString3("1010");
    cout << "BitString1 and BitString3 are equal: " << (bitString1 == bitString3) << endl;
    BitString invalidBitString("01234567");
    const int arraySize = 5; // Размер массива указателей
    String* stringArray[arraySize]; // Массив указателей на базовый класс

    // Диалог с пользователем для заполнения массива указателями на производные классы
    cout << "Введите строки для создания экземпляров производных классов:" << endl;
    for (int i = 0; i < arraySize; ++i) {
        char input[100]; // Буфер для ввода строки
        cout << "Строка " << i + 1 << ": ";
        cin >> input;

        // Создание экземпляра производного класса и запись указателя в массив
        stringArray[i] = new BitString(input);
    }

    // Проверка всех методов для созданных экземпляров и вывод результатов
    for (int i = 0; i < arraySize; ++i) {
        cout << "Экземпляр " << i + 1 << ":" << endl;
        cout << "Строка: ";
        const char* str = stringArray[i]->getStr();
        if (str) {
            cout << str << endl;
        }
        else {
            cout << "Недопустимая строка" << endl;
        }
        cout << "Длина строки: " << stringArray[i]->getLength() << endl;
        cout << endl;
    }

    // Освобождение памяти, выделенной для экземпляров производных классов
    for (int i = 0; i < arraySize; ++i) {
        delete stringArray[i];
    }

    cout << "Invalid BitString: " << invalidBitString.getStr() << endl;
    return 0;
}
