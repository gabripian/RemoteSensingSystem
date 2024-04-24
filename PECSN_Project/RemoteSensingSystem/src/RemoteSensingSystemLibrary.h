#ifndef __REMOTESENSINGSYSTEM_LIBRARY_H
#define __REMOTESENSINGSYSTEM_LIBRARY_H

#include <cmath>
#include <fstream>
#include <string>

namespace remotesensingsystem{

    static double cut_decimals(double number , unsigned int how_many_decimals = 2){
        const unsigned int dec = pow(10 , how_many_decimals);
        return (std::ceil(number * dec) /dec);
    }

    static int floor(double number){
            return (int)std::floor(number);
    }

    static double distance(double ax , double ay , double bx , double by){
        return sqrt( pow((bx - ax), 2) + pow((by - ay), 2) );
    }

    static void WriteFile(std::string file_name, double printValue){
        //open file in append
        std::ofstream file(file_name, std::ios::app);

        if(file.is_open()){
           std::string stringValue = std::to_string(printValue);
           stringValue[stringValue.find('.')] = ',';

           file << stringValue << std::endl;

           file.close();
     }else{
           EV << "Impossible open the file!" << std::endl;
     }
    }

    static void WriteFile(std::string file_name, double printValue1 , unsigned int printValue2){
            //open file in append
        // if the file doesn not exists, it will be created.
            std::ofstream file(file_name, std::ios::app);

            if(file.is_open()){
               std::string stringValue = std::to_string(printValue1);
               stringValue[stringValue.find('.')] = ',';


               //EV << "Ao " << stringValue << endl;

               file << printValue2 <<";"<< stringValue << std::endl;

               file.close();
         }else{
               EV << "Impossible open the file!" << std::endl;
         }
        }

    static void WriteFile(std::string file_name, std::string stringValue){
            //open file in append
            std::ofstream file(file_name, std::ios::app);

            if(file.is_open()){
               size_t pos = stringValue.find('.');

               stringValue[pos] = ',';

               //EV << "Ao " << stringValue << endl;

               //file << "rateMeanTot "<<";"<< stringValue << std::endl;

               file.close();
         }else{
               EV << "Impossible open the file!" << std::endl;
         }
        }
};
#endif

