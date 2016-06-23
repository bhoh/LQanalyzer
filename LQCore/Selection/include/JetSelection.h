#ifndef JetSelection_h
#define JetSelection_h

#include <iostream>
using namespace std;


#include "TLorentzVector.h"
#include <vector>
#include "LQEvent.h"
#include "KJet.h"
#include "KMuon.h"
#include "KElectron.h"
#include "BaseSelection.h"

class JetSelection : public BaseSelection {

 public:
  JetSelection(LQEvent ev);
  ~JetSelection();

  JetSelection& operator= (const JetSelection& obj);
  JetSelection(const JetSelection& bs);

 
  void Selection (std::vector<snu::KJet>& jetColl);
  void BasicSelection (std::vector<snu::KJet>& jetColl);
  
  bool PassUserID (ID id, snu::KJet jet);
  bool PassUserID_PFJetLoose( snu::KJet jet);
  bool PassUserID_PFJetMedium( snu::KJet jet);
  bool PassUserID_PFJetTight( snu::KJet jet);
  
  //// To be used in analysis
  void JetSelectionLeptonVeto(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muons, std::vector<snu::KElectron> electrons);
  void JetSelectionRealLeptonVeto(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muons, std::vector<snu::KElectron> electrons);
  void JetSelectionRealLeptonVetoV2(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muons, std::vector<snu::KElectron> electrons);
  void JetHNSelection(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl);

  void JetTopSelection(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl);
  void JetTopUESelection(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl); 
  void JetchsTopSelection(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl);
  void JetchsTopUESelection(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl); 


  void JetchsTopSelectionv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl);
  void JetchsTopSelectionv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int method);
  void JetchsTopSelectionJESv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int up);
  void JetchsTopSelectionJERv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int up);
  void JetchsTopUESelectionv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl); 
  void JetchsTopUESelectionv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int method); 
  void JetchsTopUESelectionJESv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int up); 
  void JetchsTopUESelectionJERv1(std::vector<snu::KJet>& jetColl, std::vector<snu::KMuon> muonColl, std::vector<snu::KElectron> electronColl, int up); 

};

#endif
