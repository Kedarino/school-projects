#include "work_shift.h"
#include <iostream>

extern bool is_working_hours;

void WorkShift::Behavior(){

    while(true) {
        is_working_hours = true;
        std::cout << "Work shift has started at time " << Time << std::endl;
        Wait(Time + 8*60);
        

        is_working_hours = false;
        std::cout << "Work shift has ended at time " << Time << std::endl;
        Wait(Time + 16*60);
        
    }
};
