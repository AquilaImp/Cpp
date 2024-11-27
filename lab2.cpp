#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <locale>
#include <windows.h>

class Discount {
private:
    char category[50];
    int discountPercent;

public:
    // Конструктор с параметрами
    Discount(const char* category = "", int discountPercent = 0) {
        strncpy(this->category, category, sizeof(this->category) - 1);
        this->category[sizeof(this->category) - 1] = '\0';
        this->discountPercent = discountPercent;
    }

    // Геттеры
    const char* getCategory() const { return category; }
    int getDiscountPercent() const { return discountPercent; }

    // Сеттеры
    void setCategory(const char* newCategory) { strncpy(category, newCategory, sizeof(category) - 1); }
    void setDiscountPercent(int percent) { discountPercent = percent; }
};

class Store {
private:
    char name[100];
    char address[100];
    Discount discount;

public:
    // Конструктор по умолчанию
    Store() : name(""), address(""), discount() {}

    // Конструктор с параметрами
    Store(const char* name, const char* address, const Discount& discount) {
        strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';
        strncpy(this->address, address, sizeof(this->address) - 1);
        this->address[sizeof(this->address) - 1] = '\0';
        this->discount = discount;
    }

    // Конструктор перемещения
    Store(Store&& other) noexcept {
        strncpy(this->name, other.name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';
        strncpy(this->address, other.address, sizeof(this->address) - 1);
        this->address[sizeof(this->address) - 1] = '\0';
        this->discount = std::move(other.discount);  // Перемещаем discount

        // Обнуляем данные перемещаемого объекта
        other.name[0] = '\0';
        other.address[0] = '\0';
    }

    // Оператор присваивания перемещением
    Store& operator=(Store&& other) noexcept {
        if (this != &other) {
            // Освобождаем ресурсы текущего объекта
            this->discount = std::move(other.discount);

            // Перемещаем данные из other
            strncpy(this->name, other.name, sizeof(this->name) - 1);
            this->name[sizeof(this->name) - 1] = '\0';
            strncpy(this->address, other.address, sizeof(this->address) - 1);
            this->address[sizeof(this->address) - 1] = '\0';

            // Обнуляем данные перемещаемого объекта
            other.name[0] = '\0';
            other.address[0] = '\0';
        }
        return *this;
    }

    // Геттеры
    const char* getName() const { return name; }
    const char* getAddress() const { return address; }
    const Discount& getDiscount() const { return discount; }

    // Сеттеры
    void setName(const char* newName) { strncpy(name, newName, sizeof(name) - 1); }
    void setAddress(const char* newAddress) { strncpy(address, newAddress, sizeof(address) - 1); }
    void setDiscount(const Discount& newDiscount) { discount = newDiscount; }

    // Ввод данных
      void inputStore() {
        std::cout << "Введите название магазина: ";
        std::cin.ignore();
        std::cin.getline(name, sizeof(name));

        std::cout << "Введите адрес магазина: ";
        std::cin.getline(address, sizeof(address));

        char category[50];
        int discountPercent;

        std::cout << "Введите категорию товара (например, одежда): ";
        std::cin.getline(category, sizeof(category));

        std::cout << "Введите процент скидки: ";
        std::cin >> discountPercent;

        discount.setCategory(category);  // Используем сеттеры
        discount.setDiscountPercent(discountPercent);
    }
    // Вывод информации о магазине
    void printStore() const {
        std::cout << std::left << std::setw(20) << name
                  << std::setw(30) << address
                  << std::setw(15) << discount.getCategory()
                  << std::setw(10) << discount.getDiscountPercent() << "%\n";
    }

    int getDiscountPercent() const {
        return discount.getDiscountPercent();
    }
};

const int MAX_STORES = 100;
Store storeList[MAX_STORES];
int nStores = 0;

// Добавление магазина
void addStore() {
    if (nStores < MAX_STORES) {
        storeList[nStores].inputStore();
        nStores++;
        std::cout << "\nМагазин успешно добавлен!\n";
    } else {
        std::cout << "\nНельзя добавить больше магазинов, достигнут лимит!\n";
    }
}

// Вывод магазинов
void printStores() {
    if (nStores == 0) {
        std::cout << "\nНет доступных магазинов!\n";
        return;
    }

    std::cout << std::left << std::setw(5) << "№"
              << std::setw(20) << "Название"
              << std::setw(30) << "Адрес"
              << std::setw(15) << "Категория"
              << std::setw(10) << "Скидка"
              << "\n";

    std::cout << std::string(80, '-') << "\n";

    for (int i = 0; i < nStores; ++i) {
        std::cout << std::left << std::setw(5) << (i + 1);
        storeList[i].printStore();
    }
}

// Поиск магазина с лучшей скидкой на одежду
void printBestClothingDiscount() {
    int bestIndex = -1;
    for (int i = 0; i < nStores; ++i) {
        if (strcasecmp(storeList[i].getDiscount().getCategory(), "одежда") == 0 || 
            strcasecmp(storeList[i].getDiscount().getCategory(), "clothes") == 0) {
            if (bestIndex == -1 || storeList[i].getDiscountPercent() > storeList[bestIndex].getDiscountPercent()) {
                bestIndex = i;
            }
        }
    }

    if (bestIndex == -1) {
        std::cout << "\nНет магазинов с категориями одежды!\n";
    } else {
        std::cout << "\nМагазин с самой выгодной скидкой на одежду:\n";
        storeList[bestIndex].printStore();
    }
}

// Удалить магазин
void deleteStore(int index) {
    if (index < 0 || index >= nStores) {
        std::cout << "Неверный индекс магазина.\n";
        return;
    }

    // Перемещаем все магазины после удаленного на одну позицию влево
    for (int i = index; i < nStores - 1; ++i) {
        storeList[i] = std::move(storeList[i + 1]);  // Используем перемещающий оператор
    }

    nStores--;
    std::cout << "\nМагазин удален!\n";
}

// Чтение из файла
void readFromFile() {
    std::ifstream file("store.txt");
    if (!file) return;

    nStores = 0;
    while (nStores < MAX_STORES && file.peek() != EOF) {
        char name[100], address[100], category[50];
        int discountPercent;

        file.getline(name, 100, '|');
        file.getline(address, 100, '|');
        file.getline(category, 50, '|');
        file >> discountPercent;
        file.ignore();

        if (file.fail()) break;

        storeList[nStores].setName(name);
        storeList[nStores].setAddress(address);
        storeList[nStores].setDiscount(Discount(category, discountPercent));
        nStores++;
    }
    file.close();
}

// Сохранение данных в файл
void saveToFile() {
    std::ofstream file("store.txt");
    for (int i = 0; i < nStores; ++i) {
        file << storeList[i].getName() << "|"
             << storeList[i].getAddress() << "|"
             << storeList[i].getDiscount().getCategory() << "|"
             << storeList[i].getDiscount().getDiscountPercent() << "\n";
    }
    file.close();
}

// Меню
void printMenu() {
    std::cout << "[1] Добавить магазин\n";
    std::cout << "[2] Показать все магазины\n";
    std::cout << "[3] Показать магазин с лучшей скидкой на одежду\n";
    std::cout << "[4] Удалить магазин\n";
    std::cout << "[0] Выход\n";
    std::cout << "Выберите опцию: ";
}

int main() {
    SetConsoleCP(65001);       // UTF-8
    SetConsoleOutputCP(65001); 
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    readFromFile();
    int command;

    do {
        printMenu();
        std::cin >> command;

        if (command == 1) {
            addStore();
        } else if (command == 2) {
            printStores();
        } else if (command == 3) {
            printBestClothingDiscount();
        } else if (command == 4) {
            int index;
            std::cout << "Введите номер магазина для удаления: ";
            std::cin >> index;
            deleteStore(index - 1);  // Индексы начинаются с 1, но в массиве с 0
        }
    } while (command != 0);

    saveToFile();
    std::cout << "\nВыход!\n";
    return 0;
}
