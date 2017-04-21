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
    double radius_; //radius
    //velocity in specific direction
    double vX_;
    double vY_;
    //HOW TO DEFINE COLORS
};


// purely abstract class
class Element {
private:
    static unsigned int idCount_;
    unsigned int id_;
    double x_;
    double y_;
    double radius_;
public:
    Element(); 
    virtual ~Element();
    virtual void notify() = 0; 
};


#endif //ELEMENT_HPP