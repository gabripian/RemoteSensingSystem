#include "SinkNode.h"
#include "Remote_Sensing_System_Message_m.h"
#include "RemoteSensingSystemLibrary.h"
#include <algorithm>
namespace remotesensingsystem {

Define_Module(SinkNode);

SinkNode::SinkNode(){

}

void SinkNode::initialize()
{
    Number_of_Successful_Messages = registerSignal("Number_of_Successful_Messages");
    Number_of_unique_messages_successfully_arrived  = registerSignal("Number_of_unique_messages_successfully_arrived");
    Number_of_duplicate_messages_successfully_arrived  = registerSignal("Number_of_duplicate_messages_successfully_arrived");
    rateSucc = registerSignal("rateSucc");

    N = par("N").intValue();

    simulation_time = par("simulation_time").doubleValue(); // get the simulation time limit.
    warm_up_period = getSimulation()->getWarmupPeriod().dbl(); // Get the warm-up time.
    rate_unit = 1/(simulation_time - warm_up_period); // unit to add when i have to compute rates.

    num_mess = 0;

    buffer_limit = floor(N * (par("buffer_limit").doubleValue()));
    buffer_cut = floor(buffer_limit * ((par("buffer_cut").doubleValue()) / 100));

    if(buffer_limit < 20){
        buffer_limit = 20; // Minimum size of the buffer.
    }

    if(buffer_cut < 1){
        buffer_cut = 1; // Minimum size of buffer_cut.
    }

    print_mess_rate = par("print_mess_rate").boolValue();
    num_mess_rate = 0;
    rate_period = par("rate_period").doubleValue();

    if(print_mess_rate == true){
        cMessage *msg = new cMessage("SinkNode_Period_Message");
        scheduleAt(simTime() + rate_period , msg);
        //EV << rate_period << endl;
    }
}

bool SinkNode::find_id(unsigned int id){
    if(this->find_id_return_index(id) == -1){
        return false;
    }else{
        return true;
    }
}

int SinkNode::find_id_return_index(unsigned int id){
    // Returns index of id, or -1 if not found
    // the array is ordered, so i can use a Binary Search.
    const unsigned int size = this->received_messages_id.size();

    if(size == 0){
        return -1;
    }else if(size == 1){
        if(this->received_messages_id.at(0) == id)
            return 0;
        else{
            return -1;
        }
    }else{
        unsigned int low = 0;
        unsigned int high = size - 1;
        unsigned int mid = 0 , m = 0;
        unsigned int l = this->received_messages_id.at(low);
        unsigned int h = this->received_messages_id.at(high);

        while (l <= id && h >= id) {

            mid = (low + high)/2;
            m = this->received_messages_id.at(mid);

            if (m < id) {
                low = mid + 1;
                l = this->received_messages_id.at(low);
            } else if (m > id) {
                high = mid - 1;
                h = this->received_messages_id.at(high);
            } else {
                return mid; // Element found.
            }
        }
        if (this->received_messages_id.at(low) == id){
            return low; // Element found.
        }else{
            return -1; // Element not found.
        }
    }
}

bool SinkNode::isOrdered(){
    unsigned int size = this->received_messages_id.size();
    if(size <= 1){
        return true;
    }else{
        for(unsigned int i = 0 ; i < (size - 1); i++){
            if(this->received_messages_id.at(i) > this->received_messages_id.at(i + 1)){
                return false;
            }
        }
        return true;
    }
}

void SinkNode::handleMessage(cMessage *msg)
{
    if((strcmp(msg->getName() , "SinkNode_Period_Message") == 0) && print_mess_rate == true){
        WriteFile("output.csv" , (num_mess_rate/rate_period) , floor(simTime().dbl()));
        num_mess_rate = 0;
        scheduleAt(simTime() + rate_period , msg);
    }else{
        Remote_Sensing_System_Message* msg_rem;
        msg_rem = check_and_cast<Remote_Sensing_System_Message*>(msg); // check and cast.

        const unsigned int id_msg = msg_rem->getId(); // the id of the just arrived message.

        bool dup = false;

        if(find_id(id_msg) == true){ //  If the ID is present in the array of the ID's.
            dup = true;  // Then the message is a duplicate.
        }else{
            dup = false; // Otherwise the message is not a duplicate, it's the first.
            this->received_messages_id.push_back(id_msg); // And i insert the ID in the array.
        }

        delete(msg_rem); // Delete the message sent by the access point for optimize the memory management.

        num_mess_rate++;

        if(simTime().dbl() >= warm_up_period){
            emit(Number_of_Successful_Messages, 1);
            emit(rateSucc, rate_unit);
            num_mess++;  // Increment the counter of messages received by the sink node.

            if(dup == false){
                // The message was NOT a duplicate.
                emit(Number_of_unique_messages_successfully_arrived, 1); // I just received a unique message.
            }else{
                // The message was a duplicate.
                emit(Number_of_duplicate_messages_successfully_arrived , 1); // I just received a duplicate message.
            }
       }

        if(this->received_messages_id.size() > buffer_limit){
            /*
            If buffer is too big, then i cut the first 'buffer_cut' elements.
            I do this operation for otimization reasons, becuase the array must be short for optimize the "find_id(id_msg)".

            Moreover, it does not need to be big, because ID's are incremental and an ID can't return a second time during a simulation.
            An ID can return only in the case of duplicate messages, but the trasmission period is in the order of seconds, and a duplicate can't
            be received by the sink
            */
            this->received_messages_id.erase(this->received_messages_id.begin() , this->received_messages_id.begin() + buffer_cut);
        }
    }
}
};
