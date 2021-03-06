#ifndef  _MYEDMODULESSPLASHTIMINGANALALGOS
#define  _MYEDMODULESSPLASHTIMINGANALALGOS

// -*- C++ -*-
//
// Package:    SplashTimingAnalAlgos
// Class:      SplashTimingAnalAlgos
// 
/**\class SplashTimingAnalAlgos SplashTimingAnalAlgos.cc MyEDmodules/SplashTimingAnalAlgos/src/SplashTimingAnalAlgos.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: SplashTimingAnalAlgos.hh,v 1.12 2010/04/22 03:27:40 dudero Exp $
//
//


// system include files

// user include files
#include "HcalTimingAnalAlgos.hh"
#include "SplashHitTimeCorrector.hh"
#include "TTree.h"
#include "TH1F.h"
#include "TProfile2D.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"

//
// class declaration
//

class SplashTimingAnalAlgos : public HcalTimingAnalAlgos {
public:
  SplashTimingAnalAlgos(const edm::ParameterSet&,
			SplashHitTimeCorrector *);
  ~SplashTimingAnalAlgos() {}

  void process      (const myEventData&);
  void beginJob     (const edm::EventSetup&,const myEventData&);

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

  void bookDetailHistos4cut(myAnalCut& cut);
  void fillHistos4cut(myAnalCut& cut);

  // ----------member data ---------------------------

  std::vector<std::string> v_cuts_;             // vector of cut strings

  int unravelHBatIeta_;

  SplashHitTimeCorrector *timecor_; // splash hit time geometry corrector

  edm::ESHandle<HcalDbService>  conditions_;

  // The collection of names of histos per subdetector
  std::string st_unravelTimePerPhid1_, st_unravelTimePerPhid2_;
  std::string st_unravelTimePerRMd1_,  st_unravelTimePerRMd2_;

  std::map<int,std::string>        m_unravelHBperRM_;
  std::map<int,std::string>        m_unravelHBperPhi_;

};

#endif // _MYEDMODULESSPLASHTIMINGANALALGOS
