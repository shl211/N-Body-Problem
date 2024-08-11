#ifndef CLASS_ORBITSYSTEM
#define CLASS_ORBITSYSTEM
#include "Body.h"
#include <vector>
#include <cmath>
#include <algorithm>

//class containing information on the entire system of bodies and utilises class Body
class OrbitSystem {

    public:
        OrbitSystem();//constructor
        ~OrbitSystem();//destructor

        //member functions
        void updateG(const double& pG);                                //change value of G
        double getTime();                                              //get current time of system
        void addBody(Body& pNewBody);                                  //add object Body to system
        void getSystemCondition(std::vector<double>& pOutput);         //gets current condition of system in form (X1,Y1,XDot1,YDot1,X2,Y2,XDot2,YDot2...) 
        void getEquation(std::vector<double>& pCondition, double& pTime, std::vector<double>& pOutput); //evauate and get equation describing system of bodies (for use in RK4Step)
        void stepRK4(const double& pStepSize);                         //evaluate next step of system with 4th Order Runge-Kutta

    private:
        //define member variables
        unsigned int mTotalBodies;                                      //total bodies in system
        double mG;                                                      //Gravitational constant
        double mTimeCurrent;                                            //current time
        std::vector<Body> mSystem;                                      //empty vector of Body object

        //member function 
        void updateSystemCondition(std::vector<double>& pNewCondition); //update system conditions
};

OrbitSystem::OrbitSystem() {
    //initialise data

    mTotalBodies = 0;
    mG = 6.67e-11;
    mTimeCurrent = 0;
}

OrbitSystem::~OrbitSystem() {
    //nothing to do for this class
}

void OrbitSystem::updateG(const double& pG) {
    //assign a new value of G to system
    mG = pG;
}

double OrbitSystem::getTime() {
    return mTimeCurrent;
}

void OrbitSystem::addBody(Body& pNewBody) { 
    mSystem.push_back(pNewBody);               //add body by copying to vector
    mTotalBodies ++;                           //increment body number by 1
}

void OrbitSystem::getSystemCondition(std::vector<double>& pOutput) {
    //takes a vector pOutput and outputs system condition (X1,Y1,XDot1,YDot1,X2,Y2,XDot2,YDot2...) into vector pOutput
    
    //check if any data is present to be called; if not throw exception
    if (mTotalBodies == 0) {
        throw std::logic_error("No bodies currently in system. Please add bodies to the system");
    }

    //resize output vector if necessary
    if (pOutput.size() != mTotalBodies * 4) {
        pOutput.resize(mTotalBodies * 4);
    }

    //assign data
    for (unsigned int i = 0; i < mTotalBodies; ++i) {
        //get relevant data and assign to output vector
        pOutput[4*i] = mSystem[i].getX();
        pOutput[4*i + 1] = mSystem[i].getY();
        pOutput[4*i + 2] = mSystem[i].getXDot();
        pOutput[4*i + 3] = mSystem[i].getYDot();
    }
}


//function that can evaluate system body
void OrbitSystem::getEquation(std::vector<double>& pCondition, double& pTime, std::vector<double>& pOutput) {
    //input vector pCondition in form (X1,Y1,XDot1,YDot,X2,Y2,XDot2,YDot2...)
    //input pTime time of system
    //output vector pOutput passed by reference. System equation, evaluated for given input conditions pCondition, outputted in vector pOutput
    //evaluated equation of form f(y,t) = [X',Y',X'',Y''...]

    //check if any data is present to be called; if not throw exception
    if (mTotalBodies == 0) {
        throw std::logic_error("No bodies currently in system. Please add bodies to the system");
    }

    //resize equation output vector if necessary
    if(pOutput.size() != 4 * mTotalBodies) {
        pOutput.resize(4 * mTotalBodies);
    }

    //evaluate system of equations
    for (unsigned int i = 0; i < mTotalBodies; ++i) {
        //i denotes the current ith body
        
        //output values X Y for ith body in correct location
        //i.e. X' = XDot and Y' = YDot for current body
        pOutput[4*i] = pCondition[4*i + 2];
        pOutput[4*i + 1] = pCondition[4*i + 3];
        
        //define variables
        const double pXCurrent = pCondition[4*i];
        const double pYCurrent = pCondition[4*i + 1];
        
        double pXDotDot = 0;
        double pYDotDot = 0;

        double pDistance;
        double pXOther;
        double pYOther;
        double pMassOther;

        //sum acceleration contribution from all other jth bodies for X'' and Y''
        for (unsigned j = 0; j < mTotalBodies; ++j) {
            //j denotes the other jth body
            if (i != j) {
                //evaluate and sum contibution from all bodies i != k
                pXOther = pCondition[4*j];
                pYOther = pCondition[4*j+1];
                pMassOther = mSystem[j].getMass();
                    
                pDistance = sqrt( (pXOther - pXCurrent) * (pXOther - pXCurrent) + (pYOther - pYCurrent) * (pYOther - pYCurrent));

                //prevent division by 0
                if (pDistance == 0) {
                    throw std::logic_error("Division by 0. Two bodies at same point. Cannot evaluate system.");
                }

                pXDotDot += pMassOther * (pXOther - pXCurrent) / (pDistance * pDistance * pDistance);
                pYDotDot += pMassOther * (pYOther - pYCurrent) / (pDistance * pDistance * pDistance);
            }
        }
        
        pOutput[4*i + 2] = mG * pXDotDot;
        pOutput[4*i + 3] = mG * pYDotDot;
    }    
}

void OrbitSystem::stepRK4(const double& pStepSize) {
    //input pStepSize is a step size for evaluating 4th order Runge Kutta
    //system conditions automatically updated after each step

    //define local variable for current condition
    std::vector<double> pCurrentCondition;
    
    //get current system conditions and output to pCurrentCondition
    getSystemCondition(pCurrentCondition);

    //get size of each vector
    double pSize = pCurrentCondition.size();

    //define evaluated equation vector
    std::vector<double> pEqEval(pSize);
    std::vector<double> pNewCondition(pSize);

    //define vectors for interpolated points
    std::vector<double> pK1(pSize);
    std::vector<double> pK2(pSize);
    std::vector<double> pK3(pSize);
    std::vector<double> pK4(pSize);
    
    //define variable for interpolated time
    double pTimeNext;

    //define temporary variables
    std::vector<double> pTemp(pSize);
    std::vector<double> pRKTemp1(pSize);
    std::vector<double> pRKTemp2(pSize);

    //compute f(y,t) and store in pEqEval
    getEquation(pCurrentCondition, mTimeCurrent, pEqEval);

    //find k1
    transform(pEqEval.begin(), pEqEval.end(), pK1.begin(), [pStepSize] (double i) {return pStepSize * i;});

    //find k2
    transform(pCurrentCondition.begin(),pCurrentCondition.end(),pK1.begin(),pTemp.begin(), [] (double i, double j) {return i + 0.5*j;});
    pTimeNext = mTimeCurrent + pStepSize / 2;
    getEquation(pTemp, pTimeNext, pEqEval);//compute f(y+k1/2,t+h/2)
    transform(pEqEval.begin(), pEqEval.end(), pK2.begin(), [pStepSize] (double i) {return pStepSize * i;});
    
    //find k3
    transform(pCurrentCondition.begin(),pCurrentCondition.end(),pK2.begin(),pTemp.begin(), [] (double i, double j) {return i + 0.5*j;});
    getEquation(pTemp, pTimeNext, pEqEval);//compute f(y+k2/2,t+h/2)
    transform(pEqEval.begin(), pEqEval.end(), pK3.begin(), [pStepSize] (double i) {return pStepSize * i;});

    //find k4
    transform(pCurrentCondition.begin(),pCurrentCondition.end(),pK3.begin(),pTemp.begin(), [] (double i, double j) {return i + j;});
    pTimeNext = mTimeCurrent + pStepSize;
    getEquation(pTemp, pTimeNext, pEqEval);//compute f(y+k3,t+h)
    transform(pEqEval.begin(), pEqEval.end(), pK4.begin(), [pStepSize] (double i) {return pStepSize * i;}); 
 
    //compute conditions at next step
    transform(pK1.begin(), pK1.end(), pK2.begin(), pRKTemp1.begin(), [] (double k1, double k2){return k1/6 + k2/3;});
    transform(pK3.begin(), pK3.end(), pK4.begin(), pRKTemp2.begin(), [] (double k3, double k4){return k3/3 + k4/6;});
    transform(pRKTemp1.begin(), pRKTemp1.end(), pRKTemp2.begin(), pTemp.begin(), [] (double i, double j) {return i + j;});
    transform(pCurrentCondition.begin(), pCurrentCondition.end(), pTemp.begin(), pNewCondition.begin(), [] (double i, double j) {return i + j;});
    
    //System conditions are updated with new values
    updateSystemCondition(pNewCondition);

    //update time of system
    mTimeCurrent += pStepSize;
}

void OrbitSystem::updateSystemCondition(std::vector<double>& pNewCondition) {
    //input vector pNewCondition containing new conditions to update system with

    //check input vector of correct size and throw exception if incorrect size
    if (pNewCondition.size() != mTotalBodies * 4) {
        throw std::logic_error("Input vector of incorrect size. Input vector must be of size 4 times total number of bodies");
    }

    //update system conditions by directly updating conditions within body objects
    for (unsigned int i = 0; i < mTotalBodies; ++i) {
        mSystem[i].updateX(pNewCondition[4*i]);
        mSystem[i].updateY(pNewCondition[4*i + 1]);
        mSystem[i].updateXDot(pNewCondition[4*i + 2]);
        mSystem[i].updateYDot(pNewCondition[4*i + 3]);
    }
}


#endif

