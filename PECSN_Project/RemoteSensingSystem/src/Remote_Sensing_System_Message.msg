cplusplus {{
    
}}

namespace remotesensingsystem;


message Remote_Sensing_System_Message extends cMessage{
    double x;	//coordinate x
    double y;	//coordinate y
   	double D;	//range of the sensing node
   	
   	int id;
};
