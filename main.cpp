#include "OrderBook.h"

int main() {
    OrderBook book;
    
    // Add some Bids (Buyers)
    book.add_order(100.50, 100, true); // ID 1
    book.add_order(100.40, 200, true); // ID 2
    
    // Add some Asks (Sellers)
    book.add_order(101.00, 150, false); // ID 3
    book.add_order(100.80, 100, false); // ID 4
    
    std::cout << "Initial Book:\n";
    book.print_book();
    std::cout << "Spread: $" << book.get_spread() << "\n\n";
    
    // Add a marketable buy order that crosses the spread
    std::cout << "Adding aggressive Buy order (100.90 x 50)...\n";
    book.add_order(100.90, 50, true); // ID 5
    
    std::cout << "\nBook after trade:\n";
    book.print_book();
    
    return 0;
}