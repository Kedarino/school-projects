#include "customer.h"
#include <iostream>

// Pravděpodobnost, že zákazník je "nový"
#define FIRST_PERCETNAGE 0.3

// Pravděpodobnost, že zákazník je "zavedený"
#define ESTABLISHED_PERCENTAGE 0.5

// HIGH_PECENTAGE = 1 - (FIRST + EESTABLISHED)

// Pravděpodobnost, že zákazník se rozhodne čekat ve frontě
#define DECIDE_TO_QUEUE 0.6
// Pravděpodobnost, že zákazník se rozhodne odejít
#define DECIDE_TO_LEAVE 0.8

//Pravděpodobnost eskalace pro "nového" zákazníka
#define FIRST_ESCALATE 0.3
// Pravděpodobnost eskalace pro "zavedeného" zákazníka
#define ESTABLISHED_ESCALATE 0.2


void Customer::Behavior(){

    // Náhodně určujeme typ zákazníka
    double customer_type_prob = Random();
    CustomerStatus status;
    // Nový zákazník
    if (customer_type_prob < FIRST_PERCETNAGE) {
        // Zpracování nového zákazníka
        status = HandleFirstTimeCustomer();
        if(status == Escalate) {
            // Eskalace na obsluhu "zavedeného" zákazníka
            status = ServeEstablishedCustomer();
            if(status == Escalate) {
                // Eskalace na obsluhu "váženého" zákazníka
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
    // Čekání na volného operátora pro nového zákazníka
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
    // Čekání na volného operátora pro zavedeného zákazníka
    WaitForService(*established_empl);
    return ServeEstablishedCustomer();
    
}

Customer::CustomerStatus Customer::ServeEstablishedCustomer() {
    CustomerStatus status;
   
    //Obsluha zákazníka
    Enter(*established_empl, 1);
    Wait(Uniform(5,10));

    double prob_escalate = Random();
    //Vyhodnocení problému
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
    // Čekání na volného operátora pro váženého zákazníka
    WaitForService(*high_value_empl);
    return ServeHighValueCustomer();
}

Customer::CustomerStatus Customer::ServeHighValueCustomer() {
    
    //Obsluha zákazníka
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
            // Rozhodnutí zákazníka - odejít
            else if (LeaveDecision()) {
                Cancel();
                break;
            }
            // Zákazník zavolá znovu
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




