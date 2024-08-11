#ifndef CLASS_BODY
#define CLASS_BODY

//class containing information on body's position (X,Y), velocity (XDot,YDot) and mass
class Body {
    
    public:
        Body(const double& pX, const double& pY, const double& pXDot, const double& pYDot, const double& pMass);//constructor to assign data to body
        ~Body(); //destructor    

        //member functions to get data
        double getX();
        double getY();
        double getXDot();
        double getYDot();
        double getMass();
        
        //member functions to update body data
        void updateX(const double& pVal);
        void updateY(const double& pVal);
        void updateXDot(const double& pVal);
        void updateYDot(const double& pVal);
        void updateMass(const double& pVal);

    private:
        //define class member variables
        double mXCurrent;
        double mYCurrent;
        double mXDotCurrent;
        double mYDotCurrent;
        double mMass;
};

Body::Body(const double& pX, const double& pY, const double& pXDot, const double& pYDot, const double& pMass){
    //initialise variables describing body's position,velocity and mass

    mXCurrent = pX;
    mYCurrent = pY;
    mXDotCurrent = pXDot;
    mYDotCurrent = pYDot;
    //check mass value is valid
    if (pMass > 0) {
        mMass = pMass;
    }
    else {
        throw std::logic_error("Invalid mass. Mass cannot be zero or negative");
    }
}

Body::~Body() {
    //nothing to do for this 
}

//functions to return specific value
double Body::getX() {
    return mXCurrent;
}

double Body::getY() {
    return mYCurrent;
}

double Body::getXDot() {
    return mXDotCurrent;
}

double Body::getYDot() {
    return mYDotCurrent;
}

double Body::getMass() {
    return mMass;
}

//functions to update specific value
void Body::updateX(const double& pVal) {
    //takes input value pVal and updates member variable X
    mXCurrent = pVal;
}

void Body::updateY(const double& pVal) {
    //takes input value pVal and updates member variable Y
    mYCurrent = pVal;
}

void Body::updateXDot(const double& pVal) {
    //takes input value pVal and updates member variable XDot
    mXDotCurrent = pVal;
}

void Body::updateYDot(const double& pVal) {
    //takes input value pVal and updates member variable YDot
    mYDotCurrent = pVal;
}
        
void Body::updateMass(const double& pVal) {
    //takes input value pVal and updates member variable mass
    
    if (pVal > 0) {
        mMass = pVal;
    }
    else {
        throw std::logic_error("Invalid mass. Mass cannot be zero or negative"); 
    }
}

#endif