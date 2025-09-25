#include "customer_generator.h"
#include "customer.h"
#include <iostream>

extern bool is_working_hours; // Globální proměnná pro pracovní dobu

CustomerGenerator::CustomerGenerator(double call_rate) : call_rate(call_rate) {}

//Implementace Generatoru zákazníků
void CustomerGenerator::Behavior(){
    if (is_working_hours){
        (new Customer)->Activate();
    }
    else {
       
    }

    Activate(Time + Exponential(call_rate)); //Další zákazník
}