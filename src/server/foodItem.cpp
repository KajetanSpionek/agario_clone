#include "foodItem.hpp"

namespace websocket{
    FoodItem::FoodItem(int & x, int & y,int radius, int& id): Element(x,y,radius,id)
    {
        //std::cout << "construct food" << std::endl;
    }
}