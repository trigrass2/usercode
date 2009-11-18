#ifndef  _MYEDMODULESHCALDELAYTUNERINPUT
#define  _MYEDMODULESHCALDELAYTUNERINPUT

// -*- C++ -*-
//
// Package:    HcalDelayTunerInput
// Class:      HcalDelayTunerInput
// 
/**\class HcalDelayTunerInput HcalDelayTunerInput.cc MyEDmodules/HcalDelayTunerInput/src/HcalDelayTunerInput.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: HcalDelayTunerInput.hh,v 1.5 2009/05/21 09:52:41 dudero Exp $
//
//


// system include files
#include <set>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDetId/interface/HcalFrontEndId.h"
#include "MyEDmodules/HcalDelayTuner/interface/HcalDelayTunerAlgos.hh"
//
// class declaration
//

class HcalDelayTunerInput {
public:

  struct TTCrxSettings {
    int fine;
    int coarse1;
    int coarse2;
  };

  // ParameterSet only if you identify a parameter you think
  // should be configurable from the py file.
  //
  explicit HcalDelayTunerInput(const edm::ParameterSet&);
  ~HcalDelayTunerInput() {}

  // Parses a list of xml files and returns a map of delay settings,
  // one for each channel
  //
  void getSamplingDelays   (DelaySettings& delays);

  // Parses a single xml file that has one phase setting per RBX
  // and returns a map of these settings, one per RBX name.
  //
  void getTTCrxPhases      (const std::string& xmlFileNames,
			    std::map<std::string,TTCrxSettings>& phases);

private:

  // Add private methods and data members as needed
  std::vector<std::string> xmlfileNames_;
					
};

#endif // _MYEDMODULESHCALDELAYTUNERINPUT