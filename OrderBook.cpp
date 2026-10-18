#include "OrderBook.h"

int OrderBook::add_order(double price, int quantity, bool is_buy) {
    int current_id = next_id++;
    Order new_order{current_id, price, quantity, is_buy};
    
    // Store it for potential lookup
    order_lookup[current_id] = new_order;
    
    // Add to the correct side of the book
    if (is_buy) {
        bids[price].push_back(new_order);
    } else {
        asks[price].push_back(new_order);
    }
    
    // Check if we can match immediately
    match();
    
    return current_id;
}

void OrderBook::match() {
    // Keep matching as long as there is a bid >= ask
    while (!bids.empty() && !asks.empty()) {
        auto best_bid = bids.begin();
        auto best_ask = asks.begin();
        
        // If the highest bid is lower than the lowest ask, no match
        if (best_bid->first < best_ask->first) {
            break; 
        }
        
        // We have a match! Get the first order at these price levels
        Order& buy_order = best_bid->second.front();
        Order& sell_order = best_ask->second.front();
        
        int trade_qty = std::min(buy_order.quantity, sell_order.quantity);
        
        // Print the trade (In a real system, you'd push to a trade log)
        std::cout << "TRADE EXECUTED: " << trade_qty 
                  << " shares at $" << sell_order.price << "\n";
                  
        // Reduce quantities
        buy_order.quantity -= trade_qty;
        sell_order.quantity -= trade_qty;
        
        // If an order is fully filled (quantity == 0), remove it
        if (buy_order.quantity == 0) {
            order_lookup.erase(buy_order.id);
            best_bid->second.erase(best_bid->second.begin());
            if (best_bid->second.empty()) bids.erase(best_bid);
        }
        
        if (sell_order.quantity == 0) {
            order_lookup.erase(sell_order.id);
            best_ask->second.erase(best_ask->second.begin());
            if (best_ask->second.empty()) asks.erase(best_ask);
        }
    }
}

void OrderBook::cancel_order(int id) {
    if (order_lookup.find(id) == order_lookup.end()) return; // Not found
    
    Order& ord = order_lookup[id];
    
    // Find the vector of orders at that price
    if (ord.is_buy) {
        auto& vec = bids[ord.price];
        vec.erase(std::remove_if(vec.begin(), vec.end(), 
            [id](const Order& o) { return o.id == id; }), vec.end());
        if (vec.empty()) bids.erase(ord.price);
    } else {
        auto& vec = asks[ord.price];
        vec.erase(std::remove_if(vec.begin(), vec.end(), 
            [id](const Order& o) { return o.id == id; }), vec.end());
        if (vec.empty()) asks.erase(ord.price);
    }
    
    order_lookup.erase(id);
}

void OrderBook::print_book() {
    std::cout << "--- ASKS ---\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << "$" << it->first << " x " << it->second.size() << " orders\n";
    }
    std::cout << "--- BIDS ---\n";
    for (auto& pair : bids) {
        std::cout << "$" << pair.first << " x " << pair.second.size() << " orders\n";
    }
}

double OrderBook::get_spread() {
    if (bids.empty() || asks.empty()) return 0.0;
    return asks.begin()->first - bids.begin()->first;
}