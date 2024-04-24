#include "Sensing_Node.h"
#include "Remote_Sensing_System_Message_m.h"

#include "RemoteSensingSystemLibrary.h"

namespace remotesensingsystem
{
    Define_Module(Sensing_Node);

    void Sensing_Node::initialize()
       {
           Number_of_sent_messages = registerSignal("Number_of_sent_messages");
           rateTot = registerSignal("rateTot");

           simulation_time = par("simulation_time").doubleValue(); // Get the simulation time limit.
           warm_up_period = getSimulation()->getWarmupPeriod().dbl(); // Get the warm-up time.
           rate_unit = 1/(simulation_time - warm_up_period); // Unit to add when i have to compute rates.

           num_mess_tot = 0;
           num_mess_failed = 0;

           initial_position_x = uniform(0, par("L").intValue());
           initial_position_y = uniform(0, par("H").intValue());

           times = 0;          // Counter of messages to send to the access points during the route.
           remaining_time = 0; // Remaining time due to the route that it isn't a multiple of T seconds.
           counter = 0;        // Counter useful to understand when has transmitted all the messages during the shift from a way-point to another.

           actual_coordinate_x = initial_position_x; // Initialize the starting coordinates.
           actual_coordinate_y = initial_position_y; // Initialize the starting coordinates.

           new_waypoint_x = 0; // Initialize the coordinates of the way-point to reach.
           new_waypoint_y = 0; // Initialize the coordinates of the way-point to reach.

           speed = 0; // Initialize the current speed of the sensing node.

           D = par("D").doubleValue();

           T = par("T").doubleValue();

           Vmax = par("Vmax").doubleValue();
           Vmin = par("Vmin").doubleValue();

           if(Vmax == 0){
               // The sensing node stay firm.
               cMessage *msg = new cMessage("Sensing_Node_Must_Send_a_Message");
               scheduleAt(simTime() + T, msg); // The sensing node must send the first message to the switch.
           }else{
               // The sensing node is moving.
               cMessage *msg = new cMessage("Sensing_Node_Start");
               scheduleAt(simTime() , msg);  // The sensing node must do the first movement.
           }
       }

    // Update the coordinates in the current position, by summing or subtracting a delta of shift
    void Sensing_Node::update_coordinates(double l, double m)
    {
        if ((new_waypoint_x - actual_coordinate_x) < 0)
        {
            actual_coordinate_x = actual_coordinate_x - m;
        }
        else
        {
            actual_coordinate_x = actual_coordinate_x + m;
        }

        if ((new_waypoint_y - actual_coordinate_y) < 0)
        {
            actual_coordinate_y = actual_coordinate_y - l;
        }
        else
        {
            actual_coordinate_y = actual_coordinate_y + l;
        }

        actual_coordinate_x = (actual_coordinate_x);
        actual_coordinate_y = (actual_coordinate_y);
    }

    void Sensing_Node::scheduler()
    {
        for (int i = 0; i < times; i++)
        {   // In a time multiple of the period the node send its message.
            cMessage *msg_new = new cMessage("Sensing_Node_Must_Send_a_Message");
            scheduleAt(simTime() + (i + 1) * T, msg_new);
        }
    }
    // If the result of the distribution is below 1 the movement is negative otherwise positive.
    int Sensing_Node::calculate_negative()
    {
        if (uniform(0, 2) < 1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    // Compute new waypoint and speed.
    void Sensing_Node::computing_coordinates()
    {
        const int min_dist_mov = par("min_distance_movement").intValue();
        const int max_dist_mov = par("max_distance_movement").intValue();

        const double distance_in_x = (calculate_negative() * (uniform(min_dist_mov, max_dist_mov , 0)));
        const double distance_in_y = (calculate_negative() * (uniform(min_dist_mov, max_dist_mov , 1)));

        new_waypoint_x = actual_coordinate_x + distance_in_x;
        new_waypoint_y = actual_coordinate_y + distance_in_y;

        if(new_waypoint_x > par("L").intValue() || new_waypoint_x < 0){
            new_waypoint_x = actual_coordinate_x - distance_in_x;
        }

        if(new_waypoint_y > par("H").intValue() || new_waypoint_y < 0){
            new_waypoint_y = actual_coordinate_y - distance_in_y;
        }

        // Compute the new velocity of the sensing node.

        speed = uniform(Vmin, Vmax);
    }

    void Sensing_Node::new_calculations()
    {
        computing_coordinates();

        // Compute the time to reach the new waypoint from the position where we are.

        double travel_time = distance(actual_coordinate_x ,  actual_coordinate_y , new_waypoint_x , new_waypoint_y);
        double travel_time_t = 0;

        travel_time += remaining_time;
        /*
            Sum the remaining time from the previous movement and we add it to the next travel time.
            Like it was addictional time in which it travels, in a way to consider it like addictional time to transmit.
            Compute the number of times in which hthe node will try to send the broadcast message.
        */
        while (travel_time < T)
        {
            remaining_time = travel_time;
            actual_coordinate_x = new_waypoint_x;
            actual_coordinate_y = new_waypoint_y;

            computing_coordinates();

            travel_time_t = (distance(actual_coordinate_x ,  actual_coordinate_y , new_waypoint_x , new_waypoint_y) / speed) + remaining_time;

            travel_time = travel_time_t;
        }
        // Compute the number of times in which the node will try to send the broadcast message during the new travel
        times = floor(travel_time / T);
        remaining_time = (travel_time / T) - times;
        counter = times;
        // Schedule then tot attempts of Sensing_Node_Must_Send_a_Message to the access points at T seconds of distance
        scheduler();
    }

    void Sensing_Node::handleMessage(cMessage *msg){
        if (strcmp(msg->getName(), "Sensing_Node_Start") == 0){ // It's the self message to start the trasmission.
                delete(msg); // Delete the message for optimize the memory management.
                new_calculations(); // Calculate all the new parameters (speed, new way-point, number of messages to send during the shift).

        }else if(strcmp(msg->getName(), "Sensing_Node_Must_Send_a_Message") == 0){
                // We received a message that indicates that the SS must send a message to the access points.

                if(Vmax > 0){
                    // We must update coordinates of the sensing node.
                    delete(msg); // Delete the message for optimize the memory management.
                    const double alfa = atan(abs(new_waypoint_y - actual_coordinate_y) / abs(new_waypoint_x - actual_coordinate_x));
                    const double k = speed * T; // Mini hypotenuse of the triangle.
                    update_coordinates(/*l*/(k * sin(alfa)), /*m*/(k * cos(alfa))); // Update the coordinates of the sensing node.
                }

                Remote_Sensing_System_Message *msg_r = new Remote_Sensing_System_Message("Sensing_Node_Send_Message_To_Switch");
                msg_r->setX(actual_coordinate_x);
                msg_r->setY(actual_coordinate_y);
                msg_r->setD(D);

                send(check_and_cast<cMessage*>(msg_r), "out"); // Send the message to the switch.

                if(simTime().dbl() >= warm_up_period){
                    // If the warm-up period is passed => Calculate statistics.
                    emit(Number_of_sent_messages, 1);
                    emit(rateTot, rate_unit);
                    num_mess_tot++; // I just sended a message.
                }

                if(Vmax > 0){
                    // I just sended a message.
                    counter--; // so i decrease the message counter of messages to send.
                    if (counter <= 0){
                        // I just sent the last message in this path.
                        new_calculations(); // Compute my new position.
                    }
                }else{
                    scheduleAt(simTime() + T, msg); // Resend the message to myself.
                }
            }
        }

};
