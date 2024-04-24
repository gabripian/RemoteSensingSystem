#ifndef __REMOTESENSINGSYSTEM_ACCESSPOINT_H
#define __REMOTESENSINGSYSTEM_ACCESSPOINT_H

#include <omnetpp.h>

using namespace omnetpp;

namespace remotesensingsystem {

class AccessPoint : public cSimpleModule
{
    private:
        double x;       //coordinate x
        double y;       //coordinate y
        double Psucc;
        simsignal_t Number_of_failed_receptions; //N umber of total messages sent by nodes that don't reach the sink node due to the probability
        simsignal_t rateFailed;
        unsigned int num_mess_failed;
        double rate_unit;
        double simulation_time; // The simulation-time limit, used to compute statistics.
        double warm_up_period; // The warm-up period.

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        AccessPoint();
};

};

#endif
