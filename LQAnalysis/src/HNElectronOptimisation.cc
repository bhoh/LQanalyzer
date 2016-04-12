// $id: ExampleAnalyzer.cc 1 2013-11-26 10:23:10Z jalmond $
/***************************************************************************
 * @Project: LQHNElectronOptimisation Frame - ROOT-based analysis framework for Korea SNU
 * @Package: LQCycles
 *
 * @author John Almond       <jalmond@cern.ch>           - SNU
 *
 ***************************************************************************/

/// Local includes
#include "HNElectronOptimisation.h"

//Core includes
#include "Reweight.h"
#include "EventBase.h"                                                                                                                           
#include "BaseSelection.h"

//// Needed to allow inheritance for use in LQCore/core classes
ClassImp (HNElectronOptimisation);


/**
 *   This is an Example Cycle. It inherits from AnalyzerCore. The code contains all the base class functions to run the analysis.
 *
 */
HNElectronOptimisation::HNElectronOptimisation() :  AnalyzerCore(),  out_electrons(0) {

  // To have the correct name in the log:                                                                                                                            
  SetLogName("HNElectronOptimisation");

  Message("In HNElectronOptimisation constructor", INFO);
  //
  // This function sets up Root files and histograms Needed in ExecuteEvents
  InitialiseAnalysis();
}


void HNElectronOptimisation::InitialiseAnalysis() throw( LQError ) {
  
  /// Initialise histograms
  MakeHistograms();  
  //
  // You can out put messages simply with Message function. Message( "comment", output_level)   output_level can be VERBOSE/INFO/DEBUG/WARNING 
  // You can also use m_logger << level << "comment" << int/double  << LQLogger::endmsg;
  //

   Message("Making clever hists for Z ->ll test code", INFO);

   //// Initialise Plotting class functions
   /// MakeCleverHistograms ( type, "label")  type can be muhist/elhist/jethist/sighist
   m_os_Z_nw = 0;
   m_os_Z = 0;
   m_ss_Z_nw = 0;
   m_ss_Z = 0;
   return;
}


void HNElectronOptimisation::ExecuteEvents()throw( LQError ){

  
  m_logger << DEBUG << "RunNumber/Event Number = "  << eventbase->GetEvent().RunNumber() << " : " << eventbase->GetEvent().EventNumber() << LQLogger::endmsg;
  m_logger << DEBUG << "isData = " << isData << LQLogger::endmsg;
  
  FillCutFlow("NoCut", weight);
  
  if(!PassBasicEventCuts()) return;     /// Initial event cuts  
  FillCutFlow("EventCut", weight);
  
  /// Trigger List 
  std::vector<TString> triggerslist;  
  triggerslist.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
  if(!PassTrigger(triggerslist, prescale)) return;
  
  //// if the trigger that fired the event is prescaled you can reweight the event accordingly using the variable prescale
  
  FillCutFlow("TriggerCut", weight);
  m_logger << DEBUG << "passedTrigger "<< LQLogger::endmsg;
  
    
  if (!eventbase->GetEvent().HasGoodPrimaryVertex()) return; //// Make cut on event wrt vertex
  FillCutFlow("VertexCut", weight);

  /// Correct MC for pileup   
  if (MC_pu&&!k_isdata) {
    weight  = weight* reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()))* MCweight;
  }
  m_logger << DEBUG << "reweighted pileup "<< LQLogger::endmsg;

  //////////////////////////////////////////////////////
  //////////// Select objetcs
  //////////////////////////////////////////////////////   

  std::vector<snu::KJet> jetColl;
  eventbase->GetJetSel()->SetID(BaseSelection::PFJET_LOOSE);
  eventbase->GetJetSel()->SetPt(20.);
  eventbase->GetJetSel()->SetEta(2.5);
  eventbase->GetJetSel()->Selection(jetColl);
  
  /// ELECTRONS
  std::vector<snu::KElectron> _electronAnalysisColl;
  
  if(k_running_nonprompt) eventbase->GetElectronSel()->HNLooseElectronSelection(_electronAnalysisColl);
  else eventbase->GetElectronSel()->HNTightElectronSelection(_electronAnalysisColl);


  for(unsigned int i =0; i < _electronAnalysisColl.size(); i++){
    // is the mother a tau
    int mother_pdgid=_electronAnalysisColl.at(i).MotherPdgId();
    if(fabs(mother_pdgid) == 15){
      FillHist("TauEl_d0",_electronAnalysisColl.at(i).PrimaryVertexDXY()  , weight, -3.,3.,600);
    }
  }
  
  
  if(_electronAnalysisColl.size() >= 1){
    FillHist("MC_type",_electronAnalysisColl.at(0).GetType()  , weight, 0.,10.,10);
    if(_electronAnalysisColl.size() >= 2){
      FillHist("MC_type",_electronAnalysisColl.at(1).GetType()  , weight, 0.,10.,10);
    }
  }
  
  
  /// Get Prompt electrons/CF
  std::vector<snu::KElectron> electronAnalysisColl =GetTruePrompt(_electronAnalysisColl, true,false);
  
  std::vector<snu::KElectron> electronVetoColl;
  eventbase->GetElectronSel()->HNVetoElectronSelection(electronVetoColl);
  
  std::vector<snu::KElectron> _electronLooseColl;
  eventbase->GetElectronSel()->HNLooseElectronSelection(_electronLooseColl);
  
  std::vector<snu::KElectron> _electronLooseColl_medium;
  eventbase->GetElectronSel()->HNLooseElectronSelection(false,_electronLooseColl_medium);
  std::vector<snu::KElectron> electronLooseColl_medium= GetTruePrompt(_electronLooseColl_medium, true,false);

  std::vector<snu::KElectron> electronLooseColl = GetTruePrompt(_electronLooseColl, true,false);


  std::vector<snu::KElectron>  electronLooseColl1;
  std::vector<snu::KElectron>  electronLooseColl2;
  std::vector<snu::KElectron>  electronLooseColl3;
  std::vector<snu::KElectron>  electronLooseColl4;
  
  std::vector<snu::KElectron> electronNoCutColl;
  eventbase->GetElectronSel()->Selection(electronNoCutColl);
  
  std::vector<snu::KElectron> electronPtEtaCutColl;
  std::vector<snu::KElectron> electronMedium_chargeconst;
  std::vector<snu::KElectron> electronTight;

  std::vector<snu::KElectron> electronTight_convveto;
  std::vector<snu::KElectron> electronTight_d0veto;  
  std::vector<snu::KElectron> electronTight_reliso;
  std::vector<snu::KElectron> electronTight_chargeconst;


  // For d0/iso opt
  std::vector<snu::KElectron> electronLooseColl_nodxy;
  std::vector<snu::KElectron> electronLooseColl_dxy_05;
  std::vector<snu::KElectron> electronLooseColl_dxy_10;
  std::vector<snu::KElectron> electronLooseColl_dxy_15;
  std::vector<snu::KElectron> electronLooseColl_dxy_20;
  std::vector<snu::KElectron> electronLooseColl_dxy_25;
  std::vector<snu::KElectron> electronLooseColl_dxy_30;
  

  std::vector<snu::KElectron> electronLooseColl_iseref;
  std::vector<snu::KElectron> electronLooseColl_dr03_150;
  std::vector<snu::KElectron> electronLooseColl_dr03_125;
  std::vector<snu::KElectron> electronLooseColl_dr03_100;
  std::vector<snu::KElectron> electronLooseColl_dr03_090;
  std::vector<snu::KElectron> electronLooseColl_dr03_080;
  std::vector<snu::KElectron> electronLooseColl_dr03_070;
  std::vector<snu::KElectron> electronLooseColl_dr03_060;
  std::vector<snu::KElectron> electronLooseColl_dr03_050;

  std::vector<snu::KElectron> electronLooseColl_dr04_150;
  std::vector<snu::KElectron> electronLooseColl_dr04_125;
  std::vector<snu::KElectron> electronLooseColl_dr04_100;
  std::vector<snu::KElectron> electronLooseColl_dr04_090;
  std::vector<snu::KElectron> electronLooseColl_dr04_080;
  std::vector<snu::KElectron> electronLooseColl_dr04_070;
  std::vector<snu::KElectron> electronLooseColl_dr04_060;
  std::vector<snu::KElectron> electronLooseColl_dr04_050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b150_e050;
  
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b125_e050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b100_e050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b090_e050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b080_e050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b070_e050;

  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e125;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e100;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e090;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e080;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e070;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e060;
  std::vector<snu::KElectron>  electronLooseColl_dr03_b050_e050;
  
  /// add trkiso
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b150_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b125_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b100_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b090_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b080_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b070_e050;

  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e125;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e100;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e090;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e080;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e070;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e060;
  std::vector<snu::KElectron>  electronLooseColl_NPFisodr03_b050_e050;


  for(unsigned int iel = 0; iel < electronLooseColl.size(); iel++){
    if(fabs(electronLooseColl.at(iel).dxy()) < 0.01 ) electronLooseColl1.push_back(electronLooseColl.at(iel));
    if(fabs(electronLooseColl.at(iel).dxy()) < 0.02 ) electronLooseColl2.push_back(electronLooseColl.at(iel));

    
    Double_t PHONH_03[7]          = {0.13, 0.14, 0.07, 0.09, 0.11, 0.11, 0.14};
    Double_t PHONH_04[7]          = {0.208, 0.209, 0.115, 0.143, 0.183, 0.194, 0.261};
    int ifid = 0;
    if (fabs(electronLooseColl.at(iel).SCEta()) < 1.0) ifid = 0;
    else if (fabs(electronLooseColl.at(iel).SCEta()) < 1.479) ifid = 1;
    else if (fabs(electronLooseColl.at(iel).SCEta()) < 2.0) ifid = 2;
    else if (fabs(electronLooseColl.at(iel).SCEta()) < 2.2) ifid = 3;
    else if (fabs(electronLooseColl.at(iel).SCEta()) < 2.3) ifid = 4;
    else if (fabs(electronLooseColl.at(iel).SCEta()) < 2.4) ifid = 5;
    else ifid = 6;

    float LeptonRelIsoDR03(0.);
    float LeptonRelIsoDR04(0.);
    float trkiso = electronLooseColl.at(iel).TrkIsoDR03();
    float ecaliso = electronLooseColl.at(iel).ECalIsoDR03();
    float hcaliso = electronLooseColl.at(iel).HCalIsoDR03();
    float NPFiso = (trkiso + hcaliso + ecaliso)/electronLooseColl.at(iel).Pt();

    float ElectronIsoDR03 =  electronLooseColl.at(iel).PFChargedHadronIso03() + max( electronLooseColl.at(iel).PFNeutralHadronIso03() + electronLooseColl.at(iel).PFPhotonIso03() - eventbase->GetEvent().JetRho() * PHONH_03[ifid],  0.);

    float ElectronIsoDR04 =  electronLooseColl.at(iel).PFChargedHadronIso04() + max( electronLooseColl.at(iel).PFNeutralHadronIso04() + electronLooseColl.at(iel).PFPhotonIso04() - eventbase->GetEvent().JetRho() * PHONH_04[ifid],  0.);
    if(electronLooseColl.at(iel).Pt() > 0.)  LeptonRelIsoDR03 = ElectronIsoDR03/  electronLooseColl.at(iel).Pt();
    if(electronLooseColl.at(iel).Pt() > 0.)  LeptonRelIsoDR04 = ElectronIsoDR04/  electronLooseColl.at(iel).Pt();

    if (fabs(electronLooseColl.at(iel).SCEta()) < 1.479 ){

      if(LeptonRelIsoDR03 < 0.09){
        electronLooseColl3.push_back(electronLooseColl.at(iel));
        if(fabs(electronLooseColl.at(iel).dxy()) < 0.02 )         electronLooseColl4.push_back(electronLooseColl.at(iel));
      }
      
      if(LeptonRelIsoDR03 < 0.1){
	electronLooseColl_nodxy.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.005) electronLooseColl_dxy_05.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.010) electronLooseColl_dxy_10.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.015) electronLooseColl_dxy_15.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.020) electronLooseColl_dxy_20.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.025) electronLooseColl_dxy_25.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.030) electronLooseColl_dxy_30.push_back(electronLooseColl.at(iel));
      }
    }
    else{
      
      if(LeptonRelIsoDR03 < 0.07){
        if(fabs(electronLooseColl.at(iel).dxy()) < 0.02 )         electronLooseColl4.push_back(electronLooseColl.at(iel));
        electronLooseColl3.push_back(electronLooseColl.at(iel));
      }
      if(LeptonRelIsoDR03 < 0.1){
	electronLooseColl_nodxy.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.005) electronLooseColl_dxy_05.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.010) electronLooseColl_dxy_10.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.015) electronLooseColl_dxy_15.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.020) electronLooseColl_dxy_20.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.025) electronLooseColl_dxy_25.push_back(electronLooseColl.at(iel));
	if(fabs(electronLooseColl.at(iel).dxy()) < 0.030) electronLooseColl_dxy_30.push_back(electronLooseColl.at(iel));
      }
    }
    
    if(fabs(electronLooseColl.at(iel).dxy()) < 0.010){
      electronLooseColl_iseref.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.15)  electronLooseColl_dr03_150.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.125) electronLooseColl_dr03_125.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.1)   electronLooseColl_dr03_100.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.09)  electronLooseColl_dr03_090.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.08)  electronLooseColl_dr03_080.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.07)  electronLooseColl_dr03_070.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.06)  electronLooseColl_dr03_060.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR03 < 0.05)  electronLooseColl_dr03_050.push_back(electronLooseColl.at(iel));
      
      if(LeptonRelIsoDR04 < 0.15)  electronLooseColl_dr04_150.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.125) electronLooseColl_dr04_125.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.1)   electronLooseColl_dr04_100.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.09)  electronLooseColl_dr04_090.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.08)  electronLooseColl_dr04_080.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.07)  electronLooseColl_dr04_070.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.06)  electronLooseColl_dr04_060.push_back(electronLooseColl.at(iel));
      if(LeptonRelIsoDR04 < 0.05)  electronLooseColl_dr04_050.push_back(electronLooseColl.at(iel));
      
      if (fabs(electronLooseColl.at(iel).SCEta()) < 1.479 ){
	if(LeptonRelIsoDR03 < 0.15) {
	  electronLooseColl_dr03_b150_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e100.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e090.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e080.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e070.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e060.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b150_e050.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.125) {
          electronLooseColl_dr03_b125_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e050.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.1) {
          electronLooseColl_dr03_b100_e125.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e050.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.09) {
          electronLooseColl_dr03_b090_e125.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e050.push_back(electronLooseColl.at(iel));
        }
	if(LeptonRelIsoDR03 < 0.08) {
          electronLooseColl_dr03_b080_e125.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e050.push_back(electronLooseColl.at(iel));
        }
	
	if(LeptonRelIsoDR03 < 0.07) {
	  electronLooseColl_dr03_b070_e125.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e050.push_back(electronLooseColl.at(iel));
        }
	if(LeptonRelIsoDR03 < 0.05) {
	  electronLooseColl_dr03_b050_e125.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e060.push_back(electronLooseColl.at(iel));
	}
      }
      else{
	if(LeptonRelIsoDR03 < 0.125) {
	  electronLooseColl_dr03_b150_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b100_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b090_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b080_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b070_e125.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b050_e125.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.100) {
	  electronLooseColl_dr03_b150_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e100.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e100.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.090) {
	  electronLooseColl_dr03_b150_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e090.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e090.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.080) {
	  electronLooseColl_dr03_b150_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b125_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e080.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e080.push_back(electronLooseColl.at(iel));
	}
	if(LeptonRelIsoDR03 < 0.070) {
	  electronLooseColl_dr03_b150_e070.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b125_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e070.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e070.push_back(electronLooseColl.at(iel));
        }

	if(LeptonRelIsoDR03 < 0.060) {
	  electronLooseColl_dr03_b150_e060.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b125_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e060.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b050_e060.push_back(electronLooseColl.at(iel));
        }

	if(LeptonRelIsoDR03 < 0.050) {
	  electronLooseColl_dr03_b150_e050.push_back(electronLooseColl.at(iel));
	  electronLooseColl_dr03_b125_e050.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b100_e050.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b090_e050.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b080_e050.push_back(electronLooseColl.at(iel));
          electronLooseColl_dr03_b070_e050.push_back(electronLooseColl.at(iel));
        }
      }
      
      if(NPFiso < 0.25){
	if (fabs(electronLooseColl.at(iel).SCEta()) < 1.479 ){
	  if(LeptonRelIsoDR03 < 0.15) {
	    electronLooseColl_NPFisodr03_b150_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b150_e050.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.125) {
	    electronLooseColl_NPFisodr03_b125_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e050.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.1) {
	    electronLooseColl_NPFisodr03_b100_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e050.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.09) {
	    electronLooseColl_NPFisodr03_b090_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e050.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.08) {
	    electronLooseColl_NPFisodr03_b080_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e050.push_back(electronLooseColl.at(iel));
	  }

	  if(LeptonRelIsoDR03 < 0.07) {
	    electronLooseColl_NPFisodr03_b070_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e050.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.05) {
	    electronLooseColl_NPFisodr03_b050_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e060.push_back(electronLooseColl.at(iel));
	  }
	}
	else{
	  if(LeptonRelIsoDR03 < 0.125) {
	    electronLooseColl_NPFisodr03_b150_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e125.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e125.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.100) {
	    electronLooseColl_NPFisodr03_b150_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e100.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e100.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.090) {
	    electronLooseColl_NPFisodr03_b150_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e090.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e090.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.080) {
	    electronLooseColl_NPFisodr03_b150_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e080.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e080.push_back(electronLooseColl.at(iel));
	  }
	  if(LeptonRelIsoDR03 < 0.070) {
	    electronLooseColl_NPFisodr03_b150_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e070.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e070.push_back(electronLooseColl.at(iel));
	  }

	  if(LeptonRelIsoDR03 < 0.060) {
	    electronLooseColl_NPFisodr03_b150_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e060.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b050_e060.push_back(electronLooseColl.at(iel));
	  }

	  if(LeptonRelIsoDR03 < 0.050) {
	    electronLooseColl_NPFisodr03_b150_e050.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b125_e050.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b100_e050.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b090_e050.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b080_e050.push_back(electronLooseColl.at(iel));
	    electronLooseColl_NPFisodr03_b070_e050.push_back(electronLooseColl.at(iel));
	  }
	}
      }
    }
  }

	

  for(unsigned int iel = 0; iel < electronNoCutColl.size(); iel++){
    
    if(iel ==0){
      if(electronNoCutColl[iel].Pt() < 20.) continue;
    }
    else{
      if(electronNoCutColl[iel].Pt() < 15.) continue;
    }
    if(fabs(electronNoCutColl[iel].Eta()) > 2.4) continue;
    
    electronPtEtaCutColl.push_back(electronNoCutColl[iel]);
    
    double effective_area_eta_minimums    [7] = { 0.000, 1.000, 1.479, 2.000, 2.200, 2.300, 2.400 };
    double effective_area_eta_maximums    [7] = { 1.000, 1.479, 2.000, 2.200, 2.300, 2.400, 999.0 };
    double effective_areas_03             [7] = { 0.130, 0.140, 0.07, 0.09, 0.110, 0.110, 0.140 };
    double effective_area_03  = 0.0;
    
    for (int i = 0; i < 7; ++i ){
      double bin_minimum = effective_area_eta_minimums[i];
      double bin_maximum = effective_area_eta_maximums[i];
      if ( fabs(electronNoCutColl[iel].SCEta()) >= bin_minimum && fabs(electronNoCutColl[iel].SCEta()) < bin_maximum ) {
	effective_area_03 = effective_areas_03 [i];
      }
    }
   
    double egamma_pfiso_03 = electronNoCutColl[iel].PFChargedHadronIso03() + std::max ( electronNoCutColl[iel].PFPhotonIso03() + electronNoCutColl[iel].PFNeutralHadronIso03() - ( eventbase->GetEvent().JetRho() * effective_area_03 ), 0.0 );
    egamma_pfiso_03 /= electronNoCutColl[iel].Pt();
    
    
    double egamma_e  = electronNoCutColl.at(iel).CaloEnergy();
    double egamma_p  = electronNoCutColl.at(iel).CaloEnergy() / electronNoCutColl.at(iel).ESuperClusterOverP();
    double egamma_ep = fabs ( ( 1.0 / egamma_e ) - ( 1.0 / egamma_p ) );
    /// dEtaIn     cut
    if ( fabs(electronNoCutColl.at(iel).SCEta()) < 1.479 ){
      if(fabs(electronNoCutColl.at(iel).DeltaEta())   <= 0.004) {
	if(fabs(electronNoCutColl.at(iel).DeltaPhi())   <= 0.06){
	  if(electronNoCutColl.at(iel).SigmaIEtaIEta()    <= 0.01){
	    if(fabs(electronNoCutColl.at(iel).LeadVtxDistZ ()) < 0.1){
	      if(electronNoCutColl.at(iel).HoE            ()  <= 0.12){
		if( egamma_ep          <= 0.05){
		  if(electronNoCutColl.at(iel).ConvFitProb  ()    <=  1e-6){
		    if(electronNoCutColl.at(iel).MissingHits()    <=  1){
		      if(!electronNoCutColl.at(iel).HasMatchedConvPhot()){
			if(fabs(electronNoCutColl.at(iel).dxy())  <= 0.01){
			  if(egamma_pfiso_03    <=  0.1){
			    if(electronNoCutColl.at(iel).GsfCtfScPixChargeConsistency()){
			      electronMedium_chargeconst.push_back(electronNoCutColl.at(iel));
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    else {
      if(fabs(electronNoCutColl.at(iel).DeltaEta())   <= 0.007){
	if(fabs(electronNoCutColl.at(iel).DeltaPhi())   <= 0.03) {
	  if(electronNoCutColl.at(iel).SigmaIEtaIEta()    <= 0.03){
            if(fabs(electronNoCutColl.at(iel).LeadVtxDistZ ())< 0.1){
	      if(electronNoCutColl.at(iel).HoE            ()  <= 0.10){
		if( egamma_ep          <=0.05){
		  if(electronNoCutColl.at(iel).ConvFitProb  ()    <=  1e-6){
		    if(electronNoCutColl.at(iel).MissingHits()    <=  1){
		      if(!electronNoCutColl.at(iel).HasMatchedConvPhot()){
			if(fabs(electronNoCutColl.at(iel).dxy())  <= 0.01){
			  if(egamma_pfiso_03    <=  0.1){
			    if(electronNoCutColl.at(iel).GsfCtfScPixChargeConsistency()){
                              electronMedium_chargeconst.push_back(electronNoCutColl.at(iel));
                            }
			  }
			}
                      }
		    }
		  }
		}
	      } 
            }
          }
	}
      }
    }
    /// TIGHT
    
    if ( fabs(electronNoCutColl.at(iel).SCEta()) < 1.479 ){
      if(fabs(electronNoCutColl.at(iel).DeltaEta())   <= 0.004) {
        if(fabs(electronNoCutColl.at(iel).DeltaPhi())   <= 0.03){
          if(electronNoCutColl.at(iel).SigmaIEtaIEta()    <= 0.01){
            if(fabs(electronNoCutColl.at(iel).LeadVtxDistZ ())< 0.1){
	      if(electronNoCutColl.at(iel).HoE            ()  <= 0.12){
		if( egamma_ep          <= 0.05){
		  electronTight.push_back(electronNoCutColl.at(iel));
		  if(electronNoCutColl[iel].ConvFitProb  ()    <=  1e-6){
		    if(electronNoCutColl[iel].MissingHits()    <=  0){
		      if(!electronNoCutColl.at(iel).HasMatchedConvPhot()){
			electronTight_convveto.push_back(electronNoCutColl.at(iel));
			
			if(fabs(electronNoCutColl.at(iel).dxy())  <= 0.1){
			  electronTight_d0veto.push_back(electronNoCutColl.at(iel));
			  if(egamma_pfiso_03    <=  0.1){
			    electronTight_reliso.push_back(electronNoCutColl.at(iel));
			    if(electronNoCutColl.at(iel).GsfCtfScPixChargeConsistency()){
                              electronTight_chargeconst.push_back(electronNoCutColl.at(iel));
                            }
			  }
			}
		      }
		    }
		  }
		}
              }
            }
          }
        }
      }
    }
    else {
      if(fabs(electronNoCutColl.at(iel).DeltaEta())   <= 0.005){
        if(fabs(electronNoCutColl.at(iel).DeltaPhi())   <= 0.02) {
          if(electronNoCutColl.at(iel).SigmaIEtaIEta()    <= 0.03){
            if(fabs(electronNoCutColl.at(iel).LeadVtxDistZ ())< 0.1){
	      if(electronNoCutColl.at(iel).HoE            ()  <= 0.10){
		if( egamma_ep          <=0.05){
		  electronTight.push_back(electronNoCutColl.at(iel));
		  if(electronNoCutColl[iel].ConvFitProb  ()    <=  1e-6){
		    if(electronNoCutColl[iel].MissingHits()    <=  0){
		      if(!electronNoCutColl.at(iel).HasMatchedConvPhot()){
			electronTight_convveto.push_back(electronNoCutColl.at(iel));
			if(fabs(electronNoCutColl.at(iel).dxy())  <= 0.01){
			  electronTight_d0veto.push_back(electronNoCutColl.at(iel));
			  if(egamma_pfiso_03    <=  0.1){
			    electronTight_reliso.push_back(electronNoCutColl.at(iel));
			    if(electronNoCutColl.at(iel).GsfCtfScPixChargeConsistency()){
                              electronTight_chargeconst.push_back(electronNoCutColl.at(iel));
                            }
			  }
			}
		      }
		    }
		  }
		}
	      }
            }
          }
        }
      }
    }
  }
  
  /// MUONS
  std::vector<snu::KMuon> muonVetoColl;
  eventbase->GetMuonSel()->HNVetoMuonSelection(muonVetoColl);
  
  std::vector<snu::KMuon> muonTightColl;
  eventbase->GetMuonSel()->HNTightMuonSelection(muonTightColl,false);
  
  std::vector<snu::KMuon> muonNoCutColl;
  eventbase->GetMuonSel()->Selection(muonNoCutColl);
  
  /// JETS
  std::vector<snu::KJet> jetColl_lepveto;
  std::vector<snu::KJet> jetColl_reallepveto;  /// -> does not veto jets IF the electron Fraction is < 20%
  eventbase->GetJetSel()->SetID(BaseSelection::PFJET_LOOSE);
  eventbase->GetJetSel()->SetPt(20.);
  eventbase->GetJetSel()->SetEta(2.5);
  eventbase->GetJetSel()->JetHNSelection(jetColl_lepveto, muonVetoColl, electronLooseColl);
 
  /// makes full set of plots for el/mu/jets/met with no cuts applied on objects 
  FillCLHist(sighist, "NoCut", eventbase->GetEvent(), muonNoCutColl,electronNoCutColl,jetColl, weight);
  
  

  //// Check efficiency of Selecting two same sign electrons (efficiency of medium/tight and returndown of tight cuts) 
  if(SameCharge(electronNoCutColl))   FillCutFlow("SS_NoCut",weight);
  if(SameCharge(electronPtEtaCutColl)) FillCutFlow("SS_PtEta",weight);
  if(SameCharge(electronTight))   FillCutFlow("SS_Tight",weight);
  if(SameCharge(electronTight_convveto))  FillCutFlow("SS_Tight_convveto",weight);
  if(SameCharge(electronTight_d0veto)) FillCutFlow("SS_Tight_d0veto",weight);
  if(SameCharge(electronTight_reliso)) FillCutFlow("SS_Tight_reliso", weight);  
  if(SameCharge(electronTight_chargeconst)) FillCutFlow("SS_Tight_chargeconst", weight);
  if(SameCharge(electronMedium_chargeconst))  FillCutFlow("SS_Medium",weight);
  if(SameCharge(electronAnalysisColl)) FillCutFlow("SS_anal_el", weight);
  
  
  /// Make plot of jet energy fraction for close and away jets
  CheckJetsCloseToLeptons(electronTight_chargeconst, jetColl, "Tight");
  
  
  bool jetclosetoel= false;
  for(unsigned int ijet =0; ijet < jetColl_reallepveto.size(); ijet++){
    for(unsigned int iel=0; iel <  electronTight_chargeconst.size(); iel++){
      if(jetColl_reallepveto.at(ijet).DeltaR(electronTight_chargeconst.at(iel)) < 0.4) {
	jetclosetoel=true;
      }
    }
  }
  
  if(!jetclosetoel){
    if(SameCharge(electronTight_chargeconst)) FillCutFlow("SS_Tight_noclosejet", weight);
  }  
  
  
  //Now making signal region plots 
  
  /// So far events pass event cuts and  trigger
  
  if ((electronVetoColl.size() + muonVetoColl.size()) >2) return;
  

  if(!SameCharge(electronAnalysisColl))FillHist("SScandidate_check", 0.,1., 0.,2.,2);
  if(!SameCharge(electronLooseColl4)) FillHist("SScandidate_check", 1.,1, 0.,2.,2);
  

  
  if(!k_running_nonprompt){
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"Signal_Mediumlooseiso_d0", weight)) FillHist("IDREF",0.  , weight, 0.,1.,1);
    if(CheckSignalRegion(electronMedium_chargeconst, jetColl_lepveto,"Signal_Mediumlooseiso_d0", weight)) FillHist("IDcutflow",0.  , weight, 0.,2.,2);
    if(CheckSignalRegion(electronTight_chargeconst, jetColl_lepveto,"Signal_Tightlooseiso_d0", weight))   FillHist("IDcutflow",1.  , weight, 0.,2.,2);
  }
  else{
    float ee_weight_medium = Get_DataDrivenWeight_EE(electronLooseColl_medium, eventbase->GetEvent().JetRho(), 0.01, 0.1, 0.1,  false, "medium");
    float ee_weight_tight = Get_DataDrivenWeight_EE(electronLooseColl,  eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.1,  true, "tight");
    
    if(CheckSignalRegion(electronLooseColl_medium, jetColl_lepveto,"Signal_Mediumlooseiso_d0", weight)) FillHist("IDcutflow",0.  , weight*ee_weight_medium, 0.,2.,2);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"Signal_Tightlooseiso_d0", weight))   FillHist("IDcutflow",1.  , weight*ee_weight_tight, 0.,2.,2);
    
    if(CheckSignalRegion(electronLooseColl_medium, jetColl_lepveto,"Signal_Mediumlooseiso_d0", weight)) FillHist("IDcutflow_samefakerate",0.  , weight*ee_weight_tight, 0.,2.,2);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"Signal_Tightlooseiso_d0", weight))   FillHist("IDcutflow_samefakerate",1.  , weight*ee_weight_tight, 0.,2.,2);

  }
  
  if(!k_running_nonprompt){
    if(CheckSignalRegion(electronLooseColl_iseref,jetColl_lepveto,"", weight))   FillHist("ISOREF", 0., weight, 0.,1.,1);
    if(CheckSignalRegion(electronLooseColl_dr03_150,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      0.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_125,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      1.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_100,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      2.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_090,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      3.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_080,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      4.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_070,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      5.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_060,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      6.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr03_050,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      7.  , weight, 0.,16.,16);
    
    if(CheckSignalRegion(electronLooseColl_dr04_150,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      8.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_125,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      9.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_100,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      10.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_090,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      11.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_080,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      12.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_070,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      13.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_060,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      14.  , weight, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl_dr04_050,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      15.  , weight, 0.,16.,16);
  }
  else{
    float ee_weight_dr03_150 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.15, true ,"iso_b150_e150");
    float ee_weight_dr03_125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.125, true ,"iso_b125_e125");
    float ee_weight_dr03_100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.1, true ,"iso_b100_e100");
    float ee_weight_dr03_090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.09, true ,"iso_b090_e090");
    float ee_weight_dr03_080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.08, true ,"iso_b080_e080");
    float ee_weight_dr03_070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.07, true ,"iso_b070_e070");
    float ee_weight_dr03_060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.06, 0.06, true ,"iso_b060_e060");
    float ee_weight_dr03_050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.05, true ,"iso_b050_e050");
    
    float ee_weight_dr04_150 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.15, true, "iso_b150_e150");
    float ee_weight_dr04_125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.125, true, "iso_b125_e125");
    float ee_weight_dr04_100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.1, true, "iso_b100_e100");
    float ee_weight_dr04_090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.09, true, "iso_b090_e090");
    float ee_weight_dr04_080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.08, true, "iso_b080_e080");
    float ee_weight_dr04_070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.07, true, "iso_b070_e070");
    float ee_weight_dr04_060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.06, 0.06, true, "iso_b060_e060");
    float ee_weight_dr04_050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.05, true, "iso_b050_e050");
    
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      0.  , weight*ee_weight_dr03_150, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      1.  , weight*ee_weight_dr03_125, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      2.  , weight*ee_weight_dr03_100, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      3.  , weight*ee_weight_dr03_090, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      4.  , weight*ee_weight_dr03_080, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      5.  , weight*ee_weight_dr03_070, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      6.  , weight*ee_weight_dr03_060, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      7.  , weight*ee_weight_dr03_050, 0.,16.,16);

    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      8.  , weight*ee_weight_dr04_150, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      9.  , weight*ee_weight_dr04_125, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      10.  , weight*ee_weight_dr04_100, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      11.  , weight*ee_weight_dr04_090, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      12.  , weight*ee_weight_dr04_080, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      13.  , weight*ee_weight_dr04_070, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      14.  , weight*ee_weight_dr04_060, 0.,16.,16);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISOcutflow",      15.  , weight*ee_weight_dr04_050, 0.,16.,16);

  }

  if(k_running_nonprompt){
    float ee_weight_dr03_b150_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.125, true ,"iso_b150_e125");
    float ee_weight_dr03_b150_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.100, true ,"iso_b150_e100");
    float ee_weight_dr03_b150_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.090, true ,"iso_b150_e090");
    float ee_weight_dr03_b150_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.080, true ,"iso_b150_e080");
    float ee_weight_dr03_b150_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.070, true ,"iso_b150_e070");
    float ee_weight_dr03_b150_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.060, true ,"iso_b150_e060");
    float ee_weight_dr03_b150_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.050, true ,"iso_b150_e050");
    float ee_weight_dr03_b125_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.100, true ,"iso_b125_e100");
    float ee_weight_dr03_b125_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.090, true ,"iso_b125_e090");
    float ee_weight_dr03_b125_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.080, true ,"iso_b125_e080");
    float ee_weight_dr03_b125_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.070, true ,"iso_b125_e070");
    float ee_weight_dr03_b125_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.060, true ,"iso_b125_e060");
    float ee_weight_dr03_b125_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.050, true ,"iso_b125_e050");
    float ee_weight_dr03_b100_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.125, true ,"iso_b100_e125");
    float ee_weight_dr03_b100_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.090, true ,"iso_b100_e090");
    float ee_weight_dr03_b100_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.080, true ,"iso_b100_e080");
    float ee_weight_dr03_b100_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.070, true ,"iso_b100_e070");
    float ee_weight_dr03_b100_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.060, true ,"iso_b100_e060");
    float ee_weight_dr03_b100_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.050, true ,"iso_b100_e050");
    float ee_weight_dr03_b090_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.125, true ,"iso_b090_e125");
    float ee_weight_dr03_b090_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.100, true ,"iso_b090_e100");
    float ee_weight_dr03_b090_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.080, true ,"iso_b090_e080");
    float ee_weight_dr03_b090_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.070, true ,"iso_b090_e070");
    float ee_weight_dr03_b090_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.060, true ,"iso_b090_e060");
    float ee_weight_dr03_b090_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.050, true ,"iso_b090_e050");
    float ee_weight_dr03_b080_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.125, true ,"iso_b080_e125");
    float ee_weight_dr03_b080_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.100, true ,"iso_b080_e100");
    float ee_weight_dr03_b080_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.090, true ,"iso_b080_e090");
    float ee_weight_dr03_b080_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.070, true ,"iso_b080_e070");
    float ee_weight_dr03_b080_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.060, true ,"iso_b080_e060");
    float ee_weight_dr03_b080_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.050, true ,"iso_b080_e050");
    float ee_weight_dr03_b070_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.125, true ,"iso_b070_e125");
    float ee_weight_dr03_b070_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.100, true ,"iso_b070_e100");
    float ee_weight_dr03_b070_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.090, true ,"iso_b070_e090");
    float ee_weight_dr03_b070_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.080, true ,"iso_b070_e080");
    float ee_weight_dr03_b070_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.060, true ,"iso_b070_e060");
    float ee_weight_dr03_b070_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.050, true ,"iso_b070_e050");
    float ee_weight_dr03_b050_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.125, true ,"iso_b050_e125");
    float ee_weight_dr03_b050_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.100, true ,"iso_b050_e100");
    float ee_weight_dr03_b050_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.090, true ,"iso_b050_e090");
    float ee_weight_dr03_b050_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.080, true ,"iso_b050_e080");
    float ee_weight_dr03_b050_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.070, true ,"iso_b050_e070");
    float ee_weight_dr03_b050_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.060, true ,"iso_b050_e050");

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,0., weight*ee_weight_dr03_b150_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,1., weight*ee_weight_dr03_b150_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,2., weight*ee_weight_dr03_b150_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,3., weight*ee_weight_dr03_b150_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,4., weight*ee_weight_dr03_b150_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,5., weight*ee_weight_dr03_b150_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,6., weight*ee_weight_dr03_b150_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,7., weight*ee_weight_dr03_b125_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,8., weight*ee_weight_dr03_b125_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,9., weight*ee_weight_dr03_b125_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,10, weight*ee_weight_dr03_b125_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,11., weight*ee_weight_dr03_b125_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,12., weight*ee_weight_dr03_b125_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,13., weight*ee_weight_dr03_b100_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,14., weight*ee_weight_dr03_b100_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,15., weight*ee_weight_dr03_b100_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,16., weight*ee_weight_dr03_b100_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,17., weight*ee_weight_dr03_b100_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,18., weight*ee_weight_dr03_b100_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,19., weight*ee_weight_dr03_b090_e125, 0.,49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,20., weight*ee_weight_dr03_b090_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,21., weight*ee_weight_dr03_b090_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,22., weight*ee_weight_dr03_b090_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))  FillHist("ISO_EEEB_cutflow" ,23., weight*ee_weight_dr03_b090_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))  FillHist("ISO_EEEB_cutflow" ,24., weight*ee_weight_dr03_b090_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,25., weight*ee_weight_dr03_b080_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,26., weight*ee_weight_dr03_b080_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,27., weight*ee_weight_dr03_b080_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,28., weight*ee_weight_dr03_b080_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,29., weight*ee_weight_dr03_b080_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,30., weight*ee_weight_dr03_b080_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,31., weight*ee_weight_dr03_b070_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,32., weight*ee_weight_dr03_b070_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,33., weight*ee_weight_dr03_b070_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,34., weight*ee_weight_dr03_b070_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,35., weight*ee_weight_dr03_b070_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,36., weight*ee_weight_dr03_b070_e050, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,37., weight*ee_weight_dr03_b050_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,38., weight*ee_weight_dr03_b050_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,39., weight*ee_weight_dr03_b050_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,40., weight*ee_weight_dr03_b050_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,41., weight*ee_weight_dr03_b050_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,42., weight*ee_weight_dr03_b050_e060, 0., 49.,49);
  }
  else{
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,0., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,1., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,2., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,3., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,4., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,5., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b150_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,6., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,7., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,8., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,9., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,10, weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,11., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b125_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,12., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,13., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,14., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,15., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,16., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,17., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b100_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,18., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,19., weight, 0.,49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,20., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,21., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,22., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,23., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b090_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,24., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,25., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,26., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,27., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,28., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,29., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b080_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,30., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,31., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,32., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,33., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,34., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,35., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b070_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,36., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,37., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,38., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,39., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,40., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,41., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_dr03_b050_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_EEEB_cutflow" ,42., weight, 0., 49.,49);
  }

  /// + NPiso
  if(!k_running_nonprompt){
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,0., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,1., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,2., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,3., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,4., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,5., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b150_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,6., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,7., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,8., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,9., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,10., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,11., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b125_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,12., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,13., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,14., weight, 0.,49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,15., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,16., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,17., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b100_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,18., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,19., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,20., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,21., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,22., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,23., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b090_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,24., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,25., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,26., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,27., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,28., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,29., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b080_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,30., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,31., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,32., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,33., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,34., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,35., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b070_e050 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,36., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e125 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,37., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e100 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,38., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e090 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,39., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e080 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,40., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e070 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,41., weight, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl_NPFisodr03_b050_e060 ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,42., weight, 0., 49.,49);

  }
  else{

    float ee_weight_dr03_b150_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.125, true, "NPFiso_b150_e125");
    float ee_weight_dr03_b150_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.100, true, "NPFiso_b150_e100");
    float ee_weight_dr03_b150_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.090, true, "NPFiso_b150_e090");
    float ee_weight_dr03_b150_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.080, true, "NPFiso_b150_e080");
    float ee_weight_dr03_b150_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.070, true, "NPFiso_b150_e070");
    float ee_weight_dr03_b150_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.060, true, "NPFiso_b150_e060");
    float ee_weight_dr03_b150_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.15, 0.050, true, "NPFiso_b150_e050");

    float ee_weight_dr03_b125_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.100, true, "NPFiso_b125_e100");
    float ee_weight_dr03_b125_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.090, true, "NPFiso_b125_e090");
    float ee_weight_dr03_b125_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.080, true, "NPFiso_b125_e080");
    float ee_weight_dr03_b125_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.070, true, "NPFiso_b125_e070");
    float ee_weight_dr03_b125_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.060, true, "NPFiso_b125_e060");
    float ee_weight_dr03_b125_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.125, 0.050, true, "NPFiso_b125_e050");

    float ee_weight_dr03_b100_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.125, true, "NPFiso_b100_e125");
    float ee_weight_dr03_b100_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.090, true, "NPFiso_b100_e090");
    float ee_weight_dr03_b100_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.080, true, "NPFiso_b100_e080");
    float ee_weight_dr03_b100_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.070, true, "NPFiso_b100_e070");
    float ee_weight_dr03_b100_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.060, true, "NPFiso_b100_e060");
    float ee_weight_dr03_b100_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.1, 0.050, true, "NPFiso_b100_e050");

    float ee_weight_dr03_b090_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.125, true, "NPFiso_b090_e125");
    float ee_weight_dr03_b090_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.100, true, "NPFiso_b090_e100");
    float ee_weight_dr03_b090_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.080, true, "NPFiso_b090_e080");
    float ee_weight_dr03_b090_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.070, true, "NPFiso_b090_e070");
    float ee_weight_dr03_b090_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.060, true, "NPFiso_b090_e060");
    float ee_weight_dr03_b090_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.09, 0.050, true, "NPFiso_b090_e050");
    
    float ee_weight_dr03_b080_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.125, true, "NPFiso_b080_e125");
    float ee_weight_dr03_b080_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.100, true, "NPFiso_b080_e100");
    float ee_weight_dr03_b080_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.090, true, "NPFiso_b080_e090");
    float ee_weight_dr03_b080_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.070, true, "NPFiso_b080_e070");
    float ee_weight_dr03_b080_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.060, true, "NPFiso_b080_e060");
    float ee_weight_dr03_b080_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.08, 0.050, true, "NPFiso_b080_e050");

    float ee_weight_dr03_b070_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.125, true, "NPFiso_b070_e125");
    float ee_weight_dr03_b070_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.100, true, "NPFiso_b070_e100");
    float ee_weight_dr03_b070_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.090, true, "NPFiso_b070_e090");
    float ee_weight_dr03_b070_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.080, true, "NPFiso_b070_e080");
    float ee_weight_dr03_b070_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.060, true, "NPFiso_b070_e060");
    float ee_weight_dr03_b070_e050 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.07, 0.050, true, "NPFiso_b070_e050");

    float ee_weight_dr03_b050_e125 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.125, true, "NPFiso_b050_e125");
    float ee_weight_dr03_b050_e100 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.100, true, "NPFiso_b050_e100");
    float ee_weight_dr03_b050_e090 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.090, true, "NPFiso_b050_e090");
    float ee_weight_dr03_b050_e080 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.080, true, "NPFiso_b050_e080");
    float ee_weight_dr03_b050_e070 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.070, true, "NPFiso_b050_e070");
    float ee_weight_dr03_b050_e060 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.01, 0.05, 0.060, true, "NPFiso_b050_e060");

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,0., weight*ee_weight_dr03_b150_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,1., weight*ee_weight_dr03_b150_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,2., weight*ee_weight_dr03_b150_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,3., weight*ee_weight_dr03_b150_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,4., weight*ee_weight_dr03_b150_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,5., weight*ee_weight_dr03_b150_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,6., weight*ee_weight_dr03_b150_e050, 0., 49.,49);

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,7., weight*ee_weight_dr03_b125_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,8., weight*ee_weight_dr03_b125_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,9., weight*ee_weight_dr03_b125_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,11., weight*ee_weight_dr03_b125_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,10., weight*ee_weight_dr03_b125_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,12., weight*ee_weight_dr03_b125_e050, 0., 49.,49);
    
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,13., weight*ee_weight_dr03_b100_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,14., weight*ee_weight_dr03_b100_e090, 0.,49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,15., weight*ee_weight_dr03_b100_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,16., weight*ee_weight_dr03_b100_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,17., weight*ee_weight_dr03_b100_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,18., weight*ee_weight_dr03_b100_e050, 0., 49.,49);

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,19., weight*ee_weight_dr03_b090_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,20., weight*ee_weight_dr03_b090_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,21., weight*ee_weight_dr03_b090_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,22., weight*ee_weight_dr03_b090_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,23., weight*ee_weight_dr03_b090_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,24., weight*ee_weight_dr03_b090_e050, 0., 49.,49);

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,25., weight*ee_weight_dr03_b080_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,26., weight*ee_weight_dr03_b080_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,27., weight*ee_weight_dr03_b080_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,28., weight*ee_weight_dr03_b080_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,29., weight*ee_weight_dr03_b080_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,30., weight*ee_weight_dr03_b080_e050, 0., 49.,49);

    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,31., weight*ee_weight_dr03_b070_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,32., weight*ee_weight_dr03_b070_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,33., weight*ee_weight_dr03_b070_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,34., weight*ee_weight_dr03_b070_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,35., weight*ee_weight_dr03_b070_e060, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,36., weight*ee_weight_dr03_b070_e050, 0., 49.,49);

    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,37., weight*ee_weight_dr03_b050_e125, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,38., weight*ee_weight_dr03_b050_e100, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,39., weight*ee_weight_dr03_b050_e090, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,40., weight*ee_weight_dr03_b050_e080, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,41., weight*ee_weight_dr03_b050_e070, 0., 49.,49);
    if(CheckSignalRegion(electronLooseColl ,jetColl_lepveto,"", weight))   FillHist("ISO_NPFISO_EEEB_cutflow" ,42., weight*ee_weight_dr03_b050_e060, 0., 49.,49);

    
  }
    

  if(CheckSignalRegion(electronLooseColl_nodxy, jetColl_lepveto,"pogiso_d0_nocut", weight))FillHist("d0cutflow",0.  , weight, 0.,7.,7);
  if(!k_running_nonprompt){
    if(CheckSignalRegion(electronLooseColl_dxy_05, jetColl_lepveto,"pogiso_d0_05", weight))FillHist("d0cutflow",1.  , weight, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl_dxy_10, jetColl_lepveto,"pogiso_d0_10", weight))FillHist("d0cutflow",2.  , weight, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl_dxy_15, jetColl_lepveto,"pogiso_d0_15", weight))FillHist("d0cutflow",3.  , weight, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl_dxy_20, jetColl_lepveto,"pogiso_d0_20", weight))FillHist("d0cutflow",4.  , weight, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl_dxy_25, jetColl_lepveto,"pogiso_d0_25", weight))FillHist("d0cutflow",5.  , weight, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl_dxy_30, jetColl_lepveto,"pogiso_d0_30", weight))FillHist("d0cutflow",6.  , weight, 0.,7.,7);
  }
  else{
    
    float ee_weight_05 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(), 0.005, 0.1, 0.1, true ,"dxy05");
    float ee_weight_10 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.010, 0.1, 0.1, true ,"dxy10");
    float ee_weight_15 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(), 0.015, 0.1, 0.1, true  ,"dxy15");
    float ee_weight_20 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.020, 0.1, 0.1, true ,"dxy20");
    float ee_weight_25 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.025, 0.1, 0.1, true ,"dxy25");
    float ee_weight_30 = Get_DataDrivenWeight_EE(electronLooseColl, eventbase->GetEvent().JetRho(),  0.030, 0.1, 0.1, true ,"dxy30");

       
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_05", weight*ee_weight_05))FillHist("d0cutflow",1.  , weight*ee_weight_05, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_10", weight*ee_weight_10))FillHist("d0cutflow",2.  , weight*ee_weight_10, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_15", weight*ee_weight_15))FillHist("d0cutflow",3.  , weight*ee_weight_15, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_20", weight*ee_weight_20))FillHist("d0cutflow",4.  , weight*ee_weight_20, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_25", weight*ee_weight_25))FillHist("d0cutflow",5.  , weight*ee_weight_25, 0.,7.,7);
    if(CheckSignalRegion(electronLooseColl, jetColl_lepveto,"pogiso_d0_30", weight*ee_weight_30))FillHist("d0cutflow",6.  , weight*ee_weight_30, 0.,7.,7);
    
  }
  
/// Now check signal regions
  
  if(electronAnalysisColl.size() != 2 ) return;
  
  /// Apply electron SFs for tight id
  if(!isData){
    for(std::vector<snu::KElectron>::iterator it = electronAnalysisColl.begin(); it != electronAnalysisColl.end(); it++){
      weight *=  ElectronScaleFactor(it->Eta(), it->Pt(), true);
    }
  }
  
  
  /// Look at inclusive same-sign dielectron events
  if(SameCharge(electronAnalysisColl))   FillCLHist(sighist, "SSee", eventbase->GetEvent(), muonTightColl,electronAnalysisColl,jetColl_lepveto, weight);
  
  if ((electronVetoColl.size() + muonVetoColl.size()) >2) return;  
  
  /// before third lepton veto no fake estimate can be done.
  if(k_running_nonprompt){
    float ee_weight = Get_DataDrivenWeight_EE(electronAnalysisColl, eventbase->GetEvent().JetRho());
    weight*= ee_weight;
  }
  
  if(SameCharge(electronAnalysisColl))   FillCLHist(sighist, "SSee", eventbase->GetEvent(), muonTightColl,electronAnalysisColl,jetColl_lepveto, weight);
  
  
  
  if(electronAnalysisColl.at(0).Pt() < 20.) return;
  if(electronAnalysisColl.at(1).Pt() < 15.) return;
  
  
  if(Zcandidate(electronAnalysisColl, 15., false))  {
    if(electronAnalysisColl.at(0).Charge() != electronAnalysisColl.at(1).Charge()) {
      float cf1=  CFRate(electronAnalysisColl.at(0));
      float cf2=  CFRate(electronAnalysisColl.at(1));
      
      float cfweight= weight;
      cfweight *= ((cf1/(1.-cf1)) + (cf2/(1.-cf2)));
      
      m_os_Z += cfweight;
      m_os_Z_nw +=1;
      
      FillHist("Z_os_mc_1pt",electronAnalysisColl.at(0).Pt()  , cfweight, 0.,200.,20);
      FillHist("Z_os_mc_1eta",electronAnalysisColl.at(0).Eta()  , cfweight, -2.5,2.5,10);
      FillHist("Z_os_mc_2pt",electronAnalysisColl.at(1).Pt()  , cfweight, 0.,200.,20);
      FillHist("Z_os_mc_2eta",electronAnalysisColl.at(1).Eta()  , cfweight, -2.5,2.5,10);
      
    }
    
    else{
      m_ss_Z += weight;
      m_ss_Z_nw += 1.;
      FillHist("Z_ss_mc_1pt",electronAnalysisColl.at(0).Pt()  , weight, 0.,200.,20);
      FillHist("Z_ss_mc_2pt",electronAnalysisColl.at(1).Pt()  , weight, 0.,200.,20);

      FillHist("Z_ss_mc_1eta",electronAnalysisColl.at(0).Eta()  , weight, -2.5,2.5,10);
      FillHist("Z_ss_mc_2eta",electronAnalysisColl.at(1).Eta()  , weight, -2.5,2.5,10);
    }
  }
  

  if(k_running_chargeflip) {
    if(electronAnalysisColl.at(0).Charge() != electronAnalysisColl.at(1).Charge()) {
      float cf1=  CFRate(electronAnalysisColl.at(0));
      float cf2=  CFRate(electronAnalysisColl.at(1));
      
      weight *=  0.5* ((cf1/(1.-cf1)) + (cf2/(1.-cf2)));
    }
    else return;
  }
  else {
    if(electronAnalysisColl.at(0).Charge() != electronAnalysisColl.at(1).Charge()) FillCLHist(sighist, "OSee", eventbase->GetEvent(), muonTightColl,electronAnalysisColl,jetColl_lepveto, weight);
    if(electronAnalysisColl.at(0).Charge() != electronAnalysisColl.at(1).Charge()) return;
  }
  
  
  return;
  for(unsigned int ijet=0; ijet < jetColl_lepveto.size(); ijet++){
    
    FillHist("jet_NeutralEMEnergyFraction",jetColl_lepveto.at(ijet).NeutralEMEnergyFraction()  , weight, 0.,1.,20);
    FillHist("jet_NeutralHadEnergyFraction",jetColl_lepveto.at(ijet).NeutralHadEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_ChargedEMEnergyFraction",jetColl_lepveto.at(ijet).ChargedEMEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_ChargedHadEnergyFraction",jetColl_lepveto.at(ijet).ChargedHadEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_HFEMEnergyFraction",jetColl_lepveto.at(ijet).HFEMEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_HFHadEnergyFraction",jetColl_lepveto.at(ijet).HFHadEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_ChargedMuEnergyFraction",jetColl_lepveto.at(ijet).ChargedMuEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_MuonEnergyFraction",jetColl_lepveto.at(ijet).MuonEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_ElectronEnergyFraction",jetColl_lepveto.at(ijet).ElectronEnergyFraction() , weight, 0.,1.,20);
    FillHist("jet_PhotonEnergyFraction",jetColl_lepveto.at(ijet).PhotonEnergyFraction() , weight, 0.,1.,20);

  }

  
  return;
  
  
}// End of execute event loop

bool HNElectronOptimisation::CheckSignalRegion(  std::vector<snu::KElectron> electrons, std::vector<snu::KJet> jets, TString name, float w){

  if(name.Contains("testetst")) w=0.;
  if(electrons.size() != 2 ) return false ;
  if(electrons.at(0).Pt() < 20.) return false;
  if(electrons.at(1).Pt() < 15.) return false;
  if(!SameCharge(electrons)) return false;
  if(jets.size() < 2) return false;
  snu::KParticle ee = electrons.at(0) + electrons.at(1);
  if(ee.M()  < 10.) return false;

  snu::KParticle jj = jets.at(0) + jets.at(1) ;
  if(jj.M() > 120.) return false;
  if(jj.M() < 40.) return false;

  if(electrons.at(0).DeltaR(electrons.at(1)) > 3.5) return false;
  if(eventbase->GetEvent().PFSumET() < 200.) return false;;

  if((ee.M() > 80.) && (ee.M() < 100.) ) return false;
  if(eventbase->GetEvent().PFMET() > 35.) return false;

  int nbjet=0;
  for(unsigned int ij=0; ij <jets.size(); ij++){
    if(jets.at(ij).CombinedSecVertexBtag() > 0.679) nbjet++;
  }
  if(nbjet > 0) return false;
  
  return true;

}


bool HNElectronOptimisation::LowMassCheckSignalRegion(  std::vector<snu::KElectron> electrons, std::vector<snu::KJet> jets){
  if(electrons.size() != 2 ) return false;
  if(electrons.at(0).Pt() < 20.) return false;
  if(electrons.at(1).Pt() < 15.) return false;
  if(!SameCharge(electrons)) return false;
  if(jets.size() < 2) return false;
  snu::KParticle ee = electrons.at(0) + electrons.at(1);
  if(ee.M()  < 10.) return false;

  snu::KParticle eejj = electrons.at(0) + electrons.at(1)+ jets.at(0) + jets.at(1) ; 
  if(eejj.M()  > 200.) return false;
    
  snu::KParticle jj = jets.at(0) + jets.at(1) ;
  if(jj.M() > 120.) return false;
  
  if(ee.M() > 80.) return false;
  if(eventbase->GetEvent().PFMET() > 35.) return false;

  int nbjet=0;
  for(unsigned int ij=0; ij <jets.size(); ij++){
    if(jets.at(ij).CombinedSecVertexBtag() > 0.679) nbjet++;
  }
  if(nbjet > 0) return false;

  return true;
  
}

bool HNElectronOptimisation::MidMassCheckSignalRegion(  std::vector<snu::KElectron> electrons, std::vector<snu::KJet> jets){
  if(electrons.size() != 2 ) return false;
  if(electrons.at(0).Pt() < 20.) return false;
  if(electrons.at(1).Pt() < 15.) return false;
  if(!SameCharge(electrons)) return false;
  if(jets.size() < 2) return false;
  snu::KParticle ee = electrons.at(0) + electrons.at(1);
  if(ee.M()  < 10.) return false;

  snu::KParticle eejj = electrons.at(0) + electrons.at(1)+ jets.at(0) + jets.at(1) ;
  if(eejj.M()  > 200.) return false;

  snu::KParticle jj = jets.at(0) + jets.at(1) ;
  if(jj.M() > 120.) return false;

  if(ee.M() > 80.) return false;
  if(eventbase->GetEvent().PFMET() > 35.) return false;

  int nbjet=0;
  for(unsigned int ij=0; ij <jets.size(); ij++){
    if(jets.at(ij).CombinedSecVertexBtag() > 0.679) nbjet++;
  }
  if(nbjet > 0) return false;

  return true;
}


bool HNElectronOptimisation::HighMassCheckSignalRegion(  std::vector<snu::KElectron> electrons, std::vector<snu::KJet> jets){
  if(electrons.size() != 2 ) return false;
  if(electrons.at(0).Pt() < 20.) return false;
  if(electrons.at(1).Pt() < 15.) return false;
  if(!SameCharge(electrons)) return false;
  if(jets.size() < 2) return false;
  snu::KParticle ee = electrons.at(0) + electrons.at(1);
  if(ee.M()  < 10.) return false;

  snu::KParticle eejj = electrons.at(0) + electrons.at(1)+ jets.at(0) + jets.at(1) ;
  if(eejj.M()  > 200.) return false;

  snu::KParticle jj = jets.at(0) + jets.at(1) ;
  if(jj.M() > 120.) return false;
  if(jj.M() > 40.) return false;

  if(ee.M() > 80.) return false;
  if(eventbase->GetEvent().PFMET() > 35.) return false;

  int nbjet=0;
  for(unsigned int ij=0; ij <jets.size(); ij++){
    if(jets.at(ij).CombinedSecVertexBtag() > 0.679) nbjet++;
  }
  if(nbjet > 0) return false;

  return true;
}



void HNElectronOptimisation::CheckJetsCloseToLeptons(std::vector<snu::KElectron> electrons, std::vector<snu::KJet> jets,  TString name){
  if(electrons.size() == 2) {
    if(SameCharge(electrons)) {
      snu::KParticle looseee = electrons.at(0) + electrons.at(1);
      if(fabs(looseee.M() - 90.) > 20. ){


	for(unsigned int ijet=0; ijet < jets.size(); ijet++){
	  for(unsigned int iel=0; iel < electrons.size(); iel++){
	    if( electrons[iel].DeltaR(jets.at(ijet)) < 0.4){
	      /// Jet usually vetoe
	      FillHist(("closejet" + name + "_NeutralEMEnergyFraction").Data(),jets.at(ijet).NeutralEMEnergyFraction()  , weight, 0.,1.,20);
	      FillHist(("closejet" + name + "_NeutralHadEnergyFraction").Data(),jets.at(ijet).NeutralHadEnergyFraction() , weight, 0.,1.,20);
	      FillHist(("closejet" + name + "_ChargedEMEnergyFraction").Data(),jets.at(ijet).ChargedEMEnergyFraction() , weight, 0.,1.,20);
	      FillHist(("closejet" + name + "_ChargedHadEnergyFraction").Data(),jets.at(ijet).ChargedHadEnergyFraction() , weight, 0.,1.,20);
	      FillHist(("closejet" + name + "_ElectronEnergyFraction").Data(),jets.at(ijet).ElectronEnergyFraction() , weight, 0.,1.,20);
	      FillHist(("closejet" + name + "_HadEnergyFraction").Data(),jets.at(ijet).NeutralHadEnergyFraction()+ jets.at(ijet).ChargedHadEnergyFraction() , weight, 0.,1.,200);
	      FillHist(("closejet" + name + "_ChargedEMEnergyFraction_elpt").Data(),jets.at(ijet).ChargedEMEnergyFraction() , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200, 20);
	      FillHist(("closejet" + name + "_ChargedHadEnergyFraction_elpt").Data(),jets.at(ijet).ChargedHadEnergyFraction()  , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200,  20);
	      FillHist(("closejet" + name + "_NeutralEMEnergyFraction_elpt").Data(),jets.at(ijet).NeutralEMEnergyFraction() , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200,  20);

	      
	    }
	    else{
	      FillHist(("awayjet" + name + "_NeutralEMEnergyFraction").Data(),jets.at(ijet).NeutralEMEnergyFraction()  , weight, 0.,1.,20);
              FillHist(("awayjet" + name + "_NeutralHadEnergyFraction").Data(),jets.at(ijet).NeutralHadEnergyFraction() , weight, 0.,1.,20);
              FillHist(("awayjet" + name + "_ChargedEMEnergyFraction").Data(),jets.at(ijet).ChargedEMEnergyFraction() , weight, 0.,1.,20);
              FillHist(("awayjet" + name + "_ChargedHadEnergyFraction").Data(),jets.at(ijet).ChargedHadEnergyFraction() , weight, 0.,1.,20);
              FillHist(("awayjet" + name + "_ElectronEnergyFraction").Data(),jets.at(ijet).ElectronEnergyFraction() , weight, 0.,1.,20);
              FillHist(("awayjet" + name + "_HadEnergyFraction").Data(),jets.at(ijet).NeutralHadEnergyFraction()+ jets.at(ijet).ChargedHadEnergyFraction() , weight, 0.,1.,200);
              FillHist(("awayjet" + name + "_ChargedEMEnergyFraction_elpt").Data(),jets.at(ijet).ChargedEMEnergyFraction() , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200,   20);
              FillHist(("awayjet" + name + "_ChargedHadEnergyFraction_elpt").Data(),jets.at(ijet).ChargedHadEnergyFraction()  , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200,  20);
              FillHist(("awayjet" + name + "_NeutralEMEnergyFraction_elpt").Data(),jets.at(ijet).NeutralEMEnergyFraction() , electrons[iel].Pt(),  weight, 0.,1.,20, 0.,200, 20);

	    }
	  }
	}
      }
    }
  }
  return;
}

void HNElectronOptimisation::EndCycle()throw( LQError ){
  
  Message("In EndCycle" , INFO);
  m_logger << INFO << "Number of os mc events = " << m_os_Z_nw  << LQLogger::endmsg; 
  m_logger << INFO << "Number of os mc events (weighted) = " << m_os_Z  << LQLogger::endmsg; 
  m_logger << INFO << "Number of ss mc events = " << m_ss_Z_nw  << LQLogger::endmsg; 
  m_logger << INFO << "Number of ss mc events (weighted)= " << m_ss_Z  << LQLogger::endmsg; 
}


void HNElectronOptimisation::BeginCycle() throw( LQError ){
  
  Message("In begin Cycle", INFO);
  
  string analysisdir = getenv("FILEDIR");  

  if(!k_isdata) reweightPU = new Reweight((analysisdir + "MyDataPileupHistogram.root").c_str());

  //
  //If you wish to output variables to output file use DeclareVariable
  // clear these variables in ::ClearOutputVectors function
  //DeclareVariable(obj, label, treename );
  //DeclareVariable(obj, label ); //-> will use default treename: LQTree
  DeclareVariable(out_electrons, "Signal_Electrons", "LQTree");
  DeclareVariable(out_muons, "Signal_Muons");
  
  
  
  return;
  
}

HNElectronOptimisation::~HNElectronOptimisation() {
  
  Message("In HNElectronOptimisation Destructor" , INFO);
  if(!k_isdata)delete reweightPU;
  
 }
     
     
void HNElectronOptimisation::FillCutFlow(TString cut, float weight){

  
  if(GetHist("cutflow")) {
    GetHist("cutflow")->Fill(cut,weight);
   
  }
  else{
    AnalyzerCore::MakeHistograms("cutflow",16,0.,16.);

    GetHist("cutflow")->GetXaxis()->SetBinLabel(1,"SS_NoCut");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(2,"SS_Tight");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(3,"SS_Tight_convveto");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(4,"SS_Tight_d0veto");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(5,"SS_Tight_reliso");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(6,"SS_Medium_chargeconst");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(7,"SS_Tight_chargeconst");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(8,"SS_Tight_noclosejet");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(9,"SS_anal_el");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(10,"Signal_anal");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(11,"Signal_Tightlooseiso_d0");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(12,"Signal_Mediumlooseiso_d0");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(13,"Signal_drcut1");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(14,"Signal_drcut2");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(15,"Signal_anal_dr1");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(16,"Signal_anal_dr2");
  }
}
     
     
void HNElectronOptimisation::FillIsoCutFlow(TString cut, float w){
       
  
  if(GetHist("isocutflow")) {
    if(cut.Contains("nocut")) GetHist("isocutflow")->Fill(0.,w);
       //GetHist("isocutflow")->Fill(cut,double(w));
    
  }
  else{
    AnalyzerCore::MakeHistograms("isocutflow",7,0.,7.);
    
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(1,"pogiso_d0_nocut");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(2,"pogiso_d0_05");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(3,"pogiso_d0_10");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(4,"pogiso_d0_15");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(5,"pogiso_d0_20");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(6,"pogiso_d0_25");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(7,"pogiso_d0_30");
   
    
  }
}


void HNElectronOptimisation::BeginEvent( )throw( LQError ){

  Message("In BeginEvent() " , DEBUG);

  return;
}


///############### THESE ARE FUNCTIONS SPECIFIC TO THIS CYCLE

void HNElectronOptimisation::MakeHistograms(){
  //// Additional plots to make
    
  maphist.clear();
  AnalyzerCore::MakeHistograms();
  Message("Made histograms", INFO);
  /**
   *  Remove//Overide this HNElectronOptimisationCore::MakeHistograms() to make new hists for your analysis
   **/
  
}


void HNElectronOptimisation::ClearOutputVectors() throw(LQError) {

  // This function is called before every execute event (NO need to call this yourself.
  
  // Add any new output vector you create to this list. 
  // if you do not the vector will keep keep getting larger when it is filled in ExecuteEvents and will use excessive amoun of memory
  //
  // Reset all variables declared in Declare Variable
  //
  out_muons.clear();
  out_electrons.clear();
}



