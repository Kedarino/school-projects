#include "customer.h"
#include <iostream>


#define FIRST_PERCETNAGE 0.3
#define ESTABLISHED_PERCENTAGE 0.5
// HIGH_PECENTAGE = 1 - (FIRST + EESTABLISHED)

#define DECIDE_TO_QUEUE 0.6
#define DECIDE_TO_LEAVE 0.8

#define FIRST_ESCALATE 0.3
#define ESTABLISHED_ESCALATE 0.2


void Customer::Behavior(){

    std::cout << "Caller has entered the system" << std::endl;
    double customer_type_prob = Random();
    CustomerStatus status;
    // Nový zákazník
    if (customer_type_prob < FIRST_PERCETNAGE) {
        // TODO: Popířpadě předělat aby šel přímo do fronty
        status = HandleFirstTimeCustomer();
        if(status == Escalate) {
            status = ServeEstablishedCustomer();
            if(status == Escalate) {
                ServeHighValueCustomer();
            }
        }
    }

    // Zavedený zákazník
    else if(customer_type_prob < (FIRST_PERCETNAGE + ESTABLISHED_PERCENTAGE)) {
        status = HandleEstablishedCustomer();
        if(status == Escalate) {
           status = ServeHighValueCustomer();
        }
    }

    // Vážený zákazník
    else {
        status = HandleHighValueCustomer();
    }
};

Customer::CustomerStatus Customer::HandleFirstTimeCustomer() {
    WaitForService(*first_time_empl);
    return ServeFirstTimeCustomer();
}

Customer::CustomerStatus Customer::ServeFirstTimeCustomer() {
    CustomerStatus status = Served;

    //Obsluha zákazníka
    Enter(*first_time_empl, 1);
    Wait(Uniform(1,2));
    double prob_escalate = Random();
    //Vyhodnocení problému
    if(prob_escalate < (1 - FIRST_ESCALATE)) {
        //Zákazník obsloužen
        Wait(Uniform(5,10));
    }
    else {
        // Customer needs escalation
        status = Escalate;
    }
    Leave(*first_time_empl,1);

    return status;
    
}


Customer::CustomerStatus Customer::HandleEstablishedCustomer() {
    WaitForService(*established_empl);
    return ServeEstablishedCustomer();
    
}

Customer::CustomerStatus Customer::ServeEstablishedCustomer() {
    CustomerStatus status;
   
    Enter(*established_empl, 1);
    Wait(Uniform(5,10));

    double prob_escalate = Random();

    if(prob_escalate < (1 - ESTABLISHED_ESCALATE)) {
        status = Served;
        Wait(Uniform(5,10));
    }
    else {
        status = Escalate;
    }
    Leave(*established_empl,1);

    return status;

   
}

Customer::CustomerStatus Customer::HandleHighValueCustomer() {
    WaitForService(*high_value_empl);
    return ServeHighValueCustomer();
}

Customer::CustomerStatus Customer::ServeHighValueCustomer() {
    

    Enter(*high_value_empl,1);
    Wait(Uniform(4,8));
    Leave(*high_value_empl,1);

    return Served;



}

void Customer::WaitForService(Store& employee_store) {
    while (true) {
        // Pokud nejsou k dispozici volní operátoři nebo není prázdní fornta která mmá přednost
        if (employee_store.Full()) {
            // Zákazník se rozhodne, zda se zařadí do fronty
            if (QueueDecision()) {
                break;
            }
            else if (LeaveDecision()) {
                Cancel();
                break;
            }
            else {
                Wait(Uniform(30, 60));
            }
        }
        else {
            // Jsou k dispozici volní operátoři
            break;
        }
    }
}

//Funkce vrátí true když se zákazník rozhodne zařadit do fronty
bool Customer::QueueDecision() {
    double prob_q = Random();
    if(prob_q < DECIDE_TO_QUEUE){
        return true;
    }
    return false;
}


bool Customer::LeaveDecision() {
    double prob_l = Random();
    if(prob_l < DECIDE_TO_LEAVE) {
        return true;
    }
    return false;
}




