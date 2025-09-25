#ifndef CUSTOMER_GENERATOR_H
#define CUSTOMER_GENERATOR_H

#include "simlib.h"

// Deklarace třídy CustomerGenerator
class CustomerGenerator : public Event {
    public:
        CustomerGenerator(double call_rate);
        void Behavior(); 

    private:
        double call_rate;

};

#endif // CUSTOMER_GENERATOR_H
