#ifndef CUSTOMER_GENERATOR_H
#define CUSTOMER_GENERATOR_H

#include "simlib.h"

// Deklarace třídy CustomerGenerator
class CustomerGenerator : public Event {
    public:
        CustomerGenerator(double call_rate);
        void Behavior(); // Definice bude v .cpp

    private:
        double call_rate;

};

#endif // CUSTOMER_GENERATOR_H
