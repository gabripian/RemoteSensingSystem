#include "Switch.h"
#include "Remote_Sensing_System_Message_m.h"

#include "RemoteSensingSystemLibrary.h"

namespace remotesensingsystem {

Define_Module(Switch);

Switch::Switch(){}

void Switch::initialize()
{
    Number_of_Messages_sent_to_nobody = registerSignal("Number_of_Messages_sent_to_nobody");
    Number_of_messages_that_has_not_been_duplicated = registerSignal("Number_of_messages_that_has_not_been_duplicated");
    Number_of_duplicates = registerSignal("Number_of_duplicates");
    Average_duplicates_per_message = registerSignal("Average_duplicates_per_message");
    rateEmpty = registerSignal("rateEmpty");
    rateNonDuplicated = registerSignal("rateNonDuplicated");

    simulation_time = par("simulation_time").doubleValue(); // get the simulation time limit.
    warm_up_period = getSimulation()->getWarmupPeriod().dbl(); // Get the warm-up time.
    rate_unit = 1/(simulation_time - warm_up_period); // unit to add when i have to compute rates.

    M = par("M").intValue();
    L = par("L").intValue();
    H = par("H").intValue();
    num_mess = par("num_mess").intValue();

    number_of_empty_messages = 0; // Number of unique messages that no access points received.
    number_of_unique_messages = 0; // Number of unique messages that the switch has seen, starts form 0.

    if(M > 0){
        const double increment_unit = H / sqrt(M); // Increment unit used in the generation of the position of access points.

        double limit_x_above = increment_unit;
        double limit_x_below = 0;
        double limit_y_above = increment_unit;
        double limit_y_below = 0;

        // We have to generate the coordinates of M access points.

        unsigned int conf_acc = 0; // How to generate the coordinates?

        if( strcmp( par("access_point_position")  , "deterministic") == 0 ){
            conf_acc = 0;
            /*
                I divide the working plane in M slots, each slot will have only one access point.
                Each slot is a square.
                The position INSIDE the slot is deterministic and it is the center of the square.

                Example:
                How the working plane will be divided if M=4:
                 +--------+--------+
                 |        |        |
                 |    1   |   2    |
                 |        |        |
                 +--------+--------+
                 |        |        |
                 |    3   |   4    |
                 |        |        |
                 +--------+--------+
             */
        }else if( strcmp( par("access_point_position")  , "variable") == 0 ){
            conf_acc = 1;
            /*
            I divide the working plane in M slots, each slot will have one access point.
            The position INSIDE the slot is randomly generated.
            */

        }else if( strcmp( par("access_point_position")  , "random") == 0 ){
            conf_acc = 2;
            /*
            The position of the access point is completely random.
            The position is bounded only by the working plane.
            */
        }else{
            conf_acc = 1;  // The default option (variable).
        }

        for(unsigned int i = 0 ; i < M ; i++){
            if(conf_acc == 0){
                this->x_ap_coordinates.push_back((limit_x_above + limit_x_below)/2); // Take the mid point of the boundaries.
                this->y_ap_coordinates.push_back((limit_y_above + limit_y_below)/2);
            }else if(conf_acc == 1){
                this->x_ap_coordinates.push_back(uniform(limit_x_below , limit_x_above , 0));
                this->y_ap_coordinates.push_back(uniform(limit_y_below , limit_y_above , 1));
            }else if(conf_acc == 2){
                this->x_ap_coordinates.push_back(uniform(0 , L));
                this->y_ap_coordinates.push_back(uniform(0 , H));
            }

            /*
            How to determine the pawn area of the access points.
                 +--------+--------+
                 |        |        |
                 |    1   |   2    |
                 |        |        |
                 +--------+--------+
                 |        |        |
                 |    3   |   4    |
                 |        |        |
                 +--------+--------+

            The algorithm:
                 1) We start from the square 1.
                 2) We generate an access point inside the square 1.
                 3) We move to the right in the square 2.
                 4) We generate an access point inside the square 2.
                 5) We return to the left and we go down of one row, so we move to the square 3.
                 6) We generate an access point inside the square 1.
                 7) We move to the right in the square 4.
                 8) We generate an access point inside the square 4.
             */

            if(limit_x_above < L){
                limit_x_below = limit_x_above;  // Swipe right along the row.
                limit_x_above += increment_unit;

                if(limit_x_above > L){ // If out of bound.
                    limit_x_above = L; // Correction.
                }

            }else if(limit_x_above >= L){

                limit_x_below = 0;  // Return to left of the matrix.
                limit_x_above = increment_unit;

                limit_y_below = limit_y_above; // Swipe down of 1 row.
                limit_y_above += increment_unit;

                if(limit_y_above > H){ // If out of bound.
                    limit_y_above = H; // Correction.
               }

            }
        }
    }
}

void Switch::handleMessage(cMessage *msg)
{
        Remote_Sensing_System_Message* msg_rem;
        msg_rem = check_and_cast<Remote_Sensing_System_Message*>(msg); // check and cast.

        if(strcmp(msg_rem->getName() , "Sensing_Node_Send_Message_To_Switch") == 0){
            // The switch just received a message from a sensing node.
            const double x_msg = msg_rem->getX(); // The x coordinate of the sensing node.
            const double y_msg = msg_rem->getY(); // The y coordinate of the sensing mode.
            const double D_msg = msg_rem->getD(); // The trasmission range of the sensing node that sent this message.

            double number_of_sent = 0; // The number of times that this message has been forwarded to access points.

            if(M > 0 && D_msg > 0){
                for(unsigned int i = 0 ; i < M; i++){  // For each access point.
                       // Compute the distance between the access point and the sensing node.
                        if((distance((this->x_ap_coordinates.at(i)) , (this->y_ap_coordinates.at(i)) , x_msg , y_msg)) <= D_msg){
                            // If the Access point 'i' is in the trasmission range of the sensing node that sent this message.
                            Remote_Sensing_System_Message *msg_f = new Remote_Sensing_System_Message("Sensing_Node_Send_Message_To_Switch");
                            msg_f->setX(x_msg); // Copy the message attributes.
                            msg_f->setY(y_msg); // Copy the message attributes.
                            msg_f->setD(D_msg); // Copy the message attributes.
                            msg_f->setId(number_of_unique_messages); // All duplicates of a messages have the same ID.
                            send(check_and_cast<cMessage*>(msg_f) , "out" , i);  // The switch forwards the message to the access point 'i'.
                            number_of_sent++; // I sent this message one more time.
                        }
                }
            }

            number_of_unique_messages++; // Update the number of "unique" messages that the switch has seen.

            if(simTime().dbl() >= warm_up_period){
                // If warmup period is passed:
                if(number_of_sent == 0){
                   // No access points received this message.
                   number_of_empty_messages++;
                   emit(Number_of_Messages_sent_to_nobody, 1); // I got a message that has not been received by any access point.
                   emit(rateEmpty, rate_unit);
                }else if(number_of_sent == 1){
                    // Only one access point received this message.
                   emit(Number_of_messages_that_has_not_been_duplicated, 1); // I got a message that has not been received by any access point.
                   emit(rateNonDuplicated, rate_unit);
                }else if(number_of_sent > 1){
                    // The message has been received by AT LEAST 2 different access points, so i generated at least 1 duplicate.
                   emit(Number_of_duplicates, (number_of_sent-1));
                   emit(Average_duplicates_per_message, (number_of_sent-1)/num_mess);
                }
            }
        }
        delete(msg_rem); // Delete the message sent by the sensing node, for optimize the memory.
}

};
