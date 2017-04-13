/*!
*\file      Element.hpp
*\author    
*\date      
*/
#ifndef ELEMENT_HPP
#define ELEMENT_HPP
//data that will be send while updating
struct UpdateElementData{
    unsigned int id_;
    double x_;
    double y_;
    double size_;
    //velocity in specific direction
    double vX_;
    double vY_;
};

// purely abstract class
class Element {
private:
    unsigned int id_;

public:
    Element();
    virtual ~Element();
    virtual void notify() = 0;
};


#endif //ELEMENT_HPP