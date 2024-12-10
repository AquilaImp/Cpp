#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <limits>
#include <stdexcept>

class Store {
private:
    std::string name;  
    std::string address;  
    std::string category;  
    int discountPercent;  

public:
    Store() : name("Not found"), address("Not found"), category("Not found"), discountPercent(0) {}
    //Конструктор с полным набором параметров
    Store(const std::string& n, const std::string& a, const std::string& c, int d)
        : name(n), address(a), category(c), discountPercent(d) {}

    //Конструктор(без категории и скидки)
    Store(const std::string& n, const std::string& a)
        : name(n), address(a), category("Not found"), discountPercent(0) {}

    //Конструктор для создания магазина с только скидкой
    Store(int d)
        : name("Not found"), address("Not found"), category("Not found"), discountPercent(d) {}
    //Геттеры для получения данных о магазине
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getCategory() const { return category; }
    int getDiscountPercent() const { return discountPercent; }
    //Сеттеры для изменения данных о магазине
    void setName(const std::string& n) { name = n; }
    void setAddress(const std::string& a) { address = a; }
    void setCategory(const std::string& c) { category = c; }
    void setDiscountPercent(int d) {
        if (d >= 0) {
            discountPercent = d;
        } else {
            std::cerr << "Ошибка!Введите положительное число для скидки.\n";
        }
    }
};

class StoreManager {
private:
    std::vector<Store> storeList;

public:
    //Конструктор(инициализация) цбрать
    StoreManager() {

        storeList.clear();
    }
    //Вывод всех 
    void printStores() const {
        if (storeList.empty()) {
            std::cout << "\nНет доступных магазинов!\n";
            return;
        }
        
        std::cout << std::setw(5) << std::left << "№"
                  << std::setw(32) << std::left << "Название"
                  << std::setw(48) << std::left << "Адрес"
                  << std::setw(30) << std::left << "Категория"
                  << "Скидка (%)\n";
        std::cout << std::string(115, '-') << "\n";

        for (size_t i = 0; i < storeList.size(); ++i) {
        const auto& store = storeList[i];
        std::cout << std::setw(5) << std::left << (i + 1)
                  << std::setw(22) << std::left << (store.getName() == "Not found" ? "Not found" : store.getName())
                  << std::setw(44) << std::left << (store.getAddress() == "Not found" ? "Not found" : store.getAddress())
                  << std::setw(23) << std::left << (store.getCategory() == "Not found" ? "Not found" : store.getCategory())
                  << store.getDiscountPercent() << "%\n";
        }
    }

    //- магазин
    void deleteStore() {
        printStores();
        std::cout << "Введите номер магазина для удаления: ";
        int index;
        std::cin >> index;
        if (index < 1 || index > static_cast<int>(storeList.size())) {
            std::cout << "Неверный номер магазина.\n";
            return;
        }
        storeList.erase(storeList.begin() + (index - 1));
        std::cout << "Магазин удален.\n";
    }

    // Поиск лучшей скуидки
    void findBestClothingDeals() const {
        const Store* bestDeal = nullptr;
        for (const auto& store : storeList) {
            if (store.getCategory().find("одежда") != std::string::npos || 
                store.getCategory().find("clothes") != std::string::npos) {
                if (!bestDeal || store.getDiscountPercent() > bestDeal->getDiscountPercent()) {
                    bestDeal = &store;
                }
            }
        }
        if (!bestDeal) {
            std::cout << "Нет магазинов с выгодными скидками на одежду.\n";
            return;
        }
        std::cout << "\nМагазин с лучшей скидкой на одежду:\n";
        std::cout << "Название: " << bestDeal->getName() << "\n"
                  << "Адрес: " << bestDeal->getAddress() << "\n"
                  << "Категория: " << bestDeal->getCategory() << "\n"
                  << "Скидка: " << bestDeal->getDiscountPercent() << "%\n";
    }

    void addStore() { //+ магазин
    std::string name, address, category;
    int discountPercent;
    int choice;
    std::cout << "Выберите способ добавления магазина:\n"
              << "[1] Полный набор данных\n"
              << "[2] Без категории и скидки\n"
              << "[3] Только с скидкой\n"
              << "Ваш выбор: ";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            std::cout << "Введите название магазина: ";
            std::getline(std::cin, name);
            std::cout << "Введите адрес магазина: ";
            std::getline(std::cin, address);
            std::cout << "Введите категорию товаров: ";
            std::getline(std::cin, category);
            std::cout << "Введите скидку (%): ";
            while (!(std::cin >> discountPercent) || discountPercent < 0) {
                std::cout << "Ошибка!Введите положительное число для скидки.\n";
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cout << "Введите скидку (%): ";
            }
            storeList.emplace_back(name, address, category, discountPercent);
            break;

        case 2:
            std::cout << "Введите название магазина: ";
            std::getline(std::cin, name);
            std::cout << "Введите адрес магазина: ";
            std::getline(std::cin, address);
            storeList.emplace_back(name, address);
            break;

        case 3:
            std::cout << "Введите скидку (%): ";
            while (!(std::cin >> discountPercent) || discountPercent < 0) {
                std::cout << "Ошибка! Введите целое число для скидки.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Введите скидку (%): ";
            }
            storeList.emplace_back(discountPercent); 
            break;

        default:
            std::cout << "Некорректный выбор.\n";
            return;
    }
    std::cout << "Магазин добавлен.\n";
}


    //Сохранение в файл
    void saveToFile() const {
        std::ofstream file("store.txt");
        for (const auto& store : storeList) {
            file << store.getName() << "|"
                 << store.getAddress() << "|"
                 << store.getCategory() << "|"
                 << store.getDiscountPercent() << "\n";
        }
    }

    //Загрузка из файла
    void loadFromFile() {
        std::ifstream file("store.txt");
        if (!file) return;
        storeList.clear();
        std::string name, address, category;
        int discountPercent;

        while (std::getline(file, name, '|') &&
               std::getline(file, address, '|') &&
               std::getline(file, category, '|') &&
               (file >> discountPercent)) {
            file.ignore();
            storeList.emplace_back(name, address, category, discountPercent);
        }
    }

    //открытые для приватных
    void printStoresPublic() const { printStores(); }
    void findBestClothingDealsPublic() const { findBestClothingDeals(); }
    void deleteStorePublic() { deleteStore(); }
};

int main() {
    SetConsoleCP(65001);      //UTF-8
    SetConsoleOutputCP(65001); 
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    StoreManager manager;

    manager.loadFromFile(); 
    int choice;
    do {
        std::cout << "\nМеню:\n"
                  << "[1] Добавить магазин\n"
                  << "[2] Показать все магазины\n"
                  << "[3] Показать магазин с лучшей скидкой на одежду\n"
                  << "[4] Удалить магазин\n"
                  << "[0] Выход\n"
                  << "Выберите опцию: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case 1:
                manager.addStore();
                break;
            case 2:
                manager.printStoresPublic();
                break;
            case 3:
                manager.findBestClothingDealsPublic();
                break;
            case 4:
                manager.deleteStorePublic();
                break;
            case 0:
                manager.saveToFile();
                std::cout << "Данные сохранены. Программа завершена.\n";
                break;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
