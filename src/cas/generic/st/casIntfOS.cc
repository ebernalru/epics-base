
/*
 *
 * casIntfOS.cc
 * $Id$
 *
 *
 */


//
// CA server
// 
#include "server.h"

//
// casServerReg
//
class casServerReg : public fdReg {
public:
        casServerReg (casIntfOS &osIn) :
                fdReg (osIn.getFD(), fdrRead), os (osIn) {}
        ~casServerReg ();
private:
        casIntfOS &os;
 
        void callBack ();
};


//
// casIntfOS::init()
//
caStatus casIntfOS::init(const caNetAddr &addrIn, casDGClient &dgClientIn,
		int autoBeaconAddr, int addConfigBeaconAddr)
{
	caStatus stat;

	stat = this->casIntfIO::init(addrIn, dgClientIn,
			autoBeaconAddr, addConfigBeaconAddr);
	if (stat) {
		return stat;
	}

	this->setNonBlocking();

	if (this->pRdReg==NULL) {
		this->pRdReg = new casServerReg(*this);
		if (this->pRdReg==NULL) {
			return S_cas_noMemory;
		}
	}
	
	return S_cas_success;
}


//
// casIntfOS::~casIntfOS()
//
casIntfOS::~casIntfOS()
{
	if (this->pRdReg) {
		delete this->pRdReg;
	}
}

//
// casIntfOS::newDGIntfIO()
//
casDGIntfIO *casIntfOS::newDGIntfIO(casDGClient &dgClientIn) const
{
	return new casDGIntfOS(dgClientIn);
}


//
// casServerReg::callBack()
//
void casServerReg::callBack()
{
	assert(this->os.pRdReg);
	this->os.cas.connectCB(this->os);	
}

//
// casServerReg::~casServerReg()
//
casServerReg::~casServerReg()
{
}

