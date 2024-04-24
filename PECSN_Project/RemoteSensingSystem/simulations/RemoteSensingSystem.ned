package remotesensingsystem.simulations;

import remotesensingsystem.Switch;
import remotesensingsystem.SinkNode;
import remotesensingsystem.AccessPoint;
import remotesensingsystem.Sensing_Node;

network RemoteSensingSystem
{
    parameters:
        int M = default(140); //number of Access Point
        int N = default(1000); //number of Sensing Node
          
        //number of messages transmitted with success
        @signal[Number_of_Successful_Messages](type=int);
        @statistic[Number_of_Successful_Messages_Stat](source="Number_of_Successful_Messages";record=sum;);  
                
        // number of unique messages that have arrived to the sink
        @signal[Number_of_unique_messages_successfully_arrived](type=int); // 2kr 
        @statistic[Number_of_unique_messages_successfully_arrived_Stat](source="Number_of_unique_messages_successfully_arrived";record=sum;);       
        
        //number of total messages transmitted by sensing nodes
        @signal[Number_of_sent_messages](type=int);
        @statistic[Number_of_sent_messages_Stat](source="Number_of_sent_messages";record=sum;);
        
        //number of total messages transmitted by sensing nodes and don't reach access points due to the distance
        @signal[Number_of_Messages_sent_to_nobody](type=int);
        @statistic[Number_of_Messages_sent_to_nobody_stat](source="Number_of_Messages_sent_to_nobody";record=sum;); 
        
        //number of total messages transmitted by sensing nodes and don't reach access points due to the probability
        @signal[Number_of_failed_receptions](type=int);
        @statistic[Number_of_failed_receptions_Stat](source="Number_of_failed_receptions";record=sum;); 
        
        // number of messages that have not been duplicated
        @signal[Number_of_messages_that_has_not_been_duplicated](type=int);
        @statistic[Number_of_messages_that_has_not_been_duplicated_Stat](source="Number_of_messages_that_has_not_been_duplicated";record=sum;); 
        
        // number of duplicates messages that have circulated in the system
        @signal[Number_of_duplicates](type=int);
        @statistic[Number_of_duplicates_Stat](source="Number_of_duplicates";record=sum;); 
          
        // number of unique messages that have arrived to the sink
        @signal[Number_of_duplicate_messages_successfully_arrived](type=int);
        @statistic[Number_of_duplicate_messages_successfully_arrived_Stat](source="Number_of_duplicate_messages_successfully_arrived";record=sum;);  
       	
       	// average number of times that a message has been duplicated.
       	@signal[Average_duplicates_per_message](type=double); // 2kr  
        @statistic[Average_duplicates_per_message_Stat](source="Average_duplicates_per_message";record=sum;);      
        
        //equal to the first but divided by the time
        @signal[rateSucc](type=double);
        @statistic[rateSuccStat](source="rateSucc"; record=sum;);
        
        //equal to the third but divided by the time
        @signal[rateEmpty](type=double);
        @statistic[rateEmptyStat](source="rateEmpty"; record=sum;);
        
        //equal to the second but divided by the time
        @signal[rateTot](type=double);
        @statistic[rateTotStat](source="rateTot"; record=sum;);
        
        //equal to the fourth but divided by the time
        @signal[rateFailed](type=double);
        @statistic[rateFailedStat](source="rateFailed"; record=sum;);
        
        //equal to the fourth but divided by the time
        @signal[rateNonDuplicated](type=double);
        @statistic[rateNonDuplicatedStat](source="rateNonDuplicated"; record=sum;);

    submodules:
        switch: Switch {
            @display("i=block/fork;p=1043,951;is=vl;");
        }

        sink: SinkNode {
            @display("i=device/server;p=1843.6799,961.92;is=vl");
        }

        accesspoints[M]: AccessPoint {
            @display("i=device/accesspoint;is=vl;");  	   
        }

        sensingnodes[N]: Sensing_Node {
            @display("i=device/cellphone;is=vl;");
        }

    //connections using vectors
    connections:
        
        for i=0..M-1 {
            accesspoints[i].out_sink --> {  delay = 1ms; } --> sink.in++;
            accesspoints[i].out_switch --> {  delay = 1ms; } --> switch.in_access_point++;
        }

        for i=0..N-1 {
            sensingnodes[i].out --> {  delay = 1ms; } --> switch.in_sensing_nodes++;
        }

        for i=0..M-1 {
            switch.out++ --> {  delay = 1ms; } --> accesspoints[i].in;
        }
}
