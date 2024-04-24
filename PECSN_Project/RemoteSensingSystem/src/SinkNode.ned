package remotesensingsystem;

simple SinkNode
{   parameters:
      double simulation_time @unit(s) = default(500s);
      int N = default(1000); // number of sensing nodes.
      double buffer_limit = default(1.5); //buffer_limit = 1 x N.
      double buffer_cut = default(10); // 10% of buffer_limit.
      
      bool print_mess_rate = default(false);
      
      double rate_period @unit(s) = default(2s);
    	 
    gates:
        input in[];  
}