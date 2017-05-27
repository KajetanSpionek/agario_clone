#include "foodItem.hpp"

namespace websocket{
    FoodItem::FoodItem(int & x, int & y,int radius): Element(x,y,radius)
    {
        //std::cout << "construct food" << std::endl;
    }
}