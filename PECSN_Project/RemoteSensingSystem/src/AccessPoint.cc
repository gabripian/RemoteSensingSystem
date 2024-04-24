#include "AccessPoint.h"
#include "Remote_Sensing_System_Message_m.h"

namespace remotesensingsystem {

Define_Module(AccessPoint);

AccessPoint::AccessPoint(){
}

void AccessPoint::initialize()
{
    Psucc = par("Psucc").doubleValue();

    simulation_time = par("simulation_time").doubleValue(); // get the simulation time limit.
    warm_up_period = getSimulation()->getWarmupPeriod().dbl(); // Get the warm-up time.
    rate_unit = 1/(simulation_time - warm_up_period); // unit to add when i have to compute rates.

    Number_of_failed_receptions = registerSignal("Number_of_failed_receptions");
    rateFailed = registerSignal("rateFailed");

    num_mess_failed = 0;
}

void AccessPoint::handleMessage(cMessage *msg)
{
    if(uniform(0, 1 , 0) <= Psucc){ //if the random number is less or equal of Psucc so the message is correctly sent
        send(msg, "out_sink"); // forward the message to the sink node.
    }else{
        if(simTime().dbl() >= warm_up_period){
            emit(Number_of_failed_receptions, 1); // Psucc test failed, so i increase the number of failed receptions.
            emit(rateFailed, rate_unit);
            //num_mess_failed++;
        }
        delete(msg);
    }
}

};
