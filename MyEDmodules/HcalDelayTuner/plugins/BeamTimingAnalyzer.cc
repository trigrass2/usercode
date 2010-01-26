// -*- C++ -*-
//
// Package:    BeamTimingAnalyzer
// Class:      BeamTimingAnalyzer
// 
/**\class BeamTimingAnalyzer BeamTimingAnalyzer.cc MyEDmodules/HcalDelayTuner/plugins/BeamTimingAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: BeamTimingAnalyzer.cc,v 1.2 2009/12/04 14:39:04 dudero Exp $
//
//


// system include files

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MyEDmodules/MyAnalUtilities/interface/myEventData.hh"
#include "MyEDmodules/MyAnalUtilities/interface/inSet.hh"

#include "MyEDmodules/HcalDelayTuner/interface/HcalDelayTunerAlgos.hh"
#include "MyEDmodules/HcalDelayTuner/interface/BeamDelayTunerAlgos.hh"
#include "MyEDmodules/HcalDelayTuner/interface/BeamHitTimeCorrector.hh"

//
// class declaration
//

class BeamTimingAnalyzer : public edm::EDAnalyzer {
public:
  explicit BeamTimingAnalyzer(const edm::ParameterSet&);
  ~BeamTimingAnalyzer();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------

  myEventData           *eventData_;
  BeamDelayTunerAlgos   *algo_;
  BeamHitTimeCorrector  *timecor_;
  std::set<uint32_t>     s_runs_; // set of run numbers run over
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
//======================================================================

//
// constructors and destructor
//
BeamTimingAnalyzer::BeamTimingAnalyzer(const edm::ParameterSet& iConfig)
{
  std::cerr << "-=-=BeamTimingAnalyzer Constructor=-=-" << std::endl;

  edm::ParameterSet edPset =
    iConfig.getUntrackedParameter<edm::ParameterSet>("eventDataPset");

  eventData_ = new myEventData(edPset);
  timecor_   = new BeamHitTimeCorrector();

  algo_ = new BeamDelayTunerAlgos(iConfig,timecor_);
}

BeamTimingAnalyzer::~BeamTimingAnalyzer() {
  std::cerr << "-=-=BeamTimingAnalyzer Destructor=-=-" << std::endl;
}

//======================================================================

//
// member functions
//
//======================================================================

// ------------ method called to for each event  ------------
void
BeamTimingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  eventData_->get(iEvent,iSetup);

  uint32_t runn = eventData_->runNumber();
  if (notInSet<uint32_t>(s_runs_,runn))
    s_runs_.insert(runn);

  algo_->process(*eventData_);
}

// ------------ method called once each job just before starting event loop  ------------
void 
BeamTimingAnalyzer::beginJob(const edm::EventSetup& es)
{
  timecor_->init(es);
  algo_->beginJob(es);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BeamTimingAnalyzer::endJob() {
  algo_->endJob();
}

//define this as a plug-in
DEFINE_FWK_MODULE(BeamTimingAnalyzer);
