/*!
*\file      Element.hpp
*\author    
*\date      
*/
#ifndef _ELEMENT_H_
#define _ELEMENT_H_
//data that will be send while updating
struct UpdateElementData{
    unsigned int id_;
    double x_;
    double y_;
    double size_;
    //velocity in specific direction
    double vX_;
    double vY_;
}

// purely abstract class
class Element {
private:
    unsigned int id_;

public:
    Element();
    virtual ~Element();
    virtual void update();
};


#endif //_ELEMENT_H_