#include "foodItem.hpp"

namespace websocket{
    FoodItem::FoodItem(double & x, double & y,double radius): Element(x,y,radius)
    {
        //std::cout << "construct food" << std::endl;
    }
}