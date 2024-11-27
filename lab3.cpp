#include <iostream>
#include <cstring>
#include <iomanip>
#include <locale>
#include <vector>
#include <algorithm>
#include <windows.h>

class Store {
public:
    class Discount {
    private:
        char category[50];
        int discountPercent;

    public:
        Discount() : discountPercent(0) { strcpy(category, "Не указана"); }

        Discount(const char* category, int discountPercent) {
            strncpy(this->category, category, 50);
            this->discountPercent = discountPercent;
        }

        const char* getCategory() const { return category; }
        int getDiscountPercent() const { return discountPercent; }

        void setCategory(const char* newCategory) { strncpy(category, newCategory, 50); }
        void setDiscountPercent(int percent) { discountPercent = percent; }

        // Перегрузка оператора +
        Discount operator+(const Discount& other) const {
            Discount newDiscount;
            newDiscount.discountPercent = this->discountPercent + other.discountPercent;
            return newDiscount;
        }

        // Перегрузка оператора -
        Discount operator-(const Discount& other) const {
            Discount newDiscount;
            newDiscount.discountPercent = this->discountPercent - other.discountPercent;
            return newDiscount;
        }

        // Перегрузка оператора *
        Discount operator*(int multiplier) const {
            Discount newDiscount;
            newDiscount.discountPercent = this->discountPercent * multiplier;
            return newDiscount;
        }

        // Перегрузка оператора присваивания
        Discount& operator=(const Discount& other) {
            if (this != &other) {  // проверка на самоприсваивание
                strncpy(this->category, other.category, 50);
                this->discountPercent = other.discountPercent;
            }
            return *this;
        }
    };

private:
    char name[100];
    char address[100];
    Discount discount;

public:
    Store() : discount() { strcpy(name, "Не указано"); strcpy(address, "Не указано"); }

    Store(const char* name, const char* address, const char* category, int discountPercent)
        : discount(category, discountPercent) {
        strncpy(this->name, name, 100);
        strncpy(this->address, address, 100);
    }

    const char* getName() const { return name; }
    const char* getAddress() const { return address; }
    const Discount& getDiscount() const { return discount; }

    void setName(const char* newName) { strncpy(name, newName, 100); }
    void setAddress(const char* newAddress) { strncpy(address, newAddress, 100); }
    void setDiscount(const char* category, int percent) {
        discount.setCategory(category);
        discount.setDiscountPercent(percent);
    }

    void inputStore() {
        std::cout << "Введите название магазина: ";
        std::cin.ignore();
        std::cin.getline(name, 100);
        std::cout << "Введите адрес магазина: ";
        std::cin.getline(address, 100);
        std::cout << "Введите категорию товара (например, одежда, обувь): ";
        char category[50];
        std::cin.getline(category, 50);
        std::cout << "Введите процент скидки: ";
        int discountPercent;
        std::cin >> discountPercent;

        setDiscount(category, discountPercent);
    }

    void printStore() const {
        std::cout << std::left << std::setw(20) << name
                  << std::setw(30) << address
                  << std::setw(15) << discount.getCategory()
                  << std::setw(10) << discount.getDiscountPercent() << "%\n";
    }

    // Перегрузка оператора +
    Store operator+(const Store& other) const {
        Store newStore;
        newStore.setName(this->name);
        newStore.setAddress(this->address);
        newStore.discount = this->discount + other.discount;
        return newStore;
    }

    // Перегрузка оператора -
    Store operator-(const Store& other) const {
        Store newStore;
        newStore.setName(this->name);
        newStore.setAddress(this->address);
        newStore.discount = this->discount - other.discount;
        return newStore;
    }

    // Перегрузка оператора *
    Store operator*(int multiplier) const {
        Store newStore;
        newStore.setName(this->name);
        newStore.setAddress(this->address);
        newStore.discount = this->discount * multiplier;
        return newStore;
    }

    // Перегрузка оператора присваивания
    Store& operator=(const Store& other) {
        if (this != &other) {
            strncpy(this->name, other.name, 100);
            strncpy(this->address, other.address, 100);
            this->discount = other.discount;
        }
        return *this;
    }
};

std::vector<Store> storeList;

void addStore() {
    Store store;
    store.inputStore();
    storeList.push_back(store);
    std::cout << "\nМагазин успешно добавлен!\n";
}

void printStores() {
    if (storeList.empty()) {
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

    for (size_t i = 0; i < storeList.size(); ++i) {
        std::cout << std::left << std::setw(5) << (i + 1);
        storeList[i].printStore();
    }
}

void printBestClothingDiscount() {
    int bestIndex = -1;
    for (size_t i = 0; i < storeList.size(); ++i) {
        const std::string category = storeList[i].getDiscount().getCategory();
        if (category == "джинсы" || category == "футболка" || category == "топик") {
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


void printMenu() {
    std::cout << "[1] Добавить магазин\n";
    std::cout << "[2] Показать все магазины\n";
    std::cout << "[3] Показать магазин с лучшей скидкой на одежду\n";
    std::cout << "[0] Выход\n";
    std::cout << "Выберите опцию: ";
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleCP(65001);       //UTF-8
    SetConsoleOutputCP(65001); 
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
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
        }
    } while (command != 0);

    std::cout << "\nВыход!\n";
    return 0;
}
