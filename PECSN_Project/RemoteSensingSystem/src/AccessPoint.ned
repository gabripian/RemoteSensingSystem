package remotesensingsystem;

simple AccessPoint
{
    parameters:	
        int H @unit(m) = default(1000m);
        int L @unit(m) = default(1000m);
        double Psucc = default(0.7);  
        double simulation_time @unit(s) = default(500s);
        
       
        
    gates:
        input in;
        output out_sink;
        output out_switch;
    
}
