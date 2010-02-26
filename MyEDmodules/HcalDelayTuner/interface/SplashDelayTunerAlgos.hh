#ifndef  _MYEDMODULESSPLASHDELAYTUNERALGOS
#define  _MYEDMODULESSPLASHDELAYTUNERALGOS

// -*- C++ -*-
//
// Package:    SplashDelayTunerAlgos
// Class:      SplashDelayTunerAlgos
// 
/**\class SplashDelayTunerAlgos SplashDelayTunerAlgos.cc MyEDmodules/SplashDelayTunerAlgos/src/SplashDelayTunerAlgos.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: SplashDelayTunerAlgos.hh,v 1.8 2009/12/04 14:36:00 dudero Exp $
//
//


// system include files

// user include files
#include "HcalDelayTunerAlgos.hh"
#include "SplashHitTimeCorrector.hh"
#include "TTree.h"
#include "TH1F.h"
#include "TProfile2D.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "MyEDmodules/HcalDelayTuner/src/HcalDelayTunerInput.hh"

//
// class declaration
//

class SplashDelayTunerAlgos : public HcalDelayTunerAlgos {
public:
  SplashDelayTunerAlgos(const edm::ParameterSet&,
			SplashHitTimeCorrector *);
  ~SplashDelayTunerAlgos() {}

  void process      (const myEventData&);

private:

  // ---------- private methods ---------------------------

  template<class Digi>
  void   fillDigiPulse          (TH1F *pulseHist,
				 const Digi& frame);
#if 0
  template<class Digi,class RecHit>
  void   processDigisAndRecHits (const edm::SortedCollection<Digi>&   digis,
				 const edm::SortedCollection<RecHit>& rechits);
#endif
  template<class Digi,class RecHit>
  void   processDigisAndRecHits (const edm::Handle<edm::SortedCollection<Digi> >& digihandle,
				 const edm::Handle<edm::SortedCollection<RecHit> >& rechithandle);

  void bookHistos4lastCut(void);
  void fillHistos4cut(const std::string& cutstr);

  // ----------member data ---------------------------

  int unravelHBatIeta_;

  SplashHitTimeCorrector *timecor_; // splash hit time geometry corrector

  edm::ESHandle<HcalDbService>  conditions_;

  // The collection of names of histos per subdetector

  std::map<int,std::string>        m_unravelHBperRM_;
  std::map<int,std::string>        m_unravelHBperPhi_;

};

#endif // _MYEDMODULESSPLASHDELAYTUNERALGOS
