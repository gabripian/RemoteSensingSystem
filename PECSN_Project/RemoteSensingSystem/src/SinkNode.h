#ifndef __REMOTESENSINGSYSTEM_SINKNODE_H
#define __REMOTESENSINGSYSTEM_SINKNODE_H

#include <omnetpp.h>
#include <vector>

using namespace omnetpp;

namespace remotesensingsystem {

class SinkNode : public cSimpleModule
{
    private:
        simsignal_t Number_of_Successful_Messages; //will contains total number of messages that arrive to the sink node
        simsignal_t Number_of_unique_messages_successfully_arrived;
        simsignal_t Number_of_duplicate_messages_successfully_arrived;
        simsignal_t rateSucc;           //equal to the previous divided by the time

        double simulation_time; // The simulation-time limit, used to compute statistics.
        double warm_up_period; // The warm-up period.

        unsigned int N;
        unsigned int buffer_limit;
        unsigned int buffer_cut;

        double rate_unit;

        std::vector<unsigned int> received_messages_id;

        unsigned int num_mess;          //total number of messages that arrive to the sink node
        unsigned int num_mess_rate;
        bool print_mess_rate;
        double rate_period;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    private:
        virtual bool find_id(unsigned int id);
        virtual int find_id_return_index(unsigned int id);
        virtual bool isOrdered();

    public:
        SinkNode();
};

};

#endif
