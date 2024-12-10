#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>

struct Store {
    std::string name;
    std::string address;
    std::string category;
    int discountPercent;

    Store(const std::string& n, const std::string& a, const std::string& c, int d)
        : name(n), address(a), category(c), discountPercent(d) {}
};

std::vector<Store> storeList;

//Чтение файла
void readFromFile() {
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

//Сохр файла
void saveToFile() {
    std::ofstream file("store.txt");
    for (const auto& store : storeList) {
        file << store.name << "|"
             << store.address << "|"
             << store.category << "|"
             << store.discountPercent << "\n";
    }
}

//Список
void printStores() {
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
                  << std::setw(22) << std::left << store.name
                  << std::setw(44) << std::left << store.address
                  << std::setw(23) << std::left << store.category
                  << store.discountPercent << "%\n";
    }
}

//Добавление
void addStore() {
    std::string name, address, category;
    int discountPercent;

    std::cout << "Введите название магазина: ";
    std::getline(std::cin >> std::ws, name);
    std::cout << "Введите адрес магазина: ";
    std::getline(std::cin, address);
    std::cout << "Введите категорию товаров: ";
    std::getline(std::cin, category);
    std::cout << "Введите скидку (%): ";
    std::cin >> discountPercent;

    storeList.emplace_back(name, address, category, discountPercent);
    std::cout << "Магазин добавлен.\n";
}

//Поиск скидки
void findBestClothingDeals() {
    const Store* bestDeal = nullptr;

    for (const auto& store : storeList) {
        if (store.category.find("одежда") != std::string::npos || store.category.find("clothes") != std::string::npos) {
            if (!bestDeal || store.discountPercent > bestDeal->discountPercent) {
                bestDeal = &store;
            }
        }
    }

    if (!bestDeal) {
        std::cout << "Нет магазинов с выгодными скидками на одежду.\n";
        return;
    }

    std::cout << "\nМагазин с лучшей скидкой на одежду:\n";
    std::cout << "Название: " << bestDeal->name << "\n"
              << "Адрес: " << bestDeal->address << "\n"
              << "Категория: " << bestDeal->category << "\n"
              << "Скидка: " << bestDeal->discountPercent << "%\n";
}

// Удалить магазин
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

int main() {
    SetConsoleCP(65001);       // UTF-8
    SetConsoleOutputCP(65001); 
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    readFromFile();
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

        //выбор опц
        switch (choice) {
            case 1:
                addStore();
                break;
            case 2:
                printStores();
                break;
            case 3:
                findBestClothingDeals();
                break;
            case 4:
                deleteStore();
                break;
            case 0:
                saveToFile();
                std::cout << "Данные сохранены. Программа завершена.\n";
                break;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
