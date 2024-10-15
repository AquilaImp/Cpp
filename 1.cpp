#include <iostream>
#include <vector>
#include <fstream>
#include <string>

struct Sale {
    std::string storeName;
    std::string address;
    double discountPercentage;
    std::string applicableItems;
};

std::vector<Sale> sales;


void loadSalesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        Sale sale;
        while (file >> sale.discountPercentage) {
            file.ignore(); 
            std::getline(file, sale.storeName, ';');
            std::getline(file, sale.address, ';');
            std::getline(file, sale.applicableItems);
            sales.push_back(sale);
        }
        file.close();
    }
}


void saveSalesToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& sale : sales) {
            file << sale.discountPercentage << " " << sale.storeName << ";"
                 << sale.address << ";" << sale.applicableItems << "\n";
        }
        file.close();
    }
}


void printSales() {
    if (sales.empty()) {
        std::cout << "Список распродаж пуст." << std::endl;
        return;
    }
    for (const auto& sale : sales) {
        std::cout << "Магазин: " << sale.storeName << ", Адрес: " << sale.address
                  << ", Скидка: " << sale.discountPercentage << "%, Товары: " << sale.applicableItems << std::endl;
    }
}


void addSale() {
    Sale sale;
    std::cout << "Введите название магазина: ";
    std::getline(std::cin >> std::ws, sale.storeName);
    std::cout << "Введите адрес магазина: ";
    std::getline(std::cin >> std::ws, sale.address);
    std::cout << "Введите процент скидки: ";
    std::cin >> sale.discountPercentage;
    std::cin.ignore(); 
    std::cout << "Введите, на что распространяется скидка: ";
    std::getline(std::cin >> std::ws, sale.applicableItems);
    sales.push_back(sale);
    std::cout << "Распродажа добавлена." << std::endl;
}


void deleteSale() {
    int index;
    std::cout << "Введите номер распродажи для удаления: ";
    std::cin >> index;
    index--;
    if (index < 0 || index >= sales.size()) {
        std::cout << "Неверный номер распродажи." << std::endl;
        return;
    }
    sales.erase(sales.begin() + index);
    std::cout << "Распродажа удалена." << std::endl;
}


void findBestClothingDeals() {
    double maxDiscount = 0.0;
    for (const auto& sale : sales) {
        if (sale.applicableItems.find("одежда") != std::string::npos && sale.discountPercentage > maxDiscount) {
            maxDiscount = sale.discountPercentage;
        }
    }
    if (maxDiscount == 0.0) {
        std::cout << "Нет выгодных предложений на одежду." << std::endl;
        return;
    }
    std::cout << "Лучшие предложения на одежду (скидка " << maxDiscount << "%):" << std::endl;
    for (const auto& sale : sales) {
        if (sale.applicableItems.find("одежда") != std::string::npos && sale.discountPercentage == maxDiscount) {
            std::cout << "Магазин: " << sale.storeName << ", Адрес: " << sale.address
                      << ", Товары: " << sale.applicableItems << std::endl;
        }
    }
}

int main() {
    const std::string filename = "sales.txt";
    loadSalesFromFile(filename);
    int choice;
    do {
        std::cout << "\nМеню:\n"
                  << "1. Показать все распродажи\n"
                  << "2. Добавить распродажу\n"
                  << "3. Удалить распродажу\n"
                  << "4. Найти лучшие предложения на одежду\n"
                  << "5. Выход\n"
                  << "Введите выбор: ";
        std::cin >> choice;
        std::cin.ignore(); 
        switch (choice) {
            case 1:
                printSales();
                break;
            case 2:
                addSale();
                break;
            case 3:
                deleteSale();
                break;
            case 4:
                findBestClothingDeals();
                break;
            case 5:
                saveSalesToFile(filename);
                std::cout << "Данные сохранены. Программа завершена." << std::endl;
                break;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова." << std::endl;
                break;
        }
    } while (choice != 5);
    return 0;
}
