#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "simlib.h"


extern Store* first_time_empl;
extern Store* established_empl;
extern Store* high_value_empl;


class Customer : public Process {
    public:
        void Behavior()  override;
    
    private:
    // Výčtový typ pro stav zákazníka
    enum CustomerStatus {
        Served,
        WaitingInQueue,
        Left,
        Escalate
    };

    // Deklarace metod pro zpracování zákazníka
    CustomerStatus HandleFirstTimeCustomer();
    CustomerStatus HandleEstablishedCustomer();
    CustomerStatus HandleHighValueCustomer();
    CustomerStatus ServeFirstTimeCustomer();
    CustomerStatus ServeEstablishedCustomer();
    CustomerStatus ServeHighValueCustomer();
    bool QueueDecision();
    bool LeaveDecision();
    void WaitForService(Store& employee_store);
    
   

};

#endif