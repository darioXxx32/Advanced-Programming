#Name: Dario Pomasqui
#Id:1050395092
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <string>

// ----------------------------------------------------
// Task 1: Managing Products with std::map
// ----------------------------------------------------
void addProduct(std::map<std::string, int>& inventory,
                const std::string& productName, int quantity) {
    // If product doesn't exist, it will be created automatically with value=0.
    inventory[productName] += quantity;
    std::cout << "Added " << quantity << " of " << productName
              << " to the inventory.\n";
}

void updateProduct(std::map<std::string, int>& inventory,
                   const std::string& productName, int quantity) {
    // Update quantity (replace existing quantity).
    if (inventory.find(productName) != inventory.end()) {
        inventory[productName] = quantity;
        std::cout << "Updated " << productName
                  << " quantity to " << quantity << ".\n";
    } else {
        std::cout << "Product " << productName << " not found in inventory.\n";
    }
}

void displayInventory(const std::map<std::string, int>& inventory) {
    std::cout << "\n--- Current Inventory ---\n";
    for (const auto& item : inventory) {
        std::cout << item.first << ": " << item.second << " in stock\n";
    }
    std::cout << std::endl;
}

// ----------------------------------------------------
// Task 2: Tracking Unique Products with std::set
// ----------------------------------------------------
void addCategory(std::set<std::string>& categories,
                 const std::string& categoryName) {
    auto result = categories.insert(categoryName);
    if (result.second) {
        std::cout << "Category \"" << categoryName << "\" added.\n";
    } else {
        std::cout << "Category \"" << categoryName << "\" already exists.\n";
    }
}

void displayCategories(const std::set<std::string>& categories) {
    std::cout << "\n--- Product Categories ---\n";
    for (const auto& category : categories) {
        std::cout << "- " << category << "\n";
    }
    std::cout << std::endl;
}

// ----------------------------------------------------
// Task 3: Processing Orders with std::queue
// ----------------------------------------------------
void addOrder(std::queue<std::string>& orders, const std::string& order) {
    orders.push(order);
    std::cout << "Added order: " << order << "\n";
}

void processOrders(std::queue<std::string>& orders) {
    std::cout << "\n--- Processing Orders (FIFO) ---\n";
    while (!orders.empty()) {
        std::cout << "Processing " << orders.front() << "\n";
        orders.pop();
    }
    std::cout << std::endl;
}

// ----------------------------------------------------
// Task 4: Handling Restocks with std::stack
// ----------------------------------------------------
void addRestock(std::stack<std::pair<std::string, int>>& restocks,
                const std::string& productName, int quantity) {
    restocks.push({productName, quantity});
    std::cout << "Added restock batch: " << quantity << " of " << productName << "\n";
}

void processRestocks(std::stack<std::pair<std::string, int>>& restocks,
                     std::map<std::string, int>& inventory) {
    std::cout << "\n--- Processing Restocks (LIFO) ---\n";
    while (!restocks.empty()) {
        auto& item = restocks.top();
        const std::string& productName = item.first;
        int quantity = item.second;
        
        // Update inventory
        inventory[productName] += quantity;
        
        std::cout << "Restocking " << quantity
                  << " units of " << productName << "\n";
        restocks.pop();
    }
    std::cout << std::endl;
}

// ----------------------------------------------------
// Task 5: Storing Customer Purchases with std::vector
// ----------------------------------------------------
void addToCart(std::vector<std::string>& cart, const std::string& item) {
    cart.push_back(item);
    std::cout << "Added \"" << item << "\" to cart.\n";
}

void displayCart(const std::vector<std::string>& cart) {
    std::cout << "\n--- Items in Customer Cart ---\n";
    for (const auto& item : cart) {
        std::cout << "- " << item << "\n";
    }
    std::cout << std::endl;
}

// ----------------------------------------------------
// Main Function: Demonstration
// ----------------------------------------------------
int main() {
    // 1) std::map: Inventory
    std::map<std::string, int> inventory;
    addProduct(inventory, "Laptop", 5);
    addProduct(inventory, "Mouse", 20);
    addProduct(inventory, "Keyboard", 10);
    displayInventory(inventory);

    updateProduct(inventory, "Mouse", 25);  // update quantity to 25
    displayInventory(inventory);

    // 2) std::set: Product Categories
    std::set<std::string> productCategories;
    addCategory(productCategories, "Electronics");
    addCategory(productCategories, "Accessories");
    addCategory(productCategories, "Peripherals");
    displayCategories(productCategories);

    // 3) std::queue: Orders (FIFO)
    std::queue<std::string> orders;
    addOrder(orders, "Order#1: Laptop");
    addOrder(orders, "Order#2: Mouse");
    addOrder(orders, "Order#3: Keyboard");
    processOrders(orders);

    // 4) std::stack: Restocks (LIFO)
    std::stack<std::pair<std::string, int>> restocks;
    addRestock(restocks, "Mouse", 10);
    addRestock(restocks, "Laptop", 2);
    addRestock(restocks, "Keyboard", 5);
    processRestocks(restocks, inventory);
    // Check updated inventory after restocks
    displayInventory(inventory);

    // 5) std::vector: Customer Cart
    std::vector<std::string> customerCart;
    addToCart(customerCart, "Laptop");
    addToCart(customerCart, "Mouse");
    addToCart(customerCart, "Keyboard");
    displayCart(customerCart);

    return 0;
}
//Añadir mas valores y u productos 