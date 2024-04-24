

#ifndef __PROGETTO_TCX_H
#define __PROGETTO_TCX_H

#include <omnetpp.h>


using namespace omnetpp;

namespace remotesensingsystem {


class Sensing_Node : public cSimpleModule
{
private:
  double initial_position_x;   //starting coordinate x
  double initial_position_y;   //starting coordinate y

  double actual_coordinate_x;  //current coordinate x
  double actual_coordinate_y;  //current coordinate y

  double new_waypoint_x;  //next way-point coordinate x
  double new_waypoint_y;  //next way-point coordinate y

  double remaining_time; //remaining time, after that the node must transmit

  double D;   //range of trasmission of the sensing node

  int counter; //number of messages to send

  double speed; // current speed of the sensing node.

  double T;  //period of trasmission of the sensing node.

  double Vmax;  // maximum speed of the sensing node.
  double Vmin;  // minimum speed of the sensing node.

  int times;

  double rate_unit; // number used to compute statistics.

  simsignal_t Number_of_sent_messages;  // will contain number of total messages sent by sensing nodes.
  simsignal_t rateTot; //equal to Number_of_sent_messages divided by the time.

  double simulation_time; // The simulation-time limit, used to compute statistics.
  double warm_up_period; // The warm-up period.

  unsigned int num_mess_tot;  //number of total messages sent by nodes.
  unsigned int num_mess_failed; //number of total messages sent by nodes that don't reach the sink node due to the probability.

  int id;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  private:
    int calculate_negative();
    virtual void new_calculations();
    virtual void computing_coordinates();
    virtual void scheduler();
    virtual void update_coordinates(double l, double m);
};

}; // namespace

#endif
