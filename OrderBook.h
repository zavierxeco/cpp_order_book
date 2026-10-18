#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

// 1. Define the Order
struct Order {
    int id;
    double price;
    int quantity;
    bool is_buy; // true = Bid, false = Ask
};

// 2. Define the Trade (What happens when a buy matches a sell)
struct Trade {
    int buy_id;
    int sell_id;
    double price;
    int quantity;
};

class OrderBook {
private:
    // Bids: Highest price first (std::greater)
    std::map<double, std::vector<Order>, std::greater<double>> bids;
    
    // Asks: Lowest price first (default sorting)
    std::map<double, std::vector<Order>> asks;
    
    // Quick lookup for cancellations
    std::unordered_map<int, Order> order_lookup;
    
    int next_id = 1;

    void match();

public:
    // Add an order to the book
    int add_order(double price, int quantity, bool is_buy);
    
    // Cancel an order by ID
    void cancel_order(int id);
    
    // Print the current book
    void print_book();
    
    // Get the spread (Best Ask - Best Bid)
    double get_spread();
};