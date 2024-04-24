package remotesensingsystem;

simple Switch
{   
	parameters:	
        int M = default(121);
        int H @unit(m) = default(6000m);
        int L @unit(m) = default(6000m);
        double simulation_time @unit(s) = default(201s);
        int num_mess = default(2000);
        
        string access_point_position = default("varible");
             
    gates:
        input in_sensing_nodes[];
        input in_access_point[];
        output out[]; 
}
