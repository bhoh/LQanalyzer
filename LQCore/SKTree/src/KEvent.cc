#include "KEvent.h"

#include <iomanip>
#include <iostream>

using namespace std;
using namespace snu;

ClassImp(KEvent)

/**
 *Default constructor.
 */
KEvent::KEvent() :
  TObject(),
  k_EventNumber(0),
  k_RunNumber(0),
  k_nvertices(0),
  k_ivertex(0),
  k_process_id(0),
  k_lumisec(0),
  k_vertexX(0.),
  k_vertexY(0.),
  k_vertexZ(0.), 
  k_jetrho(0.),
  k_mcweight(0.),
  k_pdf_cteq(),
  k_pdf_mstw(),
  k_pdf_nnpdf(),
  k_genmettrue(0.),
  k_genmetphitrue(0.),
  k_gensumettrue(0.),
  k_genmetcalo(0.),
  k_genmetphicalo(0.),
  k_gensumetcalo(0.),
  k_PF_MET(0.), 
  k_PF_METx(0.), 
  k_PF_METy(0.), 
  k_PF_METphi(0.),
  k_PF_SumET(0.), 
  k_PF_METraw(0.),
  k_PF_METxraw(0.),
  k_PF_METyraw(0.),
  k_PF_METrawphi(0.),
  k_PF_METtype1(0.),
  k_PF_METtype1phi(0.),
  k_PF_METtype01(0.),
  k_PF_METtype01phi(0.),
  k_PF_MET_el_enup(0.),
  k_PF_MET_el_endown(0.),
  k_PF_MET_jet_enup(0.),
  k_PF_MET_jet_endown(0.),
  k_PF_MET_jet_resup(0.),
  k_PF_MET_jet_resdown(0.),
  k_PF_MET_muon_enup(0.),
  k_PF_MET_muon_endown(0.),
  k_PF_MET_unclust_up(0.),
  k_PF_MET_unclust_down(0.),
  k_PF_METx_el_enup(0.),
  k_PF_METx_el_endown(0.),
  k_PF_METx_jet_enup(0.),
  k_PF_METx_jet_endown(0.),
  k_PF_METx_jet_resup(0.),
  k_PF_METx_jet_resdown(0.),
  k_PF_METx_muon_enup(0.),
  k_PF_METx_muon_endown(0.),
  k_PF_METx_unclust_up(0.),
  k_PF_METx_unclust_down(0.),
  k_PF_METy_el_enup(0.),
  k_PF_METy_el_endown(0.),
  k_PF_METy_jet_enup(0.),
  k_PF_METy_jet_endown(0.),
  k_PF_METy_jet_resup(0.),
  k_PF_METy_jet_resdown(0.),
  k_PF_METy_muon_enup(0.),
  k_PF_METy_muon_endown(0.),
  k_PF_METy_unclust_up(0.),
  k_PF_METy_unclust_down(0.),
  k_isfakeV(false), 
  k_isgoodevent(false),
  k_isData(false), 
  k_isTrackingFailure(false),
  k_isbeamscrape(false),
  k_passTrackingFailureFilter(false),
  k_passBeamHaloFilterLoose(false),
  k_passBeamHaloFilterTight(false),
  k_passBadEESupercrystalFilter(false),
  k_passEcalDeadCellBoundaryEnergyFilter(false),
  k_passEcalDeadCellTriggerPrimitiveFilter(false),
  k_passEcalLaserCorrFilter(false),
  k_passHBHENoiseFilter(false),
  k_passHcalLaserEventFilter(false),
  k_PileUpInteractionsTrue(0.)
{

}

/**
 * Copy constructor.
 */
KEvent::KEvent(const KEvent& ev) :
  TObject(),
  k_EventNumber(ev.k_EventNumber),
  k_RunNumber(ev.k_RunNumber),
  k_nvertices(ev.k_nvertices),
  k_ivertex(ev.k_ivertex),
  k_process_id(ev.k_process_id),
  k_lumisec(ev.k_lumisec),
  k_vertexX(ev.k_vertexX),
  k_vertexY(ev.k_vertexY),
  k_vertexZ(ev.k_vertexZ),
  k_jetrho(ev.k_jetrho),
  k_mcweight(ev.k_mcweight),

  k_pdf_cteq(ev.k_pdf_cteq),
  k_pdf_mstw(ev.k_pdf_mstw),
  k_pdf_nnpdf(ev.k_pdf_nnpdf),
  k_genmettrue(ev.k_genmettrue),
  k_genmetphitrue(ev.k_genmetphitrue),
  k_gensumettrue(ev.k_gensumettrue),
  k_genmetcalo(ev.k_genmetcalo),
  k_genmetphicalo(ev.k_genmetphicalo),
  k_gensumetcalo(ev.k_gensumetcalo),
  k_PF_MET(ev.k_PF_MET),
  k_PF_METx(ev.k_PF_METx),
  k_PF_METy(ev.k_PF_METy),
  k_PF_METphi(ev.k_PF_METphi),
  k_PF_SumET(ev.k_PF_SumET),
  k_PF_METraw(ev.k_PF_METraw),
  k_PF_METxraw(ev.k_PF_METxraw),
  k_PF_METyraw(ev.k_PF_METyraw),
  k_PF_METrawphi(ev.k_PF_METrawphi),
  k_PF_METtype1(ev.k_PF_METtype1),
  k_PF_METtype1phi(ev.k_PF_METtype1phi),
  k_PF_METtype01(ev.k_PF_METtype01),
  k_PF_METtype01phi(ev.k_PF_METtype01phi),
  k_PF_MET_el_enup(ev.k_PF_MET_el_enup),
  k_PF_MET_el_endown(ev.k_PF_MET_el_endown),
  k_PF_MET_jet_enup(ev.k_PF_MET_jet_enup),
  k_PF_MET_jet_endown(ev.k_PF_MET_jet_endown),
  k_PF_MET_jet_resup(ev.k_PF_MET_jet_resup),
  k_PF_MET_jet_resdown(ev.k_PF_MET_jet_resdown),
  k_PF_MET_muon_enup(ev.k_PF_MET_muon_enup),
  k_PF_MET_muon_endown(ev.k_PF_MET_muon_endown),
  k_PF_MET_unclust_up(ev.k_PF_MET_unclust_up),
  k_PF_MET_unclust_down(ev.k_PF_MET_unclust_down),
  k_PF_METx_el_enup(ev.k_PF_METx_el_enup),
  k_PF_METx_el_endown(ev.k_PF_METx_el_endown),
  k_PF_METx_jet_enup(ev.k_PF_METx_jet_enup),
  k_PF_METx_jet_endown(ev.k_PF_METx_jet_endown),
  k_PF_METx_jet_resup(ev.k_PF_METx_jet_resup),
  k_PF_METx_jet_resdown(ev.k_PF_METx_jet_resdown),
  k_PF_METx_muon_enup(ev.k_PF_METx_muon_enup),
  k_PF_METx_muon_endown(ev.k_PF_METx_muon_endown),
  k_PF_METx_unclust_up(ev.k_PF_METx_unclust_up),
  k_PF_METx_unclust_down(ev.k_PF_METx_unclust_down),
  k_PF_METy_el_enup(ev.k_PF_METy_el_enup),
  k_PF_METy_el_endown(ev.k_PF_METy_el_endown),
  k_PF_METy_jet_enup(ev.k_PF_METy_jet_enup),
  k_PF_METy_jet_endown(ev.k_PF_METy_jet_endown),
  k_PF_METy_jet_resup(ev.k_PF_METy_jet_resup),
  k_PF_METy_jet_resdown(ev.k_PF_METy_jet_resdown),
  k_PF_METy_muon_enup(ev.k_PF_METy_muon_enup),
  k_PF_METy_muon_endown(ev.k_PF_METy_muon_endown),
  k_PF_METy_unclust_up(ev.k_PF_METy_unclust_up),
  k_PF_METy_unclust_down(ev.k_PF_METy_unclust_down),
  k_isfakeV(ev.k_isfakeV),
  k_isgoodevent(ev.k_isgoodevent),
  k_isData(ev.k_isData),
  k_isTrackingFailure(ev.k_isTrackingFailure),
  k_isbeamscrape(ev.k_isbeamscrape),
  k_passTrackingFailureFilter(ev.k_passTrackingFailureFilter),
  k_passBeamHaloFilterLoose(ev.k_passBeamHaloFilterLoose),
  k_passBeamHaloFilterTight(ev.k_passBeamHaloFilterTight),
  k_passBadEESupercrystalFilter(ev.k_passBadEESupercrystalFilter),
  k_passEcalDeadCellBoundaryEnergyFilter(ev.k_passEcalDeadCellBoundaryEnergyFilter),
  k_passEcalDeadCellTriggerPrimitiveFilter(ev.k_passEcalDeadCellTriggerPrimitiveFilter),
  k_passEcalLaserCorrFilter(ev.k_passEcalLaserCorrFilter),
  k_passHBHENoiseFilter(ev.k_passHBHENoiseFilter),
  k_passHcalLaserEventFilter(ev.k_passHcalLaserEventFilter),
  k_PileUpInteractionsTrue(ev.k_PileUpInteractionsTrue)
{
}


KEvent::~KEvent()
{
}

void KEvent::Reset()
{
  
  k_EventNumber= 0;
  k_RunNumber= 0;
  k_nvertices= 0;
  k_ivertex= 0;
  k_process_id= 0;
  k_lumisec=0;
  k_vertexX= 0.;
  k_vertexY= 0.;
  k_vertexZ= 0.;
  k_PF_MET= 0.;
  k_PF_METx= 0.;
  k_PF_METy= 0.;
  k_PF_SumET= 0.;
  k_PF_METphi= 0.;
  k_PF_METraw= 0.;
  k_PF_METxraw= 0.;
  k_PF_METyraw= 0.;
  k_PF_METtype1= 0.;
  k_PF_METtype01= 0.;
  k_PF_METrawphi= 0.;
  k_PF_METtype1phi= 0.;
  k_PF_METtype01phi= 0.;
  k_pdf_cteq.clear();
  k_pdf_mstw.clear(); 
  k_pdf_nnpdf.clear();
  k_genmettrue = 0.;
  k_genmetphitrue= 0.;
  k_gensumettrue= 0.;
  k_genmetcalo= 0.;
  k_genmetphicalo= 0.;
  k_gensumetcalo= 0.;
  k_PF_MET_el_enup= 0.;
  k_PF_MET_el_endown= 0.;
  k_PF_MET_jet_enup= 0.;
  k_PF_MET_jet_endown= 0.;
  k_PF_MET_jet_resup= 0.;
  k_PF_MET_jet_resdown= 0.;
  k_PF_MET_muon_enup= 0.;
  k_PF_MET_muon_endown= 0.;
  k_PF_MET_unclust_up= 0.;
  k_PF_MET_unclust_down= 0.;
  k_PF_METx_el_enup= 0.;
  k_PF_METx_el_endown= 0.;
  k_PF_METx_jet_enup= 0.;
  k_PF_METx_jet_endown= 0.;
  k_PF_METx_jet_resup= 0.;
  k_PF_METx_jet_resdown= 0.;
  k_PF_METx_muon_enup= 0.;
  k_PF_METx_muon_endown= 0.;
  k_PF_METx_unclust_up= 0.;
  k_PF_METx_unclust_down= 0.;
  k_PF_METy_el_enup= 0.;
  k_PF_METy_el_endown= 0.;
  k_PF_METy_jet_enup= 0.;
  k_PF_METy_jet_endown= 0.;
  k_PF_METy_jet_resup= 0.;
  k_PF_METy_jet_resdown= 0.;
  k_PF_METy_muon_enup= 0.;
  k_PF_METy_muon_endown= 0.;
  k_PF_METy_unclust_up= 0.;
  k_PF_METy_unclust_down= 0.;
  k_jetrho= 0.;
  k_mcweight= 0.;
  k_isfakeV= false;
  k_isData= false;
  k_isgoodevent = false;
  k_isTrackingFailure= false;
  k_isbeamscrape=false;
  k_passTrackingFailureFilter= false;
  k_passBeamHaloFilterLoose= false;
  k_passBeamHaloFilterTight= false;
  k_passBadEESupercrystalFilter= false;
  k_passEcalDeadCellBoundaryEnergyFilter= false;
  k_passEcalDeadCellTriggerPrimitiveFilter= false;
  k_passEcalLaserCorrFilter= false;
  k_passHBHENoiseFilter= false;
  k_passHcalLaserEventFilter= false;
  k_PileUpInteractionsTrue = 0.;


}



KEvent& KEvent::operator= (const KEvent& p)
{
    if (this != &p) {
      k_isData=p.IsData();
      k_EventNumber=p.EventNumber();
      k_RunNumber=p.RunNumber();

      k_process_id= p.ProcessID();
      k_lumisec= p.LumiSection();
      k_PF_MET= p.PFMET();
      k_PF_METx= p.PFMETx();
      k_PF_METy= p.PFMETy();
      k_PF_METphi= p.PFMETphi();
      k_PF_SumET = p.PFSumET();
      k_PF_METraw = p.PFMETRaw();
      k_PF_METxraw = p.PFMETxRaw();
      k_PF_METyraw = p.PFMETyRaw();
      k_PF_METrawphi = p.PFMETRawphi();
      k_PF_METtype1 = p.PFMETType1();
      k_PF_METtype1phi = p.PFMETType1phi();
      k_PF_METtype01 = p.PFMETType01();
      k_PF_METtype01phi = p.PFMETType01phi();
      k_pdf_cteq = p.PDFCTEQWeight();
      k_pdf_mstw = p.PDFMSTWWeight();
      k_pdf_nnpdf = p.PDFNNPDFWeight();

      k_genmettrue = p.GenMETTrue();
      k_genmetphitrue=  p.GenMETPhiTrue();
      k_gensumettrue=  p.GenSumEtTrue();
      k_genmetcalo=  p.GenMETCalo();
      k_genmetphicalo=  p.GenMETPhiCalo();
      k_gensumetcalo=  p.GenSumEtCalo();
     

      k_PF_MET_el_enup = p.PFMET_El_EnUp();
      k_PF_MET_el_endown  = p.PFMET_El_EnDown();
      k_PF_MET_jet_enup = p.PFMET_Jet_EnUp();
      k_PF_MET_jet_endown = p.PFMET_Jet_EnDown();
      k_PF_MET_jet_resup = p.PFMET_Jet_ResUp();
      k_PF_MET_jet_resdown  = p.PFMET_Jet_ResDown();
      k_PF_MET_muon_enup = p.PFMET_Muon_EnUp();
      k_PF_MET_muon_endown= p.PFMET_Muon_EnDown();
      k_PF_MET_unclust_up = p.PFMET_Unclust_Up();
      k_PF_MET_unclust_down  = p.PFMET_Unclust_Down();
      
      k_PF_METx_el_enup = p.PFMETx_El_EnUp();
      k_PF_METx_el_endown  = p.PFMETx_El_EnDown();
      k_PF_METx_jet_enup = p.PFMETx_Jet_EnUp();
      k_PF_METx_jet_endown = p.PFMETx_Jet_EnDown();
      k_PF_METx_jet_resup = p.PFMETx_Jet_ResUp();
      k_PF_METx_jet_resdown  = p.PFMETx_Jet_ResDown();
      k_PF_METx_muon_enup = p.PFMETx_Muon_EnUp();
      k_PF_METx_muon_endown= p.PFMETx_Muon_EnDown();
      k_PF_METx_unclust_up = p.PFMETx_Unclust_Up();
      k_PF_METx_unclust_down  = p.PFMETx_Unclust_Down();


      k_PF_METy_el_enup = p.PFMETy_El_EnUp();
      k_PF_METy_el_endown  = p.PFMETy_El_EnDown();
      k_PF_METy_jet_enup = p.PFMETy_Jet_EnUp();
      k_PF_METy_jet_endown = p.PFMETy_Jet_EnDown();
      k_PF_METy_jet_resup = p.PFMETy_Jet_ResUp();
      k_PF_METy_jet_resdown  = p.PFMETy_Jet_ResDown();
      k_PF_METy_muon_enup = p.PFMETy_Muon_EnUp();
      k_PF_METy_muon_endown= p.PFMETy_Muon_EnDown();
      k_PF_METy_unclust_up = p.PFMETy_Unclust_Up();
      k_PF_METy_unclust_down  = p.PFMETy_Unclust_Down();
      k_vertexX=p.VertexX();
      k_vertexY=p.VertexY();
      k_vertexZ=p.VertexZ();
      k_nvertices= p.nVertices();
      k_isfakeV=p.IsFakeVertex(); 
      k_jetrho=p.JetRho();
      k_isgoodevent= p.HasGoodPrimaryVertex();
      k_mcweight = p.MCWeight();
      k_isTrackingFailure = p.IsTrackingFailure();
      k_isbeamscrape =p.IsBeamScraping();
      k_passTrackingFailureFilter = p.PassTrackingFailureFilter();
      k_passBeamHaloFilterLoose = p.PassBeamHaloFilterLoose();
      k_passBeamHaloFilterTight = p.PassBeamHaloFilterTight();
      k_passBadEESupercrystalFilter = p.PassBadEESupercrystalFilter();
      k_passEcalDeadCellBoundaryEnergyFilter = p.PassEcalDeadCellBoundaryEnergyFilter();
      k_passEcalDeadCellTriggerPrimitiveFilter = p.PassEcalDeadCellTriggerPrimitiveFilter();
      k_passEcalLaserCorrFilter = p.PassEcalLaserCorrFilter();
      k_passHBHENoiseFilter = p.PassHBHENoiseFilter();
      k_passHcalLaserEventFilter = p.PassHcalLaserEventFilter();
      k_PileUpInteractionsTrue = p.PileUpInteractionsTrue();
    }
    
    return *this;
}

//// SET CLASS VARIBALES
void KEvent::SetIsTrackingFailure(bool fail){
  k_isTrackingFailure = fail;
}
void KEvent::SetIsBeamScraping(bool fail){
  k_isbeamscrape= fail;
}


void KEvent::SetPassTrackingFailureFilter(bool pass){
  k_passTrackingFailureFilter= pass;
}

void KEvent::SetPassBeamHaloFilterLoose(bool pass){
  k_passBeamHaloFilterLoose = pass;
}

void KEvent::SetPassBeamHaloFilterTight(bool pass){
  k_passBeamHaloFilterTight = pass;
}

void KEvent::SetPassBadEESupercrystalFilter(bool pass){
  k_passBadEESupercrystalFilter = pass;
}

void KEvent::SetPassEcalDeadCellBoundaryEnergyFilter(bool pass){
  k_passEcalDeadCellBoundaryEnergyFilter = pass;
}

void KEvent::SetPassEcalDeadCellTriggerPrimitiveFilter(bool pass){
  k_passEcalDeadCellTriggerPrimitiveFilter = pass;
}

void KEvent::SetPassEcalLaserCorrFilter(bool pass){
  k_passEcalLaserCorrFilter= pass;
}

void KEvent::SetPassHBHENoiseFilter(bool pass){
  k_passHBHENoiseFilter= pass;
}
void KEvent::SetPassHcalLaserEventFilter(bool pass){

  k_passHcalLaserEventFilter = pass;
}

void KEvent::SetPileUpInteractionsTrue(double npu){
  k_PileUpInteractionsTrue = npu;
}

void KEvent::SetWeight(double mcweight){
  k_mcweight = mcweight;
}

void KEvent::SetJetRho(double jetrho){

  k_jetrho = jetrho;
}



///

void KEvent::SetGenMETTrue(double genmettrue){
  k_genmettrue = genmettrue;
}
void KEvent::SetGenMETPhiTrue(double genmetphitrue){
  k_genmetphitrue = genmetphitrue;
}
void KEvent::SetGenSumEtTrue(double gensumettrue){
  k_gensumettrue = gensumettrue;
}

void KEvent::SetGenMETCalo(double genmetcalo){
  k_genmetcalo = genmetcalo;
}
void KEvent::SetGenMETPhiCalo(double genmetphicalo){
  k_genmetphicalo = genmetphicalo;
}
void KEvent::SetGenSumEtCalo(double gensumetcalo){
  k_gensumetcalo = gensumetcalo;
}

///PDF weights

void KEvent::SetPDFCTEQWeight(vector<double> pdfw){
  k_pdf_cteq = pdfw;
}

void KEvent::SetPDFMSTWWeight(vector<double> pdfw){
  k_pdf_mstw= pdfw;
}

void KEvent::SetPDFNNPDFWeight(vector<double> pdfw){
  k_pdf_nnpdf =pdfw;
}
/// Filling Event variables

void KEvent::SetEventNumber(int ev){
  k_EventNumber = ev;
}

void KEvent::SetRunNumber(int rn){
  k_RunNumber = rn;
}

void KEvent::SetIsData(bool isdata){
  k_isData = isdata;
}



//// Setting Missing ET variables
void KEvent::SetPFMET(double met){
  k_PF_MET = met;
}

void KEvent::SetPFMETx(double metx){
  k_PF_METx = metx;
}

void KEvent::SetPFMETy(double mety){
  k_PF_METy = mety;
}
void KEvent::SetPFMETphi(double metphi){
  k_PF_METphi = metphi;
}

void KEvent::SetPFSumET(double sumet){
  k_PF_SumET =sumet;
}


void KEvent::SetPFMETRaw(double met){
  k_PF_METraw = met;
}

void KEvent::SetPFMETxRaw(double metx){
  k_PF_METxraw = metx;
}

void KEvent::SetPFMETyRaw(double mety){
  k_PF_METyraw = mety;
}

void KEvent::SetPFMETRawphi(double phi){
  k_PF_METrawphi = phi;
}

void KEvent::SetPFMETType1(double met){
  k_PF_METtype1 = met;
}

void KEvent::SetPFMETType1phi(double phi){
  k_PF_METtype1phi = phi;
}

void KEvent::SetPFMETType01(double met){
  k_PF_METtype01 = met;
}

void KEvent::SetPFMETType01phi(double phi){
  k_PF_METtype01phi = phi;
}



void KEvent::SetPFMETElectronEnDown (double met_elen_down){
  k_PF_MET_el_endown = met_elen_down;
}
void KEvent::SetPFMETElectronEnUp (double met_elen_up){
  k_PF_MET_el_enup= met_elen_up;
}
void KEvent::SetPFMETJetEnDown (double met_jeten_down){
  k_PF_MET_jet_endown = met_jeten_down;
}
void KEvent::SetPFMETJetEnUp (double met_jeten_up){
  k_PF_MET_jet_enup = met_jeten_up;
}
void KEvent::SetPFMETJetResDown (double  met_jetres_down){
  k_PF_MET_jet_resdown = met_jetres_down;
}
void KEvent::SetPFMETJetResUp (double  met_jetres_up){
  k_PF_MET_jet_resup= met_jetres_up;
}
void KEvent::SetPFMETMuonEnDown (double met_muonen_down){
  k_PF_MET_muon_endown= met_muonen_down;
}
void KEvent::SetPFMETMuonEnUp (double met_muonen_up){
  k_PF_MET_muon_enup= met_muonen_up;
}
void KEvent::SetPFMETUnclusteredDown  (double met_unclust_down){
  k_PF_MET_unclust_down = met_unclust_down;
}
void KEvent::SetPFMETUnclusteredUp  (double met_unclust_up){
  k_PF_MET_unclust_up = met_unclust_up;
}


void KEvent::SetPFMETxElectronEnDown (double metx_elen_down){
  k_PF_METx_el_endown = metx_elen_down;
}
void KEvent::SetPFMETxElectronEnUp (double metx_elen_up){
  k_PF_METx_el_enup= metx_elen_up;
}
void KEvent::SetPFMETxJetEnDown (double metx_jeten_down){
  k_PF_METx_jet_endown = metx_jeten_down;
}
void KEvent::SetPFMETxJetEnUp (double metx_jeten_up){
  k_PF_METx_jet_enup = metx_jeten_up;
}
void KEvent::SetPFMETxJetResDown (double  metx_jetres_down){
  k_PF_METx_jet_resdown = metx_jetres_down;
}
void KEvent::SetPFMETxJetResUp (double  metx_jetres_up){
  k_PF_METx_jet_resup= metx_jetres_up;
}
void KEvent::SetPFMETxMuonEnDown (double metx_muonen_down){
  k_PF_METx_muon_endown= metx_muonen_down;
}
void KEvent::SetPFMETxMuonEnUp (double metx_muonen_up){
  k_PF_METx_muon_enup= metx_muonen_up;
}
void KEvent::SetPFMETxUnclusteredDown  (double metx_unclust_down){
  k_PF_METx_unclust_down = metx_unclust_down;
}
void KEvent::SetPFMETxUnclusteredUp  (double metx_unclust_up){
  k_PF_METx_unclust_up = metx_unclust_up;
}



void KEvent::SetPFMETyElectronEnDown (double mety_elen_down){
  k_PF_METy_el_endown = mety_elen_down;
}
void KEvent::SetPFMETyElectronEnUp (double mety_elen_up){
  k_PF_METy_el_enup= mety_elen_up;
}
void KEvent::SetPFMETyJetEnDown (double mety_jeten_down){
  k_PF_METy_jet_endown = mety_jeten_down;
}
void KEvent::SetPFMETyJetEnUp (double mety_jeten_up){
  k_PF_METy_jet_enup = mety_jeten_up;
}
void KEvent::SetPFMETyJetResDown (double  mety_jetres_down){
  k_PF_METy_jet_resdown = mety_jetres_down;
}
void KEvent::SetPFMETyJetResUp (double  mety_jetres_up){
  k_PF_METy_jet_resup= mety_jetres_up;
}
void KEvent::SetPFMETyMuonEnDown (double mety_muonen_down){
  k_PF_METy_muon_endown= mety_muonen_down;
}
void KEvent::SetPFMETyMuonEnUp (double mety_muonen_up){
  k_PF_METy_muon_enup= mety_muonen_up;
}
void KEvent::SetPFMETyUnclusteredDown  (double mety_unclust_down){
  k_PF_METy_unclust_down = mety_unclust_down;
}
void KEvent::SetPFMETyUnclusteredUp  (double mety_unclust_up){
  k_PF_METy_unclust_up = mety_unclust_up;
}




/// Settizang Vertex Variables
void KEvent::SetNVertices(int nvert){
  k_nvertices = nvert;
}

void KEvent::SetVertexIndex(int ivertex){
  k_ivertex = ivertex;
}

void KEvent::SetProcessID(int id){
  k_process_id = id;
}

void KEvent::SetLumiSection(int ls){
  k_lumisec = ls;
}


void KEvent::SetVertexIsFake(bool isfakeV){
  k_isfakeV = isfakeV;
}

void KEvent::SetIsGoodEvent(bool isgood){
  k_isgoodevent = isgood;
}

void KEvent::SetVertexX(double vX){
  k_vertexX = vX;
}

void KEvent::SetVertexY(double vY){
  k_vertexY = vY;
}

void KEvent::SetVertexZ(double vZ){
  k_vertexZ = vZ;
}




