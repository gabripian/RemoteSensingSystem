
package remotesensingsystem;


simple Sensing_Node
{
    parameters:
        int H @unit(m) = default(6000m);
        int L @unit(m) = default(6000m);
        double D  @unit(m) = default(100m); 
        double T @unit(s) = default(2s);
                 
        double Vmax = default(5);
        double Vmin = default(1);
        
        int max_distance_movement @unit(m) = default(10m);
        int min_distance_movement @unit(m) = default(1m);
        
        double simulation_time @unit(s) = default(500s);
        
       
        
    gates:
        output out;
}
