/**\class HcalTimingAnalAlgos HcalTimingAnalAlgos.cc MyEDmodules/HcalTimingAnalAlgos/src/HcalTimingAnalAlgos.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: HcalTimingAnalAlgos.cc,v 1.9 2009/08/27 01:56:37 dudero Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <set>
#include <stdlib.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "MyEDmodules/HcalTimingAnalyzer/src/HcalTimingAnalAlgos.hh"
#include "MyEDmodules/MyAnalUtilities/interface/myAnalHistos.hh"
#include "MyEDmodules/MyAnalUtilities/interface/inSet.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TProfile2D.h"

//
// constants, enums and typedefs
//
class IetaIphi_t {
public:
  inline int sign(int x) const { return ((x<0) ? -1 : 1); }
  IetaIphi_t (int inieta, int iniphi) : ieta_(inieta), iphi_(iniphi) {}
  IetaIphi_t (int code) { ieta_ = code/100; iphi_ = abs(code)%100; }
  int toCode (void) const { return (ieta_*100)+(sign(ieta_)*iphi_); }
  int ieta   (void) const { return ieta_; }
  int iphi  (void) const { return iphi_; }
  bool operator<(const IetaIphi_t& right) const {
    if (ieta_ < right.ieta_) return true;
    else if (iphi_ < right.iphi_) return true;
    return false;
  }
private:
  int ieta_;
  int iphi_;
};

struct HFtowerEnergies_t {
  HFtowerEnergies_t () : ieip(0,0), totalE(0.0), longE(0.0), shortE(0.0) {}
  IetaIphi_t ieip;
  double totalE;
  double longE;
  double shortE;
};

//
// static data member definitions
//
//======================================================================

//
// constructors and destructor
//
HcalTimingAnalAlgos::HcalTimingAnalAlgos(const edm::ParameterSet& iConfig) :
  minHitGeVHB_(iConfig.getParameter<double>("minHitGeVHB")),
  minHitGeVHE_(iConfig.getParameter<double>("minHitGeVHE")),
  minHitGeVHO_(iConfig.getParameter<double>("minHitGeVHO")),
  minHitGeVHF1_(iConfig.getParameter<double>("minHitGeVHF1")),
  minHitGeVHF2_(iConfig.getParameter<double>("minHitGeVHF2")),
  ecalRecHitTscaleNbins_(iConfig.getParameter<int>("ecalRecHitTscaleNbins")),
  ecalRecHitTscaleMinNs_(iConfig.getParameter<double>("ecalRecHitTscaleMinNs")),
  ecalRecHitTscaleMaxNs_(iConfig.getParameter<double>("ecalRecHitTscaleMaxNs")),
  hcalRecHitTscaleNbins_(iConfig.getParameter<int>("hcalRecHitTscaleNbins")),
  hcalRecHitTscaleMinNs_(iConfig.getParameter<double>("hcalRecHitTscaleMinNs")),
  hcalRecHitTscaleMaxNs_(iConfig.getParameter<double>("hcalRecHitTscaleMaxNs")),
  hcalRecHitEscaleMinGeV_(iConfig.getParameter<double>("hcalRecHitEscaleMinGeV")),
  hcalRecHitEscaleMaxGeV_(iConfig.getParameter<double>("hcalRecHitEscaleMaxGeV")),
  rundescr_(iConfig.getUntrackedParameter<std::string>("runDescription",""))
{
 //now do what ever initialization is needed
  std::vector<int> v_events2anal(iConfig.getParameter<std::vector<int> >("eventNumbers"));

  for (unsigned i=0; i<v_events2anal.size(); i++) {
    s_events2anal_.insert(v_events2anal[i]);
  }

  cutNone_       = new myAnalCut(0,"cutNone");
  cutMaxHitOnly_ = new myAnalCut(1,"cutMaxHitOnly");
  cutTgtTwrOnly_ = new myAnalCut(2,"cutTgtTwrOnly");
  cutMinHitGeV_  = new myAnalCut(3,"cutMinHitGeV");
  cutAll_        = new myAnalCut(4,"cutAll");

  std::vector<int> v_tgtid = iConfig.getParameter<std::vector<int> > ("tgtTwrId");
  if ((v_tgtid.size() > 0) &&
      !convertIdNumbers(v_tgtid, tgtTwrId_))
    throw cms::Exception("Invalid detID vector");

  edm::ParameterSet edPset =
    iConfig.getUntrackedParameter<edm::ParameterSet>("eventDataPset");

  edm::InputTag hbheDigiTag =
    edPset.getUntrackedParameter<edm::InputTag>("hbheDigiLabel",edm::InputTag(""));

  doHBHEdigis_ = hbheDigiTag.label().size() > 0;

  edm::InputTag simHitTag =
    edPset.getUntrackedParameter<edm::InputTag>("simHitLabel",edm::InputTag(""));

  doSimHits_ = simHitTag.label().size() > 0;

  if (doSimHits_) {
    simHitTscaleNbins_           = iConfig.getParameter<int>("simHitTscaleNbins");
    simHitTscaleMinNs_           = iConfig.getParameter<double>("simHitTscaleMinNs");
    simHitTscaleMaxNs_           = iConfig.getParameter<double>("simHitTscaleMaxNs");
    simHitEnergyMinGeVthreshold_ = iConfig.getParameter<double>("simHitEnergyMinGeVthreshold");
  }
}                            // HcalTimingAnalAlgos::HcalTimingAnalAlgos

//======================================================================

bool
HcalTimingAnalAlgos::convertIdNumbers(std::vector<int>& v_idnumbers,
				      HcalDetId& detId)
{
  // convert det ID numbers to valid detIds:
  if (v_idnumbers.size()!=3) {
    return false;
  }

  int ieta  = v_idnumbers[0];
  int iphi  = v_idnumbers[1];
  int depth = v_idnumbers[2];

  enum HcalSubdetector subdet;
  if      (depth == 4)       subdet = HcalOuter;
  else if (abs(ieta) <= 16)  subdet = HcalBarrel;
  else if (abs(ieta) <= 28)  subdet = HcalOuter; // gud enuf fer gubmint werk
  else if (abs(ieta) <= 41)  subdet = HcalForward;
  else return false;

  if (!HcalDetId::validDetId(subdet,ieta,iphi,depth))
    return false;

  detId = HcalDetId(subdet,ieta,iphi,depth);
  return true;
}                                   // HFtrigAnalAlgos::convertIdNumbers

//======================================================================

void HcalTimingAnalAlgos::bookPerRunHistos(const uint32_t rn)
{
  std::vector<myAnalHistos::HistoParams_t> v_hpars1d; 
  std::vector<myAnalHistos::HistoParams_t> v_hpars2d; 
  std::vector<myAnalHistos::HistoParams_t> v_hpars2dprof;

  myAnalHistos::HistoParams_t hpars1d;
  myAnalHistos::HistoParams_t hpars2d;

  stringstream o; o<<rn;

  std::string runstrn = (rn>10000) ? o.str() : rundescr_;
  std::string runstrt =
    (rn>10000) ? "[RUN:" + o.str() + "]" : "[" + rundescr_ + "]";

  /*****************************************
   * 1-D HISTOGRAMS FIRST:                 *
   *****************************************/
  hpars1d.nbinsy = 0;

  if (doHBHEdigis_) {
    st_avgPulse_   = "h1d_pulse" + runstrn;
    hpars1d.name   = st_avgPulse_;
    hpars1d.title  = "HBHE Average Pulse Shape " + runstrt;
    hpars1d.nbinsx = 10;
    hpars1d.minx   = -0.5;
    hpars1d.maxx   =  9.5;

    v_hpars1d.push_back(hpars1d);

    st_avgPulseTerr_  = "h1d_pulseTerr" + runstrn;
    hpars1d.name   = st_avgPulseTerr_;
    hpars1d.title  = "HBHE Avg Pulse Shape (Pulse Shape Error) " + runstrt;
    hpars1d.nbinsx = 10;
    hpars1d.minx   = -0.5;
    hpars1d.maxx   =  9.5;

    v_hpars1d.push_back(hpars1d);

    st_avgPulseHPDmult_   = "h1d_pulseHPDmult" + runstrn;
    hpars1d.name   = st_avgPulseHPDmult_;
    hpars1d.title  = "HBHE Avg Pulse Shape (HPD noise) " + runstrt;
    hpars1d.nbinsx = 10;
    hpars1d.minx   = -0.5;
    hpars1d.maxx   =  9.5;

    v_hpars1d.push_back(hpars1d);

    st_hbhedigiColSize_ = "h1d_HBHEdigiCollectionSize" + runstrn;
    hpars1d.name   = st_hbhedigiColSize_;
    hpars1d.title  = "HBHE Digi Collection Size " + runstrt;
    hpars1d.nbinsx = 5201; // 72chan/RBX*72RBX = 5184
    hpars1d.minx   = -0.5;
    hpars1d.maxx   = 5200.5;

    v_hpars1d.push_back(hpars1d);
  }

  st_hbheRHColSize_ = "h1d_HBHERecHitCollectionSize" + runstrn;
  hpars1d.name   = st_hbheRHColSize_;
  hpars1d.title  = "HBHE RecHit Collection Size " + runstrt;
  hpars1d.nbinsx = 5201; // 72chan/RBX*72RBX = 5184
  hpars1d.minx   = -0.5;
  hpars1d.maxx   = 5200.5;
  
  v_hpars1d.push_back(hpars1d);

  st_hbheRHflags_ = "h1d_HBHERecHitFlags" + runstrn;
  hpars1d.name   = st_hbheRHflags_;
  hpars1d.title  = "HBHE RecHit Flags " + runstrt;
  hpars1d.nbinsx = 32;
  hpars1d.minx   = -0.5;
  hpars1d.maxx   = 31.5;
  
  v_hpars1d.push_back(hpars1d);

  st_rhTimes_ = "h1d_RHTimes" + runstrn;
  hpars1d.name   = st_rhTimes_;
  hpars1d.title  = "HBHE RecHit Times " + runstrt + "; Rechit Time (ns)";
  hpars1d.nbinsx = hcalRecHitTscaleNbins_;
  hpars1d.minx   = hcalRecHitTscaleMinNs_;
  hpars1d.maxx   = hcalRecHitTscaleMaxNs_;

  v_hpars1d.push_back(hpars1d);

  st_rhEnergies_ = "h1d_RHEnergies" + runstrn;
  hpars1d.name   = st_rhEnergies_;
  hpars1d.title  = "HBHE RecHit Energies " + runstrt + "; Rechit Energy (GeV)";
  hpars1d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars1d.minx   = hcalRecHitEscaleMinGeV_;
  hpars1d.maxx   = hcalRecHitEscaleMaxGeV_;

  v_hpars1d.push_back(hpars1d);

  if (doSimHits_) {
    st_shTimes_ = "h1d_SHTimes" + runstrn;
    hpars1d.name   = st_shTimes_;
    hpars1d.title  = "Hcal SimHit Times " + runstrt + "; Simhit Time (ns)";
    hpars1d.nbinsx = simHitTscaleNbins_;
    hpars1d.minx   = simHitTscaleMinNs_;
    hpars1d.maxx   = simHitTscaleMaxNs_;
    
    v_hpars1d.push_back(hpars1d);

    st_shEnergies_ = "h1d_SHEnergies" + runstrn;
    hpars1d.name   = st_shEnergies_;
    hpars1d.title  = "HBHE SimHit Energies " + runstrt + "; Simhit Energy * 200)";
    hpars1d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
    hpars1d.minx   = hcalRecHitEscaleMinGeV_;
    hpars1d.maxx   = hcalRecHitEscaleMaxGeV_;

    v_hpars1d.push_back(hpars1d);
  }

  st_caloMet_Met_ = "h1d_caloMet_Met" + runstrn;
  hpars1d.name   = st_caloMet_Met_;
  hpars1d.title  = "MET from CaloTowers " + runstrt + "; MET (GeV)";
  hpars1d.nbinsx = 200;    // 100;
  hpars1d.minx   = 0.;
  hpars1d.maxx   = 1000.0; // hcalRecHitEscaleMaxGeV_;

  v_hpars1d.push_back(hpars1d);

  st_caloMet_Phi_ = "h1d_caloMet_Phi" + runstrn;
  hpars1d.name   = st_caloMet_Phi_;
  hpars1d.title  = "MET #phi from CaloTowers " + runstrt + "; MET phi (rad)";
  hpars1d.nbinsx = 100;
  hpars1d.minx   =  -4.;
  hpars1d.maxx   =   4.;

  v_hpars1d.push_back(hpars1d);

  st_caloMet_SumEt_ = "h1d_caloMet_SumEt" + runstrn;
  hpars1d.name   = st_caloMet_SumEt_;
  hpars1d.title  = "SumET from CaloTowers " + runstrt + "; SumET (GeV)";
  hpars1d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars1d.minx   = hcalRecHitEscaleMinGeV_;
  hpars1d.maxx   = hcalRecHitEscaleMaxGeV_;

  v_hpars1d.push_back(hpars1d);

  /*****************************************
   * 2-D HISTOGRAMS AFTER:                 *
   *****************************************/

  st_rhEmap_     = "h2d_rhEperIetaIphi" + runstrn;
  hpars2d.name   = st_rhEmap_;
  hpars2d.title  = "HBEF RecHit Energy Map (#Sigma depths)" + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2d.push_back(hpars2d);

  st_HFratioVsE_ = "h2d_HFratioVsE" + runstrn;
  hpars2d.name   = st_HFratioVsE_;
  hpars2d.title  = "HF (L-S)/(L+S) Ratio VS Energy" + runstrt + "; L+S (GeV); (L-S)/(L+S)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = 50;
  hpars2d.miny   = -1.;
  hpars2d.maxy   =  1.;

  v_hpars2d.push_back(hpars2d);

  st_hbheTimingVsE_ = "h2d_hbheTimingVsE" + runstrn;
  hpars2d.name   = st_hbheTimingVsE_;
  hpars2d.title  = "HBHE RecHit Timing vs. Energy " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_hbheTvsEpulseErr_ = "h2d_hbheTvsEpulseErr" + runstrn;
  hpars2d.name   = st_hbheTvsEpulseErr_;
  hpars2d.title  = "HBHE RecHit Timing vs. Energy, pulse Shape Error bit set " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_hbheTvsEhpdMult_ = "h2d_hbheTvsEhpdMult" + runstrn;
  hpars2d.name   = st_hbheTvsEhpdMult_;
  hpars2d.title  = "HBHE RecHit Timing vs. Energy, HPD noise bit set " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_hfTimingVsE_ = "h2d_hfTimingVsE" + runstrn;
  hpars2d.name   = st_hfTimingVsE_;
  hpars2d.title  = "HF RecHit Timing vs. Energy " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_hfTvsEpulseErr_ = "h2d_hfTvsEpulseErr" + runstrn;
  hpars2d.name   = st_hfTvsEpulseErr_;
  hpars2d.title  = "HF RecHit Timing vs. Energy, pulse Shape Error bit set " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_hoTimingVsE_ = "h2d_hoTimingVsE" + runstrn;
  hpars2d.name   = st_hoTimingVsE_;
  hpars2d.title  = "HO RecHit Timing vs. Energy " + runstrt + "; Rechit Energy (GeV); Rechit Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  if (doSimHits_) {
    st_shTimingVsE_ = "h2d_shTimingVsE" + runstrn;
    hpars2d.name   = st_shTimingVsE_;
    hpars2d.title  = "Hcal SimHit Timing vs. Energy " + runstrt + "; Simhit Energy * 200; Simhit Time (ns)";
    hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_) + 1;
    hpars2d.minx   =  -0.5;
    hpars2d.maxx   = hcalRecHitEscaleMaxGeV_;
    hpars2d.nbinsy = simHitTscaleNbins_;
    hpars2d.miny   = simHitTscaleMinNs_;
    hpars2d.maxy   = simHitTscaleMaxNs_;

    v_hpars2d.push_back(hpars2d);
  }

  st_ctHcalTvstwrE_ = "h2d_ctHcalTvstwrE" + runstrn;
  hpars2d.name   = st_ctHcalTvstwrE_;
  hpars2d.title  = "Calo Tower HCAL Timing vs. Energy " + runstrt + "; Tower Energy (GeV); HCAL Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = 2*hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = 2*hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = 100;      // hcalRecHitTscaleNbins_;
  hpars2d.miny   = -35000.0; // hcalRecHitTscaleMinNs_;
  hpars2d.maxy   =  10000.0; // hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_ctHcalTvstwrEg_ = "h2d_ctHcalTvstwrEgood" + runstrn;
  hpars2d.name   = st_ctHcalTvstwrEg_;
  hpars2d.title  = "Calo Tower HCAL Timing vs. Energy " + runstrt + "; Tower Energy (GeV); HCAL Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = 2*hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = 2*hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_ctEcalTvstwrE_ = "h2d_ctEcalTvstwrE" + runstrn;
  hpars2d.name   = st_ctEcalTvstwrE_;
  hpars2d.title  = "Calo Tower ECAL Timing vs. Energy " + runstrt + "; Tower Energy (GeV); ECAL Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = 2*hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = 2*hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = 100;      // ecalRecHitTscaleNbins_;
  hpars2d.miny   = -35000.0; // ecalRecHitTscaleMinNs_;
  hpars2d.maxy   =  10000.0; // ecalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_ctEcalTvstwrEg_ = "h2d_ctEcalTvstwrEgood" + runstrn;
  hpars2d.name   = st_ctEcalTvstwrEg_;
  hpars2d.title  = "Calo Tower ECAL Timing vs. Energy " + runstrt + "; Tower Energy (GeV); ECAL Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = 2*hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = 2*hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = ecalRecHitTscaleNbins_;
  hpars2d.miny   = ecalRecHitTscaleMinNs_;
  hpars2d.maxy   = ecalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_ctEcalTvsHcalT_ = "h2d_ctEcalTvsHcalT" + runstrn;
  hpars2d.name   = st_ctEcalTvsHcalT_;
  hpars2d.title  = "Calo Tower ECAL Time vs. HCAL Time " + runstrt + "; HCAL Time (ns); ECAL Time (ns)";
  hpars2d.nbinsx = 100;      // hcalRecHitTscaleNbins_;
  hpars2d.minx   = -35000.0; // hcalRecHitTscaleMinNs_;
  hpars2d.maxx   =  10000.0; // hcalRecHitTscaleMaxNs_;
  hpars2d.nbinsy = 100;      // ecalRecHitTscaleNbins_;
  hpars2d.miny   = -35000.0; // ecalRecHitTscaleMinNs_;
  hpars2d.maxy   =  10000.0; // ecalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

  st_ctEcalTvsHcalTg_ = "h2d_ctEcalTvsHcalTgood" + runstrn;
  hpars2d.name   = st_ctEcalTvsHcalTg_;
  hpars2d.title  = "Calo Tower ECAL Time vs. HCAL Time " + runstrt + "; HCAL Time (ns); ECAL Time (ns)";
  hpars2d.nbinsx = hcalRecHitTscaleNbins_;
  hpars2d.minx   = hcalRecHitTscaleMinNs_;
  hpars2d.maxx   = hcalRecHitTscaleMaxNs_;
  hpars2d.nbinsy = ecalRecHitTscaleNbins_;
  hpars2d.miny   = ecalRecHitTscaleMinNs_;
  hpars2d.maxy   = ecalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);

#if 0
  st_ctWeightTvsE_ = "h2d_ctWeightTvsE" + runstrn;
  hpars2d.name   = st_ctWeightTvsE_;
  hpars2d.title  = "Calo Tower E-Weighted Time vs. Energy " + runstrt + "; Tower Energy (GeV); E-Weighted Time (ns)";
  hpars2d.nbinsx = (uint32_t)(hcalRecHitEscaleMaxGeV_ - hcalRecHitEscaleMinGeV_);
  hpars2d.minx   = 2*hcalRecHitEscaleMinGeV_;
  hpars2d.maxx   = 2*hcalRecHitEscaleMaxGeV_;
  hpars2d.nbinsy = hcalRecHitTscaleNbins_;
  hpars2d.miny   = hcalRecHitTscaleMinNs_;
  hpars2d.maxy   = hcalRecHitTscaleMaxNs_;

  v_hpars2d.push_back(hpars2d);
#endif

  /*****************************************
   * 2-D PROFILES:                         *
   *****************************************/

  st_rhTprofd1_  = "p2d_rhTperIetaIphiDepth1" + runstrn;
  hpars2d.name   = st_rhTprofd1_;
  hpars2d.title  = "HBEF (Depth 1) RecHit Time Map-Profile " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  st_rhTprofd1hpdMult_  = "p2d_rhTperIetaIphiDepth1hpdMult" + runstrn;
  hpars2d.name   = st_rhTprofd1hpdMult_;
  hpars2d.title  = "HBHE (Depth 1) RecHit Time Map-Profile for HPD Noise events " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  st_rhTprofd2_  = "p2d_rhTperIetaIphiDepth2" + runstrn;
  hpars2d.name   = st_rhTprofd2_;
  hpars2d.title  = "HBEF (Depth 2) RecHit Time Map-Profile " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  st_rhTprofd3_  = "p2d_rhTperIetaIphiDepth3" + runstrn;
  hpars2d.name   = st_rhTprofd3_;
  hpars2d.title  = "HBEF (Depth 3) RecHit Time Map-Profile " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  st_rhTprofd4_  = "p2d_rhTperIetaIphiDepth4" + runstrn;
  hpars2d.name   = st_rhTprofd4_;
  hpars2d.title  = "HO (Depth 4) RecHit Time Map-Profile " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  st_ctTprof_    = "p2d_ctTperIetaIphiProf" + runstrn;
  hpars2d.name   = st_ctTprof_;
  hpars2d.title  = "Calo Tower Weighted Time Map-Profile " + runstrt + "; ieta; iphi";
  hpars2d.nbinsx =  83;
  hpars2d.minx   =  -41.5;
  hpars2d.maxx   =   41.5;
  hpars2d.nbinsy =  72;
  hpars2d.miny   =    0.5;
  hpars2d.maxy   =   72.5;

  v_hpars2dprof.push_back(hpars2d);

  /*****************************************
   * BOOK 'EM, DANNO...                    *
   *****************************************/

  cutNone_->histos()->book1d<TH1D>(v_hpars1d);
  cutMinHitGeV_->histos()->book1d<TH1D>(v_hpars1d);
  cutMaxHitOnly_->histos()->book1d<TH1D>(v_hpars1d);
#if 0
  cutTgtTwrOnly_->histos()->book1d<TH1D>(v_hpars1d);
  cutAll_->histos()->book1d<TH1D>(v_hpars1d);
#endif

  cutNone_->histos()->book2d<TH2D>(v_hpars2d);
  cutMinHitGeV_->histos()->book2d<TH2D>(v_hpars2d);
  cutMaxHitOnly_->histos()->book2d<TH2D>(v_hpars2d);
#if 0
  cutTgtTwrOnly_->histos()->book2d<TH2D>(v_hpars2d);
  cutAll_->histos()->book2d<TH2D>(v_hpars2d);
#endif

  cutNone_->histos()->book2d<TProfile2D>(v_hpars2dprof);
  cutMinHitGeV_->histos()->book2d<TProfile2D>(v_hpars2dprof);
  cutMaxHitOnly_->histos()->book2d<TProfile2D>(v_hpars2dprof);
#if 0
  cutTgtTwrOnly_->histos()->book2d<TProfile2D>(v_hpars2dprof);
  cutAll_->histos()->book2d<TProfile2D>(v_hpars2dprof);
#endif
}                                                    // bookPerRunHistos

//======================================================================

double HcalTimingAnalAlgos::lookupThresh(const HcalDetId& id)
{
  double thresh;
  switch (id.subdet()) {
  case HcalBarrel: thresh = minHitGeVHB_; break;
  case HcalEndcap: thresh = minHitGeVHE_; break;
  case HcalOuter:  thresh = minHitGeVHO_; break;
  case HcalForward:
    if (id.depth() == 1)      thresh = minHitGeVHF1_;
    else if (id.depth() == 2) thresh = minHitGeVHF2_;
    else                      thresh = 0.0;
    break;
  default:         thresh = 0.0; break;
  }
  return thresh;
}

//======================================================================

void
HcalTimingAnalAlgos::fillDigiPulse(TH1D *pulseHist,
				   const HBHEDataFrame& frame)
{
  for (int isample = 0; isample < std::min(10,frame.size()); ++isample) {
    int rawadc = frame[isample].adc();
    pulseHist->Fill(isample,rawadc);

  } // loop over samples in digi
}

//======================================================================

bool
HcalTimingAnalAlgos::fillHBHEhistos(myAnalHistos *myAH, const HBHERecHit& rh)
{
  HcalDetId detId = rh.id();
  double   htime  = rh.time();
  double   energy = rh.energy();
  int      ieta   = detId.ieta();
  int      iphi   = detId.iphi();
  int      depth  = detId.depth();
  uint32_t flags  = rh.flags();

  myAH->fill1d<TH1D>(st_rhTimes_,htime);
  myAH->fill1d<TH1D>(st_rhEnergies_,energy);
  myAH->fill2d<TH2D>(st_rhEmap_,ieta,iphi,energy);
  myAH->fill2d<TH2D>(st_hbheTimingVsE_,energy,htime);

  for (int i=0; i<32; i++)
    myAH->fill1d<TH1D>(st_hbheRHflags_,i,(flags & (1<<i))!=0);

  if (flags & 1) // HBHE HPD multiplicity bit - noise
    myAH->fill2d<TH2D>(st_hbheTvsEhpdMult_,energy,htime);

  if (flags & 2) // HBHE timing error
    myAH->fill2d<TH2D>(st_hbheTvsEpulseErr_,energy,htime);

  std::string st_rhTprof;
  switch(depth) {
  case 1: st_rhTprof = st_rhTprofd1_; break;
  case 2: st_rhTprof = st_rhTprofd2_; break;
  case 3: st_rhTprof = st_rhTprofd3_; break;
    //case 4: st_rhTprof = st_rhTprofd4_; break; // not for hbhe!
  default:
    edm::LogWarning("Invalid depth in rechit collection! detId = ") << detId << std::endl;
    return false;
  }

  if (energy > 50.0)
    myAH->fill2d<TProfile2D>(st_rhTprof,ieta,iphi,htime);

  return true;
}                                 // HcalTimingAnalAlgos::fillHBHEhistos

//======================================================================

// ------------ method called to for each event  ------------
void
HcalTimingAnalAlgos::process(const myEventData& ed)
{
  using namespace edm;

  uint32_t runn = ed.runNumber();
  uint32_t evtn = ed.evtNumber();

  if (notInSet<uint32_t>(s_runs_,runn)) {
    bookPerRunHistos(runn);
    s_runs_.insert(runn);
  }

  //cout << 'A' << endl;

  std::set<HcalDetId> s_idsOverThresh;

  //cout << 'B' << endl;

  /***************************************************
   *                    HBHE RECHITS                 *
   ***************************************************/

  //HBHERecHit maxrh(HcalDetId::Undefined,-1e99,0.0);
  HcalDetId maxId;
  HBHERecHit maxrh(maxId,-1e99,-1e99);

  cutNone_->histos()->fill1d<TH1D>(st_hbheRHColSize_,(double)ed.hbherechits()->size());

  //cout << 'C' << endl;

  int rechitsOverThresh = 0;
  //int rechitsInTgtTwr   = 0;

#if 0
  if (!ed.hbherechits().isValid()) {
    cerr << "WARNING! hbherechits handle is not valid!" << endl;
  } else {
#endif
  for (unsigned irh = 0; irh < ed.hbherechits()->size (); ++irh) {

    //cout << irh;

    const HBHERecHit& rh = (*(ed.hbherechits()))[irh];

    //cout << irh << endl;
    //std::cout << rh.id().rawId() << std::endl;

    if (!fillHBHEhistos(cutNone_->histos(),rh)) continue;

    double minHitGeV = lookupThresh(rh.id());

    //if (energy > maxrh.energy()) maxrh = rh;
    if (rh.energy() > maxrh.energy()) {
      maxrh = rh;
    }
#if 0
    if (detId == tgtTwrId_) {
      rechitsInTgtTwr++;
      myAnalHistos *myAH = cutTgtTwrOnly_->histos();
    
      myAH->fill1d<TH1D>(st_rhTimes_,htime);
      myAH->fill1d<TH1D>(st_rhEnergies_,energy);
      myAH->fill2d<TH2D>(st_rhEmap_,rh.id().ieta(),rh.id().iphi(),energy);
      myAH->fill2d<TH2D>(st_hbheTimingVsE_,energy,htime);

      if (energy > 50.0)
	myAH->fill2d<TProfile2D>(st_rhTprof,rh.id().ieta(),rh.id().iphi(),htime);
    }
#endif
    if (rh.energy() > minHitGeV) {
      rechitsOverThresh++;
      s_idsOverThresh.insert(rh.id());
      fillHBHEhistos(cutMinHitGeV_->histos(),rh);
    }
  } // loop over HBHE rechits

  cutMinHitGeV_->histos()->fill1d<TH1D>(st_hbheRHColSize_,(double)rechitsOverThresh);
  //cutTgtTwrOnly_->histos()->fill1d<TH1D>(st_hbheRHColSize_,(double)rechitsInTgtTwr);

  //if (maxrh.id() != HcalDetId::Undefined) {
  if (maxrh.energy() > 0.0) {
    fillHBHEhistos(cutMaxHitOnly_->histos(),maxrh);
#if 0
    if ((energy > lookupThresh(maxId)) &&
	(maxId == tgtTwrId_)) {
      cutAll_->histos()->fill1d<TH1D>(st_rhTimes_,htime);
      cutAll_->histos()->fill1d<TH1D>(st_rhEnergies_,energy);
      cutAll_->histos()->fill2d<TH2D>(st_rhEmap_,maxId.ieta(),maxId.iphi(),energy);
      if (energy > 50.0)
	cutAll_->histos()->fill2d<TProfile2D>(st_rhTprof,maxId.ieta(),maxId.iphi(),htime);
    }
#endif
  }
  //  } // else hbherechits handle is valid

  /***************************************************
   *                    HF   RECHITS                 *
   ***************************************************/

  std::map<int,HFtowerEnergies_t> m_HFtowerEnergies;
  for (unsigned irh = 0; irh < ed.hfrechits()->size (); ++irh) {
    const HFRecHit& rh = (*(ed.hfrechits()))[irh];
    HcalDetId detId = rh.id();

    std::string st_rhTprof;
    switch(detId.depth()) {
    case 1: st_rhTprof = st_rhTprofd1_; break;
    case 2: st_rhTprof = st_rhTprofd2_; break;
      //case 3: st_rhTprof = st_rhTprofd3_; break; // not for hf!
      //case 4: st_rhTprof = st_rhTprofd4_; break; // not for hf!
    default:
      edm::LogWarning("Invalid depth in rechit collection! detId = ") << detId << std::endl;
      continue;
    }

    // add up tower energies
    IetaIphi_t ieip(detId.ieta(),detId.iphi());
    map<int,HFtowerEnergies_t>::iterator it = m_HFtowerEnergies.find(ieip.toCode());
    HFtowerEnergies_t tower;

    tower.ieip = ieip;

    if (it == m_HFtowerEnergies.end()) {
      tower.totalE = rh.energy();
      if      (detId.depth() == 1) tower.longE  = rh.energy();
      else if (detId.depth() == 2) tower.shortE = rh.energy();
      m_HFtowerEnergies[ieip.toCode()] = tower;
    }
    else {
      it->second.totalE += rh.energy();
      if      (detId.depth() == 1) it->second.longE  += rh.energy();
      else if (detId.depth() == 2) it->second.shortE += rh.energy();
    }

    double minHitGeV = lookupThresh(detId);

    cutNone_->histos()->fill2d<TH2D>(st_hfTimingVsE_,rh.energy(),rh.time());

    if (rh.flags() & 2) // HF timing error
      cutNone_->histos()->fill2d<TH2D>(st_hfTvsEpulseErr_,rh.energy(),rh.time());

    if (rh.energy() > minHitGeV) {
      cutMinHitGeV_->histos()->fill2d<TH2D>(st_rhEmap_,
					    detId.ieta(),
					    detId.iphi()+detId.depth()-1,
					    rh.energy());

      cutMinHitGeV_->histos()->fill2d<TH2D>(st_hfTimingVsE_,rh.energy(),rh.time());

      if (rh.flags() & 2) // HF timing error
	cutMinHitGeV_->histos()->fill2d<TH2D>(st_hfTvsEpulseErr_,rh.energy(),rh.time());

      if (rh.energy() > 50.0)
	cutMinHitGeV_->histos()->fill2d<TProfile2D>(st_rhTprof,
						    detId.ieta(),
						    detId.iphi()+detId.depth()-1,
						    rh.time());
    }
  } // loop over HF rechits

  map<int,HFtowerEnergies_t>::iterator it;
  for (it  = m_HFtowerEnergies.begin();
       it != m_HFtowerEnergies.end(); it++) {
    double lpluss  = it->second.longE + it->second.shortE;
    double lminuss = it->second.longE - it->second.shortE;
    double sum     = lpluss + lminuss;
    double diff    = lpluss - lminuss;
    cutNone_->histos()->fill2d<TH2D>(st_HFratioVsE_, diff/sum, sum);
  }

  /***************************************************
   *                    HO   RECHITS                 *
   ***************************************************/

  for (unsigned irh = 0; irh < ed.horechits()->size (); ++irh) {
    const HORecHit& rh = (*(ed.horechits()))[irh];
    HcalDetId detId = rh.id();

    std::string st_rhTprof;
    if (detId.depth() != 4) {
      edm::LogWarning("Invalid depth in rechit collection! detId = ") << detId << std::endl;
      continue;
    }

    cutNone_->histos()->fill2d<TH2D>(st_hoTimingVsE_,rh.energy(),rh.time());

    double minHitGeV = lookupThresh(rh.id());

    if (rh.energy() > minHitGeV) {
      cutMinHitGeV_->histos()->fill2d<TH2D>(st_rhEmap_,
					    detId.ieta(),
					    detId.iphi(),
					    rh.energy());

      cutMinHitGeV_->histos()->fill2d<TH2D>(st_hoTimingVsE_,rh.energy(),rh.time());

      if (rh.energy() > 50.0)
	cutMinHitGeV_->histos()->fill2d<TProfile2D>(st_rhTprofd4_,
						    detId.ieta(),
						    detId.iphi(),
						    rh.time());
    }
  } // loop over HO rechits

  /***************************************************
   *                    SIMHITS                      *
   ***************************************************/

  if (doSimHits_ && ed.simhits().isValid()) {
    std::vector<PCaloHit>  simhits;
    simhits.insert(simhits.end(),ed.simhits()->begin(),ed.simhits()->end());

    for (unsigned ish = 0; ish < simhits.size (); ++ish) {
      const PCaloHit& sh = simhits[ish];

      double htime    = sh.time();
      double energy   = sh.energy() * 200;

      if (energy > simHitEnergyMinGeVthreshold_) {
	myAnalHistos *myAH = cutMinHitGeV_->histos();
	myAH->fill1d<TH1D>(st_shTimes_,htime);
	myAH->fill1d<TH1D>(st_shEnergies_,energy);
	myAH->fill2d<TH2D>(st_shTimingVsE_,energy,htime);
      }
      
    } // loop over simhits
  }

  /***************************************************
   *                    HBHE DIGIS                   *
   ***************************************************/

  if (doHBHEdigis_ && ed.hbhedigis().isValid()) {
    // all the different pulses we may want to look at...
    //
    TH1D *uncutPulse = cutNone_->histos()->get<TH1D>(st_avgPulse_);
    TH1D *ecutPulse =  cutMinHitGeV_->histos()->get<TH1D>(st_avgPulse_);
    TH1D *noisePulse = cutNone_->histos()->get<TH1D>(st_avgPulseHPDmult_);
    TH1D *terrPulse  = cutNone_->histos()->get<TH1D>(st_avgPulseTerr_);

    cutNone_->histos()->fill1d<TH1D>(st_hbhedigiColSize_,
				     (double)ed.hbhedigis()->size());

    for (unsigned idig=0, irh=0;
	 idig < ed.hbhedigis()->size() && irh < ed.hbherechits()->size();
	 ++idig, ++irh) {
      const HBHEDataFrame& frame  = (*(ed.hbhedigis()))[idig];
      const HBHERecHit&    rechit = (*(ed.hbherechits()))[irh];
      HcalDetId dgDetId = frame.id();
      HcalDetId rhDetId = rechit.id();

      fillDigiPulse(uncutPulse,frame);
      if (inSet<HcalDetId>(s_idsOverThresh,dgDetId))
	fillDigiPulse(ecutPulse,frame);
      if( dgDetId==rhDetId )  {
	if (rechit.flags() & 1) fillDigiPulse(noisePulse,frame);
	if (rechit.flags() & 2) fillDigiPulse(terrPulse,frame);
      }
    } // loop over digis,rechits together
  } // if have digis


  /***************************************************
   *                    CALO TOWERS                  *
   ***************************************************/

  if (ed.towers().isValid()) {
    //TTree t1("t1","BadTowers");
    CaloTower *badecal = new CaloTower();
    CaloTower *badhcal = new CaloTower();
    CaloTower *badboth = new CaloTower();

    //TBranch *badecalbr = t1.Branch<CaloTower>("BadECAL",&badecal);
    //TBranch *badhcalbr = t1.Branch<CaloTower>("BadHCAL",&badhcal);
    //TBranch *badbothbr = t1.Branch<CaloTower>("BadBoth",&badboth);

    for (unsigned itwr=0; itwr<ed.towers()->size(); ++itwr) {
      const CaloTower& twr = (*(ed.towers()))[itwr];
      double twrEnergy = twr.hadEnergy() + twr.emEnergy(); // + twr.outerEnergy();
      double ecalT     = twr.ecalTime();
      double hcalT     = twr.hcalTime();

      myAnalHistos *myAH = cutNone_->histos();
      myAH->fill2d<TH2D>(st_ctHcalTvstwrE_,twrEnergy,hcalT);
      myAH->fill2d<TH2D>(st_ctEcalTvstwrE_,twrEnergy,ecalT);
      myAH->fill2d<TH2D>(st_ctEcalTvsHcalT_,hcalT,ecalT);

      myAH = cutMinHitGeV_->histos();

      if (twr.hcalTime() > -200.0) {
	if (twrEnergy > 0.) myAH->fill2d<TH2D>(st_ctHcalTvstwrEg_,twrEnergy,hcalT);
	if (twr.ecalTime() > -200.0) {
	  if (twrEnergy > 0) {
	    myAH->fill2d<TH2D>(st_ctEcalTvsHcalTg_,hcalT,ecalT);
	    myAH->fill2d<TH2D>(st_ctEcalTvstwrEg_,twrEnergy,ecalT);
	  }
	} else {
	  *badecal = twr;
	  //badecalbr->Fill();
	}
      } else {
	if (twr.ecalTime() > -200.0) {
	  myAH->fill2d<TH2D>(st_ctEcalTvstwrEg_,twrEnergy,ecalT);
	  *badhcal = twr;
	  //badhcalbr->Fill();
	} else {
	  *badboth = twr;
	  //badbothbr->Fill();
	}
      }
#if 0
      double weightedT =  // doesn't mean much now with hcal energy mean off of zero.
	(hcalT*twr.hadEnergy() + ecalT*twr.emEnergy())/twrEnergy;
      myAH->fill2d<TH2D>(st_ctWeightTvsE_,twrEnergy,weightedT);
      myAH->fill2d<TProfile2D>(st_ctTprof_,twr.ieta(), twr.iphi(), weightedT);
#endif
    } // loop over towers

    //t1.Write("", TObject::kOverwrite);

  } // if have towers


  /***************************************************
   *                    HF   RECHITS                 *
   ***************************************************/

  // MET
  if (ed.recmet().isValid()) {
    const CaloMET *calomet = &(ed.recmet().product()->front());
    cutNone_->histos()->fill1d<TH1D>(st_caloMet_Met_,calomet->pt());
    cutNone_->histos()->fill1d<TH1D>(st_caloMet_Phi_,calomet->phi());
    cutNone_->histos()->fill1d<TH1D>(st_caloMet_SumEt_,calomet->sumEt());
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalTimingAnalAlgos::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTimingAnalAlgos::endJob() {
}

