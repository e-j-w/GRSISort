#include "TTransition.h"

#include <iostream>

/// \cond CLASSIMP
ClassImp(TTransition)
/// \endcond

TTransition::TTransition()
{
	Clear();
}

TTransition::~TTransition()
{
	// empty
}

void TTransition::Clear(Option_t*)
{
	fEnergy         = 0;
	fEngUncertainty = 0;
	fIntensity      = 0;
	fIntUncertainty = 0;
}

void TTransition::Print(Option_t*) const
{

	if(!std::isnan(fEngUncertainty)) {
		std::cout<<"Energy:    "<<fEnergy<<" +/- "<<fEngUncertainty<<std::endl;
	} else {
		std::cout<<"Energy:    "<<fEnergy<<std::endl;
	}
	if(!std::isnan(fIntensity)) {
		if(!std::isnan(fIntUncertainty)) {
			std::cout<<"\tIntensity: "<<fIntensity<<" +/- "<<fIntUncertainty<<std::endl;
		} else {
			std::cout<<"\tIntensity: "<<fEnergy<<std::endl;
		}
	} else {
		std::cout<<std::endl;
	}
}

std::string TTransition::PrintToString()
{
	std::string toString;
	toString.append(Form("%f\t", fEnergy));
	toString.append(Form("%f\t", fEngUncertainty));
	toString.append(Form("%f\t", fIntensity));
	toString.append(Form("%f\t", fIntUncertainty));

	return toString;
}

int TTransition::CompareIntensity(const TObject* obj) const
{
	if(fIntensity > static_cast<const TTransition*>(obj)->fIntensity) {
		return -1;
	}
	if(fIntensity == static_cast<const TTransition*>(obj)->fIntensity) {
		return 0;
	}
	return 1;

	return -9;
}

int TTransition::Compare(const TObject* obj) const
{

	return CompareIntensity(obj);

	// Compares the intensities of the TTransitions and returns
	if(fEnergy > static_cast<const TTransition*>(obj)->fEnergy) {
		return -1;
	}
	if(fEnergy == static_cast<const TTransition*>(obj)->fEnergy) {
		return 0;
	}
	return 1;

	return -9;
}
