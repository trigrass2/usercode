// Current issues:
//
#include "MyEDmodules/LjmetAnalyzer/src/GenEvtClass.h"
#include "MyEDmodules/LjmetAnalyzer/src/pdgid.h"

#include <iostream>

#define OTHERCLASS (numClasses()-1)

//======================================================================

using namespace std;
//using namespace HepMC;
using namespace reco;

//======================================================================
void GenEvtClass::loadPdgidStrings(void)
{

// QUARKS
  m_pdgidstr_[1]    = "dq    ";
  m_pdgidstr_[2]    = "uq    ";
  m_pdgidstr_[3]    = "sq    ";
  m_pdgidstr_[4]    = "cq    ";
  m_pdgidstr_[5]    = "bq    ";
  m_pdgidstr_[6]    = "tq    ";

  m_pdgidstr_[-1]   = "/dq   ";
  m_pdgidstr_[-2]   = "/uq   ";
  m_pdgidstr_[-3]   = "/sq   ";
  m_pdgidstr_[-4]   = "/cq   ";
  m_pdgidstr_[-5]   = "/bq   ";
  m_pdgidstr_[-6]   = "/tq   ";

//LEPTONS
  m_pdgidstr_[ 11]  = "e-    ";
  m_pdgidstr_[-11]  = "e+    ";
  m_pdgidstr_[ 12]  = "nue   ";
  m_pdgidstr_[-12]  = "/nue  ";
  m_pdgidstr_[ 13]  = "mu-   ";
  m_pdgidstr_[-13]  = "mu+   ";
  m_pdgidstr_[ 14]  = "numu  ";
  m_pdgidstr_[-14]  = "/numu ";
  m_pdgidstr_[ 15]  = "tau-  ";
  m_pdgidstr_[-15]  = "tau+  ";
  m_pdgidstr_[ 16]  = "nutau ";
  m_pdgidstr_[-16]  = "/nutau";

// GAUGE AND HIGGS BOSONS
  m_pdgidstr_[ 21]  = "g     ";
  m_pdgidstr_[ 22]  = "ph    ";
  m_pdgidstr_[ 23]  = "Z0    ";
  m_pdgidstr_[ 24]  = "W+    ";
  m_pdgidstr_[-24]  = "W-    ";
  m_pdgidstr_[ 25]  = "H     ";

// DIQUARKS
  m_pdgidstr_[1103] = "dd1   ";
  m_pdgidstr_[2101] = "ud0   ";
  m_pdgidstr_[2103] = "ud1   ";
  m_pdgidstr_[2203] = "uu1   ";
  m_pdgidstr_[3101] = "sd0   ";
  m_pdgidstr_[3103] = "sd1   ";
  m_pdgidstr_[3201] = "su0   ";
  m_pdgidstr_[3203] = "su1   ";
  m_pdgidstr_[3303] = "ss1   ";
  m_pdgidstr_[4101] = "cd0   ";
  m_pdgidstr_[4103] = "cd1   ";
  m_pdgidstr_[4201] = "cu0   ";
  m_pdgidstr_[4203] = "cu1   ";
  m_pdgidstr_[4301] = "cs0   ";
  m_pdgidstr_[4303] = "cs1   ";
  m_pdgidstr_[4403] = "cc1   ";
  m_pdgidstr_[5101] = "bd0   ";
  m_pdgidstr_[5103] = "bd1   ";
  m_pdgidstr_[5201] = "bu0   ";
  m_pdgidstr_[5203] = "bu1   ";
  m_pdgidstr_[5301] = "bs0   ";
  m_pdgidstr_[5303] = "bs1   ";
  m_pdgidstr_[5401] = "bc0   ";
  m_pdgidstr_[5403] = "bc1   ";
  m_pdgidstr_[5503] = "bb0   ";

// SUSY PARTICLES
  m_pdgidstr_[ 1000001] = "sdqL  ";
  m_pdgidstr_[-1000001] = "/sdqL ";
  m_pdgidstr_[ 1000002] = "suqL  ";
  m_pdgidstr_[-1000002] = "/suqL ";
  m_pdgidstr_[ 1000003] = "ssqL  ";
  m_pdgidstr_[-1000003] = "/ssqL ";
  m_pdgidstr_[ 1000004] = "scqL  ";
  m_pdgidstr_[-1000004] = "/scqL ";
  m_pdgidstr_[ 1000005] = "sbq1  ";
  m_pdgidstr_[-1000005] = "/sbq1 ";
  m_pdgidstr_[ 1000006] = "stq1  ";
  m_pdgidstr_[-1000006] = "/stq1 ";
  m_pdgidstr_[ 1000011] = "seL   ";
  m_pdgidstr_[-1000011] = "/seL  ";
  m_pdgidstr_[ 1000012] = "snue  ";
  m_pdgidstr_[-1000012] = "/snue ";
  m_pdgidstr_[ 1000013] = "smuL  ";
  m_pdgidstr_[-1000013] = "/smuL ";
  m_pdgidstr_[ 1000014] = "snumu ";
  m_pdgidstr_[-1000014] = "/snumu";
  m_pdgidstr_[ 1000015] = "stau1 ";
  m_pdgidstr_[-1000015] = "/stau1";
  m_pdgidstr_[ 1000016] = "snutau";
  m_pdgidstr_[-1000016] = "/snutau";
  m_pdgidstr_[ 2000001] = "sdqR  ";
  m_pdgidstr_[-2000001] = "/sdqR ";
  m_pdgidstr_[ 2000002] = "suqR  ";
  m_pdgidstr_[-2000002] = "/suqR ";
  m_pdgidstr_[ 2000003] = "ssqR  ";
  m_pdgidstr_[-2000003] = "/ssqR ";
  m_pdgidstr_[ 2000004] = "scqR  ";
  m_pdgidstr_[-2000004] = "/scqR ";
  m_pdgidstr_[ 2000005] = "sbq2  ";
  m_pdgidstr_[-2000005] = "/sbq2 ";
  m_pdgidstr_[ 2000006] = "stq2  ";
  m_pdgidstr_[-2000006] = "/stq2 ";
  m_pdgidstr_[ 2000011] = "seR   ";
  m_pdgidstr_[-2000011] = "/seR  ";
  m_pdgidstr_[ 2000013] = "smuR  ";
  m_pdgidstr_[-2000013] = "/smuR ";
  m_pdgidstr_[ 2000015] = "stau2 ";
  m_pdgidstr_[-2000015] = "/stau2";
  m_pdgidstr_[ 1000021] = "gluino";
  m_pdgidstr_[ 1000022] = "X01   ";
  m_pdgidstr_[ 1000023] = "X02   ";
  m_pdgidstr_[ 1000024] = "X+1   ";
  m_pdgidstr_[ 1000025] = "X03   ";
  m_pdgidstr_[ 1000035] = "X04   ";
  m_pdgidstr_[ 1000037] = "X+2   ";
  m_pdgidstr_[ 1000039] = "~G    ";

// LIGHT I=1 MESONS
  m_pdgidstr_[ 111] = "pi0   ";
  m_pdgidstr_[ 211] = "pi+   ";
  m_pdgidstr_[-211] = "pi-   ";
  m_pdgidstr_[ 113] = "rho770";
  m_pdgidstr_[ 213] = "rho770+";
  m_pdgidstr_[-213] = "rho770-";
  m_pdgidstr_[ 115] = "a2    ";
  m_pdgidstr_[ 215] = "a2+   ";
  m_pdgidstr_[ 117] = "rho3  ";
  m_pdgidstr_[ 217] = "rho3+ ";
  m_pdgidstr_[-217] = "rho3- ";
  m_pdgidstr_[ 119] = "a4    ";
  m_pdgidstr_[ 219] = "a4+   ";
  m_pdgidstr_[-219] = "a4-   ";

// LIGHT I=0 MESONS
  m_pdgidstr_[ 221] = "eta   ";
  m_pdgidstr_[ 331] = "eta'  ";
  m_pdgidstr_[ 223] = "w782  ";
  m_pdgidstr_[ 333] = "phi1020";

// STRANGE MESONS
  m_pdgidstr_[ 130] = "K0    ";
  m_pdgidstr_[ 310] = "K0    ";
  m_pdgidstr_[ 311] = "K0    ";
  m_pdgidstr_[-311] = "/K0   ";
  m_pdgidstr_[ 313] = "K*0   ";
  m_pdgidstr_[-313] = "/K*0  ";
  m_pdgidstr_[ 321] = "K+    ";
  m_pdgidstr_[-321] = "K-    ";
  m_pdgidstr_[ 323] = "K*+   ";
  m_pdgidstr_[-323] = "/K*+  ";

// CHARMED MESONS
  m_pdgidstr_[ 411] = "D+    ";
  m_pdgidstr_[-411] = "D-    ";
  m_pdgidstr_[ 413] = "D*+   ";
  m_pdgidstr_[-413] = "/D*+  ";
  m_pdgidstr_[ 421] = "D0    ";
  m_pdgidstr_[-421] = "/D0   ";
  m_pdgidstr_[ 423] = "D*0   ";
  m_pdgidstr_[-423] = "/D*0  ";

  m_pdgidstr_[ 511] = "B0    ";
  m_pdgidstr_[-511] = "/B0   ";
  m_pdgidstr_[ 521] = "B+    ";
  m_pdgidstr_[-521] = "B-    ";

// LIGHT BARYONS
  m_pdgidstr_[ 2212] = "P     ";
  m_pdgidstr_[-2212] = "/P    ";
  m_pdgidstr_[ 2112] = "N     ";
  m_pdgidstr_[-2112] = "/N    ";
  m_pdgidstr_[ 2224] = "Delta++";
  m_pdgidstr_[-2224] = "/Delta++";
  m_pdgidstr_[ 2214] = "Delta+";
  m_pdgidstr_[-2214] = "/Delta+";
  m_pdgidstr_[ 2114] = "Delta0";
  m_pdgidstr_[-2114] = "/Delta0";
  m_pdgidstr_[ 1114] = "Delta-";
  m_pdgidstr_[-1114] = "/Delta-";
}                                                    // loadPdgidStrings

//======================================================================

std::string GenEvtClass::pdgidStr(int pdgid) {
  std::map<int,std::string>::const_iterator it = m_pdgidstr_.find(pdgid);
  
  if (it == m_pdgidstr_.end()) {
    ostringstream ss;
    ss << pdgid;
    return ss.str();
  }

  return it->second;
}                                                            // pdgidStr

//======================================================================

myParticleRecord *GenEvtClass::newParticleRecord(void)
{
  vector<int> pdgidvec;

  /****************************************
   * Initialize Generated Particle Record
   ****************************************/

  pdgidvec.push_back(downq);
  pdgidvec.push_back(upq);
  pdgidvec.push_back(strangeq);
  pdgidvec.push_back(charmq);
  pdgidvec.push_back(bottomq);
  pdgidvec.push_back(topq);

  pdgidvec.push_back(electron);
  pdgidvec.push_back(nu_e);
  pdgidvec.push_back(muminus);
  pdgidvec.push_back(nu_mu);
  pdgidvec.push_back(tauminus);
  pdgidvec.push_back(nu_tau);

  return new myParticleRecord(pdgidvec);
}

//======================================================================

GenEvtClass::GenEvtClass(bool verbosity) :
  verbosity_(verbosity)
{
  myprec_ = newParticleRecord();
  loadPdgidStrings();
}

//======================================================================

GenEvtClass::~GenEvtClass()
{
  if (myprec_)
    delete myprec_;
}

//======================================================================

int GenEvtClass::numClasses() const
{
  return 5;
}

//======================================================================

std::string GenEvtClass::classDescr(int classnum) const
{
  switch(classnum) {
  case 0: return "susy";
  case 1: return "ttbar";
  case 2: return "wjets";
  case 3: return "zjets";
  case 4: return "other";
  }

  return "unclassed";
}

//======================================================================

int GenEvtClass::numSignatureTypes() const
{
  return 5;
}

//======================================================================

std::string GenEvtClass::signDescr(GenEvtClass::EnumSignature_t sig) const
{
  switch(sig) {
  case GenEvtClass::eNOLEPTONS:      return "0leptons";
  case GenEvtClass::e1ELEC:          return "1e";
  case GenEvtClass::e1ELECpLEPTONS:  return "1e+lepton(s)";
  case GenEvtClass::e1MUorTAU:       return "1muORtau";
  case GenEvtClass::e2orMOREMUorTAU: return "2+muORtau";
  }

  return "other";
}

//======================================================================

GenEvtClass::EnumSignature_t GenEvtClass::detSignatureClass(void)
{
  if (getElectronCount() == 0) {
    if       (getNonELeptonCount()  > 1) return GenEvtClass::e2orMOREMUorTAU;
    else if  (getNonELeptonCount() == 1) return GenEvtClass::e1MUorTAU;
    else                                 return GenEvtClass::eNOLEPTONS;
  }
  else if (getElectronCount() == 1) {
    if       (getNonELeptonCount()) return GenEvtClass::e1ELECpLEPTONS;
    else                            return GenEvtClass::e1ELEC;
  }

  return GenEvtClass::e1ELECpLEPTONS;
}

//======================================================================
//
void GenEvtClass::printTree(HepMC::GenParticle *p, int depth)
{
  int pdgid = abs(p->pdg_id());

  cout << setw(7) << pdgidStr(pdgid);

  if (!p->end_vertex() ||  (pdgid == 92)) {  cout << endl; return; }

  HepMC::GenVertex::particle_iterator child;
  HepMC::GenVertex::particle_iterator begin;
  begin = p->end_vertex ()->particles_begin (HepMC::children);
  for (child  = begin;
       child != p->end_vertex ()->particles_end   (HepMC::children); 
       ++child) {
    if (child != begin) {
      cout << "       ";
      for (int i=0; i<depth; i++) cout << "          ";
    }
    cout << " => ";
    printTree(*child, depth+1);
  }
}                                               // GenEvtClass:printTree

//======================================================================

void GenEvtClass::printCounts(myParticleRecord& prec)
{
  cout << "#e = "        << prec.count(electron);
  cout << ", #nu_e = "   << prec.count(nu_e);
  cout << ", #mu- = "    << prec.count(muminus);
  cout << ", #nu_mu = "  << prec.count(nu_mu);
  cout << ", #tau- = "   << prec.count(tauminus);
  cout << ", #nu_tau = " << prec.count(nu_tau);
  cout << ", #quark = "  << prec.quarkcount() << endl;
}

//======================================================================
// pMap keeps track of visited particles as we traverse the tree.
// Look for end vertices and count up final state particles.
// Classify the event according to the current particle, and also count
// up the leptons tau, mu, and e.
// So we collect classification codes from child recursion calls and see
// if a particular lepton showed up amongst the children. Return abs(pdgid)
// until the classification is known, then return -1*class back up the
// recursion.
//
int GenEvtClass::recurseTree(HepMC::GenParticle *p,
			     int depth,
			     std::map<int,HepMC::GenParticle *>& pMap,
			     myParticleRecord *prec)
{
  int evtclass = -1*numClasses();

  std::map<int,HepMC::GenParticle *>::const_iterator it;
  it = pMap.find(p->barcode());

  int pdgid   = abs(p->pdg_id());
  int barcode = p->barcode();

  /*********************************
   * RECURSION TERMINUS CONDITIONS *
   *********************************/

  /* 1. particle already visited - 
  **    no need to traverse the same subtree twice!
  */
  if (it == pMap.end())
    pMap [barcode] = p;
  else {
    //if(verbosity_) cout<<"double-linked barcode "<<barcode<<" pdgid "<<pdgid<<endl;
    return (depth ? pdgid : (-1*OTHERCLASS));
  }

  /* 2. String encountered - she's gonna hadronize!
  */
  if (pdgid == 92) return (depth ? pdgid : (-1*OTHERCLASS));

  /* 3. Final state particles encountered
  */
  if ((pdgid == tauminus) ||  // count it as a final state particle and stop
      (pdgid == muminus) ||
      (!p->end_vertex())) {
    prec->add(pdgid);
    return (depth ? pdgid : (-1*OTHERCLASS));
  }

  /*************************************************************************
   * Recurse the children in all other cases to count final state particles
   *************************************************************************/

  std::map<int, HepMC::GenParticle *> m_childretvals;

  HepMC::GenVertex::particle_iterator child;
  HepMC::GenVertex::particle_iterator begin;
  begin = p->end_vertex ()->particles_begin (HepMC::children);
  for (child  = begin;
       child != p->end_vertex ()->particles_end   (HepMC::children); 
       ++child) {

    int retval = recurseTree(*child, depth+1, pMap, prec);
#if 0
    if (verbosity_) {
      cout << "Depth " << depth+1 << ": retval = " << retval << endl;
      printCounts(*prec);
    }
#endif
    m_childretvals[retval] = (*child); // don't care if it overwrites
  }

  /***************************************************
   * Now try classifying event on current particles...
   ***************************************************/

  if ((pdgid > 1000000) &&
      (pdgid < 3000000)) evtclass =  0;    // susy
  if (pdgid == topq)     evtclass = -1;    // top
  if (pdgid == Wplus)    evtclass = -2;    // W
  if (pdgid == Z0boson)  evtclass = -3;    // Z
  if (pdgid == Higgs)    evtclass = -4;    // Higgs->other

  if (evtclass != -1*numClasses()) return evtclass;

  /**************************
   * Or on child particles...
   **************************/

  // No class determined yet, inspect the kid pdgids...
  // return highest classification from among the children...
  //
  for (int i=0; i>=-3; i--)
    if (m_childretvals.find(i)  != m_childretvals.end()) {
      evtclass =  i;
      break;
    }
  
  if (evtclass != -1*numClasses()) return evtclass;

  return (depth ? pdgid : (-1*OTHERCLASS));
}                                             // GenEvtClass:recurseTree

//======================================================================

void GenEvtClass::classifyEvent(const HepMC::GenEvent& genEvt,
				EnumSample_t&    sampleclass,
				EnumSignature_t& signatureclass)
{
  int eventclass = numClasses();
  int nParticles = genEvt.particles_size();
    
  if (verbosity_) {
    cout << "          Number of Raw Particles collected:         ";
    cout << nParticles << endl;
  }

  myprec_->clearcounts();
  std::map<int,HepMC::GenParticle *> pMap;

  HepMC::GenEvent::particle_const_iterator p;
  for (p  = genEvt.particles_begin ();
       p != genEvt.particles_end ();
       ++p) {
    if (!(*p)->production_vertex()) { // top of the tree
      myParticleRecord *prec = newParticleRecord();
      int evtclass = recurseTree(*p,0,pMap,prec);
      if (evtclass < 0) evtclass *= -1;
      if (evtclass < numClasses()) {
	myprec_->Add(*prec);
      }
      if (verbosity_ && (evtclass!=numClasses())) printTree(*p,0);
      if (evtclass < eventclass) // found a "higher" event class
	eventclass = evtclass;
      delete prec;
    }
  }

  signatureclass = detSignatureClass();

  if (verbosity_) {
    cout << "event class = " << eventclass << " = " << classDescr(eventclass) << endl;
    printCounts(*myprec_);
    cout << "signature class = " << signDescr(signatureclass) << endl;
  }

  sampleclass = EnumSample_t(eventclass);

}                                          // GenEvtClass::classifyEvent

//======================================================================
//
void GenEvtClass::printTree(const Candidate &p, int depth)
{
  int pdgid = abs(p.pdgId());

  cout << setw(7) << pdgidStr(pdgid);

  int n = p.numberOfDaughters();
  if (!n ||  (pdgid == 92)) {  cout << endl; return; }

  for( int j = 0; j < n; ++j ) {
    const Candidate * d = p.daughter(j);
    if (j) {
      cout << "       ";
      for (int i=0; i<depth; i++) cout << "          ";
    }
    cout << " => ";
    if (d) printTree(*d, depth+1);
    else   cout << endl;
  }
}                                               // GenEvtClass:printTree

//======================================================================
// Same thing, but with GenParticleCandidates:
//
int GenEvtClass::recurseTree(const Candidate &p,
			     int depth,
			     std::map<const Candidate *, int>& pMap,
			     myParticleRecord *prec)
{
  int evtclass = -1*numClasses();

  int pdgid   = abs(p.pdgId());
  unsigned int n = p.numberOfDaughters();

  std::map<const Candidate *, int>::const_iterator it;
  it = pMap.find(&p);

  /*********************************
   * RECURSION TERMINUS CONDITIONS *
   *********************************/

  /* 1. particle already visited - 
  **    no need to traverse the same subtree twice!
  */
  if (it == pMap.end())
    pMap [&p] = pdgid;
  else {
    //if(verbosity_) cout<<"we been here before...pdgid "<<pdgid<<endl;
    return (depth ? pdgid : (-1*OTHERCLASS));
  }

  /* 2. String encountered - she's gonna hadronize!
  */
  if (pdgid == 92) return (depth ? pdgid : (-1*OTHERCLASS));

  /* 3. Final state particles encountered
  */
  if ((pdgid == tauminus) ||  // count it as a final state particle and stop
      (pdgid == muminus) ||
      (!n)) {
    prec->add(pdgid);
    return (depth ? pdgid : (-1*OTHERCLASS));
  }

  /*************************************************************************
   * Recurse the children in all other cases to count final state particles
   *************************************************************************/

  std::map<int, const Candidate *> m_childretvals;

  /* Visit all the children:
   */
  for( size_t j=0; j < n; ++j ) {
    const Candidate * d = p.daughter(j);

    if (!d) {
      cout << "daughter #" << j << " of " << n << " pointer is 0???" << endl;
      continue;
    }
    
    int retval = recurseTree(*d, depth+1, pMap, prec);
#if 0
    if (verbosity_) {
      cout << "Depth " << depth+1 << ": retval = " << retval << endl;
      printCounts(*prec);
    }
#endif
    m_childretvals[retval] = d; // don't care if it overwrites
  }

  /***************************************************
   * Now try classifying event on current particles...
   ***************************************************/

  if ((pdgid > 1000000) &&
      (pdgid < 3000000)) evtclass =  0;    // susy
  if (pdgid == topq)     evtclass = -1;    // top
  if (pdgid == Wplus)    evtclass = -2;    // W
  if (pdgid == Z0boson)  evtclass = -3;    // Z
  if (pdgid == Higgs)    evtclass = -4;    // Higgs->other

  if (evtclass != -1*numClasses()) return evtclass;

  /**************************
   * Or on child particles...
   **************************/

  // No class determined yet, inspect the kid pdgids...
  // return highest classification from among the children...
  //
  for (int i=0; i>=-3; i--)
    if (m_childretvals.find(i)  != m_childretvals.end()) {
      evtclass =  i;
      break;
    }
  
  if (evtclass != -1*numClasses()) return evtclass;

  return (depth ? pdgid : (-1*OTHERCLASS));
}                                             // GenEvtClass:recurseTree
#if 1
//======================================================================

static void DumpCollection(const reco::GenParticleCollection& genParticles)
{
  char s[256];
  cout << " i  pdgid   nm nd  daughters" << endl;
  for( size_t i = 0; i < genParticles.size(); ++ i ) {
    const GenParticle p = genParticles[i];
    int ndau  = p.numberOfDaughters();
    int nmoth = p.numberOfMothers();
    int pdgid = p.pdgId();

    sprintf(s,"%3d %7d %2d %2d ", i, pdgid, nmoth, ndau); cout << s;
    for (int j = 0; j<nmoth; j++) {
      const Candidate *m = p.mother(j);
      sprintf (s, "0x%08x ", (int)m); cout << s;
    }
    cout << "| ";
    for (int j = 0; j<ndau; j++) {
      const Candidate *d = p.daughter(j);
      sprintf (s, "0x%08x ", (int)d); cout << s;
    }
    cout << endl;
  }
}
#endif
//======================================================================

void GenEvtClass::classifyEvent(const reco::GenParticleCollection& genParticles,
				EnumSample_t&    sampleclass,
				EnumSignature_t& signatureclass)
{
  int eventclass = numClasses();
  int nParticles = genParticles.size();
    
  if (verbosity_) {
    cout << "          Number of Raw Particles collected:         ";
    cout << nParticles << endl;
  }

  //DumpCollection(genParticles);

  myprec_->clearcounts();
  std::map<const Candidate *,int> pMap;

  for( size_t i = 0; i < genParticles.size(); ++ i ) {
    const Candidate &p = genParticles[i];

    if (!p.mother()) { // top of the tree
      myParticleRecord *prec = newParticleRecord();
      int evtclass = recurseTree(p,0,pMap,prec);
      if (evtclass < 0) evtclass *= -1;
      if (evtclass < numClasses()) {
	myprec_->Add(*prec);
      }
      if (verbosity_ && (evtclass!=numClasses())) printTree(p,0);
      if (evtclass < eventclass) // found a "higher" event class
	eventclass = evtclass;
      delete prec;
    }
  }

  signatureclass = detSignatureClass();

  if (verbosity_) {
    cout << "event class = " << eventclass << " = " << classDescr(eventclass) << endl;
    if (abs(eventclass) > numClasses())
      cout << "eventclass = " << eventclass << endl;
    printCounts(*myprec_);
    cout << "signature class = " << signDescr(signatureclass) << endl;
  }

  sampleclass = EnumSample_t(eventclass);

}                                          // GenEvtClass::classifyEvent

//======================================================================
