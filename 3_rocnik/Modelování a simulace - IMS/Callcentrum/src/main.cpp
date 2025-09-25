#include "simlib.h"
#include "customer_generator.h"
#include "work_shift.h"
#include <iostream>


bool is_working_hours;

// Definování ukazatelů na sklady
Store* first_time_empl = nullptr;
Store* established_empl = nullptr;
Store* high_value_empl = nullptr;


int main(int argc, char* argv[]) {

    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " TIME(h) ARRIVAL_RATE ( EXP(ARRIVAL_RATE) ) LOW_EMPL_NUMBER MID_EMPL_NUMBER HIGH_EMPL_NUMBER" << std::endl;
        return 1;
    }

    int TIME_HOURS = std::stoi(argv[1]);
    int CALL_RATE =std::stoi(argv[2]);
    int LOW_EMPL_NUMBER = std::stoi(argv[3]);
    int MID_EMPL_NUMBER = std::stoi(argv[4]);
    int HIGH_EMPL_NUMBER = std::stoi(argv[5]);

    // Inicializace skladů
    first_time_empl = new Store("First-time Customers", LOW_EMPL_NUMBER);
    established_empl = new Store("Established Customers", MID_EMPL_NUMBER);
    high_value_empl = new Store("High-valued Customers", HIGH_EMPL_NUMBER);

    Init(0, TIME_HOURS*60); 

    (new WorkShift)->Activate();
    (new CustomerGenerator(CALL_RATE))->Activate(); // Aktivace jednoduchého procesu na začátku

    Run(); // Spuštění simulace
    first_time_empl->Output();
    established_empl->Output();
    high_value_empl->Output();

    std::cout << "Simulation finished at time: " << Time << std::endl;


    delete first_time_empl;
    delete established_empl;
    delete high_value_empl;
    return 0;
}