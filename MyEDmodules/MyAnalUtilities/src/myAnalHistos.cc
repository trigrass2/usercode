// -*- C++ -*-
//
// Package:    myAnalHistos
// Class:      myAnalHistos
// 
/**\class myAnalHistos myAnalHistos.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Phillip Russell DUDERO
//         Created:  Tue Sep  9 13:11:09 CEST 2008
// $Id: myAnalHistos.cc,v 1.1 2009/04/09 22:12:03 dudero Exp $
//
//


// system include files
#include <iostream>
#include <vector>

// user include files
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "MyEDmodules/MyAnalUtilities/interface/myAnalHistos.hh"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1D.h"
#include "TH2D.h"

//======================================================================

//
// constructors and destructor
//
myAnalHistos::myAnalHistos(const std::string& dirdescr)
{
  edm::Service<TFileService> fs;
  dir_ = new TFileDirectory(fs->mkdir(dirdescr));
}
