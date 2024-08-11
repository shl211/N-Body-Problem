#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <exception>
#include "Body.h"
#include "OrbitSystem.h"

using namespace std;

int main(){

    //define variables
    double vG;
    double vT;
    double vStep;
    
    //create object for orbital system of bodies
    OrbitSystem vSystem;

    try{    
        //load file
        ifstream vDataFile("parameters.txt");

        //while data file is good, read data
        if (vDataFile.good()) {

            //define temporary variables to read data
            string vLine, vTempCount;
            string vTempX, vTempY, vTempXDot, vTempYDot, vTempMass;
            string vTemp1,vTemp2,vTemp3;

            //read first line of data
            getline(vDataFile, vLine);
            stringstream vLineStream(vLine);

            //verify data of sufficient length
            unsigned int count = 0;
            while (vLineStream >> vTempCount) {
                count++;
            }

            if (count != 3) {
                throw logic_error("Insufficient input. Make sure first line of file has three values denoting gravitational constant, time and time step");
            }

            //recreate stream object and assign data
            stringstream vLineStream1(vLine);
            vLineStream1 >> vTemp1 >> vTemp2 >> vTemp3;
            
            //verify and assign data
            if (stod(vTemp1) > 0) {
                vG = stod(vTemp1);
            }
            else {
                throw logic_error("Graviational constant must be greater than 0");
            }

            if (stod(vTemp2) > 0) {
                vT = stod(vTemp2);
            }
            else {
                throw logic_error("Time must be bigger than 0");
            }
            
            if (stod(vTemp3) > 0) {
                vStep = stod(vTemp3);
            }
            else {
                throw logic_error("Step size must be greater than 0");
            }
  
            if (vStep > vT) {
                throw logic_error("Step size cannot be greater than the time");
            }
            
            vSystem.updateG(vG);//update G of system

            while (getline(vDataFile, vLine)) {
                //read line and assign data
                stringstream vLineStream2(vLine);

                //verify data of sufficient length
                count = 0;
                while (vLineStream2 >> vTempCount) {
                    count ++;
                }

                if (count == 0) {
                    break;//terminate while loop if no more data
                }
                else if (count != 5) {
                    throw logic_error("Insufficient data to define body. There should be five elements per row defining X,Y,XDot,YDot,Mass");
                }

                //recreate stringstream and assign data
                stringstream vLineStream3(vLine);
                vLineStream3 >> vTempX >> vTempY >> vTempXDot >> vTempYDot >> vTempMass;

                Body vBodyTemp (stod(vTempX), stod(vTempY), stod(vTempXDot), stod(vTempYDot), stod(vTempMass));//define temporary body
                
                vSystem.addBody(vBodyTemp);//add body to system
        
            }
        
            vDataFile.close();
            cout << "File has been read and data assigned..." << endl;
        }  
        
        //if data file failed to open, throw exception
        else {
            throw logic_error("'parameters.txt' file failed to open. File must be named 'parameters.txt'");
        }        
        

        
        //define variable for current condition
        vector <double> vCurrentCondition;
        double vCurrentTime = 0.0;

        //get current condition [X Y X' Y']... of system and define total bodies in system (4 terms in current condition vector per body)
        vSystem.getSystemCondition(vCurrentCondition);
        const unsigned int vTotalBodies = vCurrentCondition.size()/4;

        ofstream vOutputFile("output.txt",ios::out | ios::trunc);//create file for output
    
        //output data file
        if (vOutputFile.good()) {
            //assign initial data
            for (unsigned int i = 0 ; i < vTotalBodies ;  ++i) {
                //start body index from 1 rather than 0 so more natural for user
                vOutputFile << i + 1 << " ";
                vOutputFile << vSystem.getTime() << " ";
                vOutputFile << vCurrentCondition[4*i] << " ";
                vOutputFile << vCurrentCondition[4*i + 1] << " ";
                vOutputFile << vCurrentCondition[4*i + 2] << " ";
                vOutputFile << vCurrentCondition[4*i + 3] << " " << endl;
            }
            
            //calculate next time step conditions until reached time vT and output system conditions to file

            while (vCurrentTime < vT) {
    
                vSystem.stepRK4(vStep);
                vCurrentTime = vSystem.getTime();
                vSystem.getSystemCondition(vCurrentCondition);
        
                for (unsigned int i = 0 ; i < vTotalBodies ;  ++i) {
                    //start from 1 rather than 0 so more natural for user
                    vOutputFile << i + 1 << " ";
                    vOutputFile << vCurrentTime << " ";
                    vOutputFile << vCurrentCondition[4*i] << " ";
                    vOutputFile << vCurrentCondition[4*i + 1] << " ";
                    vOutputFile << vCurrentCondition[4*i + 2] << " ";
                    vOutputFile << vCurrentCondition[4*i + 3] << " " << endl;
                }
            }
                
            vOutputFile.close();

            cout << "System has been evaluated. Data has been written into file 'outputs.txt'" << endl;
            
        }
        
        else {
            throw logic_error("'output.txt' file failed to open");
        }
        
    }
    
    catch (const logic_error& e) {
        cout << "An error has occured: " << e.what() << endl;
        return 0;
    }
        
    cout << "Program has completed successfully" << endl;
    return 1;
}