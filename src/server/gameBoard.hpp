/*!
*\file      GameBoard.hpp
*\author    
*\date      
*/
#ifndef _FOODITEM_H_
#define _FOODITEM_H_

#include "element.hpp"

#include <iostream>
#include <memory>
#include <vector>

class GameBoard {
public:
    //different argument of the method
   // void registerElement();
   // void unregisterElement();
   // void notifyElements();
private:
    typedef std::shared_ptr<Element> observer;
    typedef std::vector<observer> observers;
    observers observersCollection_;
};

#endif //_FOODITEM_H_