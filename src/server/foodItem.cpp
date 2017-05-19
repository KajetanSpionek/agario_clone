#include "foodItem.hpp"

namespace websocket{
    FoodItem::FoodItem(int & x, int & y, int& id): Id_(id),x_(x), y_(y)
    {
        std::cout << "construct food" << std::endl;
    }
}