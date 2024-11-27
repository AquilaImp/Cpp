#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <locale>
#include <windows.h>

struct Discount {
    char category[50];
    int discountPercent;
};

class Store {
private:
    char name[100];
    char address[100];
    Discount discount;

public:
    //Геттеры
    const char* getName() const { return name; }
    const char* getAddress() const { return address; }
    const Discount& getDiscount() const { return discount; }

    //Сеттеры
    void setName(const char* newName) { strncpy(name, newName, 100); }
    void setAddress(const char* newAddress) { strncpy(address, newAddress, 100); }
    void setDiscount(const char* category, int percent) {
        strncpy(discount.category, category, 50);
        discount.discountPercent = percent;
    }

    //Ввод данных
    void inputStore() {
        std::cout << "Введите название магазина: ";
        std::cin.ignore();
        std::cin.getline(name, 100);
        std::cout << "Введите адрес магазина: ";
        std::cin.getline(address, 100);
        std::cout << "Введите категорию товара (например, одежда): ";
        std::cin.getline(discount.category, 50);
        std::cout << "Введите процент скидки: ";
        std::cin >> discount.discountPercent;
    }

    // Вывод информации о магазине
    void printStore() const {
        std::cout << std::left << std::setw(20) << name
                  << std::setw(30) << address
                  << std::setw(15) << discount.category
                  << std::setw(10) << discount.discountPercent << "%\n";
    }

    int getDiscountPercent() const {
        return discount.discountPercent;
    }

    const char* getCategory() const {
        return discount.category;
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
    std::cout << std::setw(5) << std::left << "№" 
              << std::setw(32) << std::left << "Название" 
              << std::setw(48) << std::left << "Адрес" 
              << std::setw(30) << std::left << "Категория" 
              << std::setw(0) << std::left << "Скидка" 
              << "\n";

    std::cout << std::string(100, '-') << "\n";
    // Вывод каждого магазина
    for (int i = 0; i < nStores; ++i) {
        std::cout << std::setw(5) << std::left << (i + 1)  
                  << std::setw(25) << std::left << storeList[i].getName()   
                  << std::setw(40) << std::left << storeList[i].getAddress() 
                  << std::setw(22) << std::left << storeList[i].getCategory()  
                  << std::setw(0) << std::left << storeList[i].getDiscountPercent() << "%" 
                  << "\n";
    }
}
// Поиск магазина с лучшей скидкой на одежду
void printBestClothingDiscount() {
    int bestIndex = -1;
    for (int i = 0; i < nStores; ++i) {   
        if (stricmp(storeList[i].getCategory(), "одежда") == 0 || stricmp(storeList[i].getCategory(), "clothes") == 0) {
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
// Чтение данных из файла
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
        storeList[nStores].setDiscount(category, discountPercent);
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
             << storeList[i].getDiscount().category << "|"
             << storeList[i].getDiscount().discountPercent << "\n";
    }
    file.close();
}

// Удаление магазина
void removeStore() {
    if (nStores == 0) {
        std::cout << "\nНет магазинов для удаления!\n";
        return;
    }

    int storeIndex;
    std::cout << "\nВведите номер магазина для удаления (1-" << nStores << "): ";
    std::cin >> storeIndex;

    if (storeIndex < 1 || storeIndex > nStores) {
        std::cout << "\nНекорректный номер магазина!\n";
        return;
    }

    // Сдвигаем все магазины после удаляемого на одну позицию влево
    for (int i = storeIndex - 1; i < nStores - 1; ++i) {
        storeList[i] = storeList[i + 1];
    }

    nStores--;
    std::cout << "\nМагазин успешно удален!\n";
}

// Меню
void printMenu() {
    std::cout << "[1] Добавить магазин\n";
    std::cout << "[2] Показать все магазины\n";
    std::cout << "[3] Показать магазин с лучшей скидкой на одежду\n";
    std::cout << "[4] Удалить магазин\n";  // Новая опция
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
            removeStore();  // Обработка удаления магазина
        }
    } while (command != 0);

    saveToFile();
    std::cout << "\nВыход!\n";
    return 0;
}
