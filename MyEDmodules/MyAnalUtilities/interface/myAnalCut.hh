#ifndef _MYANALCUT
#define _MYANALCUT

// -*- C++ -*-
//
// Package:    myAnalCut
// Class:      myAnalCut
// 
/**\class myAnalCut myAnalCut.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: myAnalCut.hh,v 1.6 2010/05/05 23:37:07 dudero Exp $
//
//

// system include files
#include <string>
#include <vector>

// user include files

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MyEDmodules/MyAnalUtilities/interface/myAnalHistos.hh"

//
// class declaration
//
class myAnalCut {
public:
  myAnalCut(const int cutnum,
	    const std::string& descr, 
	    TFileDirectory& rootDir,
	    bool  doInverted = false ) :
    active_(false), cutnum_(cutnum), evtCount_(0), cutdescr_(descr), pInverted_(0)
  {
    std::string invd="~"+descr;
    pHistos_ = new myAnalHistos(descr,rootDir);
    if (doInverted) pInverted_ = new myAnalHistos(invd,rootDir);
    userFlags_.clear();
    userFlagsInv_.clear();
  }

  inline void                 Activate  (bool val){ active_ = val;     }
  inline bool                 isActive()    const { return active_;    }
  inline int                  nEvents()     const { return evtCount_;  }
  inline myAnalHistos        *cuthistos()   const { return pHistos_;   }
  inline myAnalHistos        *invhistos()   const { return pInverted_; }
  inline const std::string&   description() const { return cutdescr_;  }
  inline bool                 doInverted()  const { return (bool)pInverted_; }

  inline void setFlag(const std::string& flagname){ userFlags_[flagname] = true; }
  inline void clrFlag(const std::string& flagname){ userFlags_[flagname] = false;}
  inline bool flagSet(const std::string& flagname){
    std::map<std::string,bool>::const_iterator it = userFlags_.find(flagname);
    if (it == userFlags_.end()) return false;
    return it->second;
  }
  
  inline void setFlagInv(const std::string& flagname){ userFlagsInv_[flagname] = true; }
  inline void clrFlagInv(const std::string& flagname){ userFlagsInv_[flagname] = false;}
  inline bool flagSetInv(const std::string& flagname){
    std::map<std::string,bool>::const_iterator it = userFlagsInv_.find(flagname);
    if (it == userFlagsInv_.end()) return false;
    return it->second;
  }
  
private:
  bool           active_;
  int            cutnum_;
  uint32_t       evtCount_;
  std::string    cutdescr_;
  myAnalHistos  *pHistos_;
  myAnalHistos  *pInverted_;
  std::map<std::string, bool> userFlags_;
  std::map<std::string, bool> userFlagsInv_;
};

#endif // _MYANALCUT
