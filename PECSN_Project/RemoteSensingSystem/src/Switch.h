#ifndef __REMOTESENSINGSYSTEM_SWITCH_H
#define __REMOTESENSINGSYSTEM_SWITCH_H

#include <omnetpp.h>
#include <vector>

using namespace omnetpp;

namespace remotesensingsystem {

class Switch : public cSimpleModule
{
    private:
        std::vector<double> x_ap_coordinates; // Coordinates x of access points.
        std::vector<double> y_ap_coordinates; // Coordinates y of access points.

        simsignal_t Number_of_Messages_sent_to_nobody; // Will contains number of messages that doen't reach access point due to the distance
        simsignal_t Number_of_duplicates;
        simsignal_t Number_of_messages_that_has_not_been_duplicated;
        simsignal_t Average_duplicates_per_message;
        simsignal_t rateEmpty; // Equal to the previous divided by the time.
        simsignal_t rateNonDuplicated;

        unsigned int number_of_empty_messages; // Number of messages that doen't reach access point due to the distance

        double simulation_time; // The simulation-time limit, used to compute statistics.
        double warm_up_period; // The warm-up period.
        double rate_unit;

        unsigned int M;

        unsigned int L;
        unsigned int H;

        unsigned int num_mess;

        unsigned int number_of_unique_messages;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        Switch();
};

};

#endif
