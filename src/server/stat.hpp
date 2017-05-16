#ifndef STAT_HPP
#define STAT_HPP

class Stat{
    public:
        void incBall(int n_el);
        void incFood(int n_el);
        unsigned int getBallNum();
        unsigned int getFoodNum();
    private:
        //unsigned int score_;
        unsigned int ballEaten_;
        unsigned int foodEaten_;
    };

#endif //STAT_HPP