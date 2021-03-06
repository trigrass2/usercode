#ifndef MYPARTICLERECORD_H
#define MYPARTICLERECORD_H

using namespace std;

#include <map>
#include <vector>

/** \class myParticleRecord
 *  
 *  $Id$
 */


class myParticleRecord {
 public:

  // Constructor, all counts set to zero
  myParticleRecord(vector<int> pdgids);

  int found(int pdgid);              /* returns whether  particle  of given id
					is contained in the map */
  int count(int pdgid);              // returns count of particles of given id
  void clearcounts();                 // returns count of particles of given id
  int add(int pdgid);                // adds to count of particle of given id

  int quarkcount();                  // returns total number of quarks
  int leptoncount();                 // returns total number of leptons

 private:
  typedef struct {
    int count;
  } prec_t;
  map<int,prec_t> m_particleRec_;
};

#endif
