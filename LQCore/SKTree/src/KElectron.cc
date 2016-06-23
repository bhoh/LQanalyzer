#include "KElectron.h"

#include <iomanip>
#include <iostream>

using namespace std;
using namespace snu;

ClassImp(KElectron)

/**
 *Default constructor.
 */
KElectron::KElectron() :
KParticle()
{
  k_isEB =false;
  k_isEE = false; 
  k_sceta=0;
  k_scphi=0;
  k_scpt=0;
  k_rawenergy=0;
  k_mva=0.;
  k_trigmva=0.;
  k_trackdrivenseed = false;
  k_ecaldrivenseed= false;
  k_delta_etatrkSC=0.;
  k_delta_phitrkSC=0.;
  k_sigmaIEtaIEta=0.;
  k_hoe=0.;
  k_caloE=0.;
  k_E_cluster_over_p=0.;
  k_trkvx=0.;
  k_trkvy=0.;
  k_trkvz=0.;
  k_vtx_xy=0.;
  k_vtx_z=0.;
  k_lvtx_xy=0.;
  k_lvtx_z=0.;
  k_dxy=0;
  k_dz=0;
  k_gsf_ctscpix_charge=false;
  k_gsf_scpix_charge=false;
  k_gsf_ct_charge=false;
  k_hasmatchconvphot=false; 
  k_missinghits=0;
  k_missing_lost_hits=0;
  k_convFitProb=0.;
  k_nbrems=0.;
  k_fbrem=0.;
  k_cottheta=0.;
  k_eldist=0.;
  k_trkiso_03=0.;
  k_ecaliso_03=0.;
  k_hcaliso_03=0.;
  k_trkiso_04=0.;
  k_ecaliso_04=0.;
  k_hcaliso_04=0.;
  k_pf_chargedhad_iso03=0.;
  k_pf_photon_iso03=0.;
  k_pf_neutral_iso03=0.;
  k_pf_chargedhad_iso04=0.;
  k_pf_photon_iso04=0.;
  k_pf_neutral_iso04=0.;
  k_matched_gen_pt=0.;
  k_matched_gen_eta=0.;
  k_matched_gen_phi=0.;
  k_diel_trig_match= false;
  k_el_trig_match8= false;
  k_el_trig_match17= false;
  k_elwp80_trig_match= false;
  k_emu_trig_match8= false;
  k_emu_trig_match17= false;
  k_track_pt = 0.;
  k_track_hitfrac=0.;
  k_pv_dist_xy=0.;
  k_pv_dist_xy_error=0.;
  k_vertex_index=0;

  k_pt_shifted_up=0.;
  k_pt_shifted_down=0.;
  k_px_shifted_up=0.;
  k_px_shifted_down=0.;
  k_py_shifted_up=0.;
  k_py_shifted_down=0.;

}

/**
 * Copy constructor.
 */
KElectron::KElectron(const KElectron& el) :
    KParticle(el)
{
  
  k_isEB = el.isEB();
  k_isEE = el.isEE();
  k_sceta= el.SCEta();
  k_scpt= el.SCPt();
  k_scphi= el.SCPhi();
  k_rawenergy= el.RawEnergy();
  k_mva=el.MVA();
  k_trigmva=el.TrigMVA();
  k_trackdrivenseed = el.TrackerDrivenSeed(); 
  k_ecaldrivenseed= el.EcalDrivenSeed(); 
  k_delta_etatrkSC= el.DeltaEta();
  k_delta_phitrkSC= el.DeltaPhi();
  k_sigmaIEtaIEta= el.SigmaIEtaIEta();
  k_hoe= el.HoE();
  k_caloE= el.CaloEnergy();
  k_E_cluster_over_p= el.ESuperClusterOverP();
  k_trkvx= el.TrkVx();
  k_trkvy= el.TrkVy();
  k_trkvz= el.TrkVz();
  k_vtx_xy= el.VtxDistXY();
  k_vtx_z= el.VtxDistZ();
  k_lvtx_xy= el.LeadVtxDistXY();
  k_lvtx_z= el.LeadVtxDistZ();
  k_dxy= el.dxy();
  k_dz= el.dz();
  k_gsf_ctscpix_charge= el.GsfCtfScPixChargeConsistency();
  k_gsf_scpix_charge= el.GsffScPixChargeConsistency();
  k_gsf_ct_charge= el.GsfCtfChargeConsistency();
  k_hasmatchconvphot= el.HasMatchedConvPhot();
  k_missinghits= el.MissingHits();
  k_missing_lost_hits= el.MissingLostHits();
  k_convFitProb= el.ConvFitProb();
  k_nbrems= el.NBrems();
  k_fbrem= el.FBrem();
  k_cottheta=el.CotTheta();
  k_eldist=el.Dist();
  k_trkiso_03= el.TrkIsoDR03();
  k_ecaliso_03= el.ECalIsoDR03();
  k_hcaliso_03= el.HCalIsoDR03();
  k_trkiso_04= el.TrkIsoDR04();
  k_ecaliso_04= el.ECalIsoDR04();
  k_hcaliso_04= el.HCalIsoDR04();
  k_pf_chargedhad_iso03= el.PFChargedHadronIso03();
  k_pf_photon_iso03 = el.PFPhotonIso03();
  k_pf_neutral_iso03= el.PFNeutralHadronIso03();
  k_pf_chargedhad_iso04= el.PFChargedHadronIso04();
  k_pf_photon_iso04= el.PFPhotonIso04();
  k_pf_neutral_iso04= el.PFNeutralHadronIso04();
  k_matched_gen_pt= el.MatchedGenParticlePt();
  k_matched_gen_eta= el.MatchedGenParticleEta();
  k_matched_gen_phi= el.MatchedGenParticlePhi();
  k_diel_trig_match= el.MatchedDiElectronTrigger();
  k_el_trig_match8= el.MatchedSingleElectronTrigger8();
  k_el_trig_match17= el.MatchedSingleElectronTrigger17();
  k_elwp80_trig_match= el.MatchedSingleElectronWP80Trigger();
  k_emu_trig_match8= el.MatchedElectronMuonTrigger8();
  k_emu_trig_match17= el.MatchedElectronMuonTrigger17();
  k_track_pt = el.TrackPt();
  k_track_hitfrac=el.TrackValidHitFraction();
  k_pv_dist_xy=el.PrimaryVertexDXY();
  k_pv_dist_xy_error=el.PrimaryVertexDXYError();
  k_vertex_index=el.VertexIndex();
  k_pt_shifted_up=el.PtShiftedUp();
  k_pt_shifted_down=el.PtShiftedDown();
  k_px_shifted_up=el.PxShiftedUp();
  k_px_shifted_down=el.PxShiftedDown();
  k_py_shifted_up=el.PxShiftedUp();
  k_py_shifted_down=el.PxShiftedDown();

}


KElectron::~KElectron()
{
}

void KElectron::Reset()
{
  KParticle::Reset();
  k_isEB =false;
  k_isEE = false;
  k_sceta=0;
  k_scpt=0;
  k_scphi=0;
  k_rawenergy=0;
  k_mva=0.;
  k_trigmva=0.;
  k_trackdrivenseed = false;
  k_ecaldrivenseed= false;
  k_delta_etatrkSC=0.;
  k_delta_phitrkSC=0.;
  k_sigmaIEtaIEta=0.;
  k_hoe=0.;
  k_caloE=0.;
  k_E_cluster_over_p=0.;
  k_trkvx=0.;
  k_trkvy=0.;
  k_trkvz=0.;
  k_vtx_xy=0.;
  k_vtx_z=0.;
  k_lvtx_xy=0.;
  k_lvtx_z=0.;
  k_dxy=0;
  k_dz=0;
  k_gsf_ctscpix_charge=false;
  k_gsf_scpix_charge=false;
  k_gsf_ct_charge=false;
  k_hasmatchconvphot=false;
  k_missinghits=0;
  k_missing_lost_hits=0;
  k_convFitProb=0.;
  k_nbrems=0.;
  k_fbrem=0.;
  k_cottheta=0.;
  k_eldist=0.;
  k_trkiso_03=0.;
  k_ecaliso_03=0.;
  k_hcaliso_03=0.;
  k_trkiso_04=0.;
  k_ecaliso_04=0.;
  k_hcaliso_04=0.;
  k_pf_chargedhad_iso03=0.;
  k_pf_photon_iso03=0.;
  k_pf_neutral_iso03=0.;
  k_pf_chargedhad_iso04=0.;
  k_pf_photon_iso04=0.;
  k_pf_neutral_iso04=0.;
  k_matched_gen_pt=0.;
  k_matched_gen_eta=0.;
  k_matched_gen_phi=0.;
  k_diel_trig_match= 0.;
  k_el_trig_match8= 0.;
  k_el_trig_match17= 0.;
  k_elwp80_trig_match= 0.;
  k_emu_trig_match8= 0.;
  k_emu_trig_match17= 0.;
  k_track_pt = 0.;
  k_track_hitfrac=0.;
  k_pv_dist_xy=0.;
  k_pv_dist_xy_error=0.;
  k_vertex_index=0;
  k_pt_shifted_up=0.;
  k_pt_shifted_down=0.;
  k_px_shifted_up=0.;
  k_px_shifted_down=0.;
  k_py_shifted_up=0.;
  k_py_shifted_down=0.;
}



KElectron& KElectron::operator= (const KElectron& p)
{
  if (this != &p) {
    KParticle::operator=(p);
    k_isEB = p.isEB();
    k_isEE = p.isEE();
    k_sceta= p.SCEta();
    k_scpt= p.SCPt();
    k_scphi= p.SCPhi();
    k_rawenergy= p.RawEnergy();
    k_mva=p.MVA();
    k_trigmva=p.TrigMVA();
    k_trackdrivenseed = p.TrackerDrivenSeed();
    k_ecaldrivenseed= p.EcalDrivenSeed();
    k_delta_etatrkSC= p.DeltaEta();
    k_delta_phitrkSC= p.DeltaPhi();
    k_sigmaIEtaIEta= p.SigmaIEtaIEta();
    k_hoe= p.HoE();
    k_caloE= p.CaloEnergy();
    k_E_cluster_over_p= p.ESuperClusterOverP();
    k_trkvx= p.TrkVx();
    k_trkvy= p.TrkVy();
    k_trkvz= p.TrkVz();
    k_vtx_xy= p.VtxDistXY();
    k_vtx_z= p.VtxDistZ();
    k_lvtx_xy= p.LeadVtxDistXY();
    k_lvtx_z= p.LeadVtxDistZ();
    k_dxy= p.dxy();
    k_dz= p.dz();
    k_gsf_ctscpix_charge= p.GsfCtfScPixChargeConsistency();
    k_gsf_scpix_charge= p.GsffScPixChargeConsistency();
    k_gsf_ct_charge= p.GsfCtfChargeConsistency();
    k_hasmatchconvphot= p.HasMatchedConvPhot();
    k_missinghits= p.MissingHits();
    k_missing_lost_hits= p.MissingLostHits();
    k_convFitProb= p.ConvFitProb();
    k_nbrems= p.NBrems();
    k_fbrem= p.FBrem();
    k_cottheta=p.CotTheta();
    k_eldist=p.Dist();
    k_trkiso_03= p.TrkIsoDR03();
    k_ecaliso_03= p.ECalIsoDR03();
    k_hcaliso_03= p.HCalIsoDR03();
    k_trkiso_04= p.TrkIsoDR04();
    k_ecaliso_04= p.ECalIsoDR04();
    k_hcaliso_04= p.HCalIsoDR04();
    k_pf_chargedhad_iso03= p.PFChargedHadronIso03();
    k_pf_photon_iso03 = p.PFPhotonIso03();
    k_pf_neutral_iso03= p.PFNeutralHadronIso03();
    k_pf_chargedhad_iso04= p.PFChargedHadronIso04();
    k_pf_photon_iso04= p.PFPhotonIso04();
    k_pf_neutral_iso04= p.PFNeutralHadronIso04();
    k_matched_gen_pt= p.MatchedGenParticlePt();
    k_matched_gen_eta= p.MatchedGenParticleEta();
    k_matched_gen_phi= p.MatchedGenParticlePhi();
    k_diel_trig_match= p.MatchedDiElectronTrigger();
    k_el_trig_match8= p.MatchedSingleElectronTrigger8();
    k_el_trig_match17= p.MatchedSingleElectronTrigger17();
    k_elwp80_trig_match= p.MatchedSingleElectronWP80Trigger();
    k_emu_trig_match8= p.MatchedElectronMuonTrigger8();
    k_emu_trig_match17= p.MatchedElectronMuonTrigger17();
    k_track_pt = p.TrackPt();
    k_track_hitfrac=p.TrackValidHitFraction();
    k_pv_dist_xy=p.PrimaryVertexDXY();
    k_pv_dist_xy_error=p.PrimaryVertexDXYError();
    k_vertex_index=p.VertexIndex();
    k_pt_shifted_up=p.PtShiftedUp();
    k_pt_shifted_down=p.PtShiftedDown();
    k_px_shifted_up=p.PtShiftedUp();
    k_px_shifted_down=p.PtShiftedDown();
    k_py_shifted_up=p.PtShiftedUp();
    k_py_shifted_down=p.PtShiftedDown();
  }
  
  return *this;
}



///// SETTING CLASS VARIABLES

void KElectron::SetSCEta(Double_t sceta){
  k_sceta = sceta;
}

void KElectron::SetSCPt(Double_t scpt){
  k_scpt = scpt;
}

void KElectron::SetSCPhi(Double_t scphi){
  k_scphi = scphi;
}

void KElectron::SetRawEnergy(Double_t rawE){
  k_rawenergy = rawE;
}




void KElectron::SetPrimaryVertexDXY(Double_t pv_dist_xy){
  k_pv_dist_xy= pv_dist_xy;

}
void KElectron::SetPrimaryVertexDXYError(Double_t pv_dist_xy_error){
  k_pv_dist_xy_error = pv_dist_xy_error;
}

void KElectron::SetVtxIndex (Int_t vtx_index){
  k_vertex_index = vtx_index;
}


/// track variables

void  KElectron::SetTrackPt(Double_t pt){
  k_track_pt = pt;
}
void  KElectron::SetTrackValidFractionOfHits(Double_t valid_frac_hits){
  k_track_hitfrac = valid_frac_hits;
}

// ID variables

void KElectron::Setdz(double d_z){ 
  k_dz = d_z;
}

void KElectron::Setdxy(double d_xy){ 
  k_dxy = d_xy;
}

void KElectron::SetElectronMVA(Double_t mva){
  k_mva=mva;
}

void KElectron::SetElectronTrigMVA(Double_t trigmva){
  k_trigmva=trigmva;
}

void KElectron::SetisEB(Bool_t iseb){
  k_isEB = iseb;
}

void KElectron::SetisEE(Bool_t isee){
  k_isEE = isee;
}


void KElectron::SetTrackerDrivenSeed(Bool_t trackdrivenseed){
  k_trackdrivenseed = trackdrivenseed;
}
void KElectron::SetEcalDrivenSeed(Bool_t ecaldrivenseed){  
  k_ecaldrivenseed = ecaldrivenseed;
}


void KElectron::SetVtxDistXY(Double_t vtx_dist_xy){
  k_vtx_xy= vtx_dist_xy;
}
void KElectron::SetVtxDistZ(Double_t vtx_dist_z){
  k_vtx_z = vtx_dist_z;
}

void KElectron::SetLeadVtxDistXY(Double_t lvtx_dist_xy){
  k_lvtx_xy= lvtx_dist_xy;
}
void KElectron::SetLeadVtxDistZ(Double_t lvtx_dist_z){
  k_lvtx_z = lvtx_dist_z;
}


void KElectron::SetTrkIsoDR03(Double_t trkiso){  
  k_trkiso_03  = trkiso;
}
void KElectron::SetECalIsoDR03(Double_t ecaliso){  
  k_ecaliso_03 = ecaliso;
}
void KElectron::SetHCalIsoDR03(Double_t hcaliso){  
  k_hcaliso_03 = hcaliso;
}


void KElectron::SetTrkIsoDR04(Double_t trkiso){
  k_trkiso_04  = trkiso;
}
void KElectron::SetECalIsoDR04(Double_t ecaliso){
  k_ecaliso_04 = ecaliso;
}
void KElectron::SetHCalIsoDR04(Double_t hcaliso){
  k_hcaliso_04 = hcaliso;
}



void KElectron::SetPFChargedHadronIso03(Double_t pf_ch_03){
  k_pf_chargedhad_iso03= pf_ch_03;
}

void KElectron::SetPFPhotonIso03(Double_t pf_ph_03){
  k_pf_photon_iso03 = pf_ph_03;
}

void KElectron::SetPFNeutralHadronIso03(Double_t pf_ne_03){
  k_pf_neutral_iso03 = pf_ne_03;
}

void KElectron::SetPFChargedHadronIso04(Double_t pf_ch_03){
  k_pf_chargedhad_iso04 = pf_ch_03;
}

void KElectron::SetPFPhotonIso04(Double_t pf_ph_03){
  k_pf_photon_iso04= pf_ph_03;
}

void KElectron::SetPFNeutralHadronIso04(Double_t pf_ne_03){
  k_pf_neutral_iso04 = pf_ne_03;
}

void KElectron::SetGsfCtfScPixCharge(bool gsfctfscpix_ch){
  k_gsf_ctscpix_charge = gsfctfscpix_ch;
}
void KElectron::SetGsfScPixCharge(bool gsfscpix_ch){
  k_gsf_scpix_charge = gsfscpix_ch;
}
void KElectron::SetGsfCtfCharge(bool gsfctf_ch){
  k_gsf_ct_charge = gsfctf_ch;
}



void KElectron::SetMissingHits(Int_t missinghits){  
  k_missinghits = missinghits;
}

void KElectron::SetMissingLostHits(Int_t missinglosthits){
  k_missing_lost_hits = missinglosthits;
}

void KElectron::SetConvFitProb(double conv_fit_prob){
  k_convFitProb = conv_fit_prob;
}
void KElectron::SetNBrems(double nbrems){
  k_nbrems = nbrems;
}
void KElectron::SetFBrem(double fbrem){
  k_fbrem = fbrem;
}

void KElectron::SetCotTheta(double theta){
  k_cottheta=theta;
}
void KElectron::SetElDist(double dist){

  k_eldist=dist;
}

void KElectron::SetHasMatchedConvPhot(Bool_t hasmatchConvPhot){  
  k_hasmatchconvphot = hasmatchConvPhot;
}
void KElectron::SetDeltaEtaTrkSC(Double_t delta_etatrkSC){  
  k_delta_etatrkSC = delta_etatrkSC;
}
void KElectron::SetDeltaPhiTrkSC(Double_t delta_phitrkSC){  
  k_delta_phitrkSC = delta_phitrkSC;
}
 
void KElectron::SetSigmaIEtaIEta(Double_t sigmaIEtaIEta){
  k_sigmaIEtaIEta = sigmaIEtaIEta;
}
void KElectron::SetHoE(Double_t hoe){  
  k_hoe = hoe;
}
void KElectron::SetcaloEnergy(Double_t caloE){  
  k_caloE = caloE;
}

void KElectron::SetESuperClusterOverP(Double_t cluster_over_p){  
  k_E_cluster_over_p = cluster_over_p;
}

void KElectron::SetTrkVx(Double_t trkvx){  
  k_trkvx = trkvx;
}
void KElectron::SetTrkVy(Double_t trkvy){  
  k_trkvy = trkvy;
}
void KElectron::SetTrkVz(Double_t trkvz){  
  k_trkvz = trkvz;
}

void KElectron::SetShiftedEUp(Double_t Eup){
  k_pt_shifted_up= Eup;
}

void KElectron::SetShiftedEDown(Double_t Edown){
  k_pt_shifted_down= Edown;
}

void KElectron::SetShiftedExUp(Double_t Exup){
  k_px_shifted_up= Exup;
}

void KElectron::SetShiftedExDown(Double_t Exdown){
  k_px_shifted_down= Exdown;
}

void KElectron::SetShiftedEyUp(Double_t Eyup){
  k_py_shifted_up= Eyup;
}

void KElectron::SetShiftedEyDown(Double_t Eydown){
  k_py_shifted_down= Eydown;
}



void KElectron::SetElectronMatchedGenPt(Double_t pt){
  k_matched_gen_pt = pt;
}
void KElectron::SetElectronMatchedGenEta(Double_t eta){
  k_matched_gen_eta = eta;
}
void KElectron::SetElectronMatchedGenPhi(Double_t phi){
  k_matched_gen_phi = phi;
}

void KElectron::SetHLTDoubleElMatched(bool match){
  k_diel_trig_match= match;
}

void KElectron::SetHLTSingleElMatched8(bool match){
  k_el_trig_match8= match;
}

void KElectron::SetHLTSingleElMatched17(bool match){
  k_el_trig_match17= match;
}


void KElectron::SetHLTSingleElWP80Matched(bool match){
  k_elwp80_trig_match= match;
}

void KElectron::SetHLTEMuMatched8(bool match){
  k_emu_trig_match8= match;
}

void KElectron::SetHLTEMuMatched17(bool match){
  k_emu_trig_match17= match;
}


float KElectron::RelIso03( float rho, float elpt){
  double effective_area_eta_minimums    [7] = { 0.000, 1.000, 1.479, 2.000, 2.200, 2.300, 2.400 };
  double effective_area_eta_maximums    [7] = { 1.000, 1.479, 2.000, 2.200, 2.300, 2.400, 999.0 };
  double effective_areas_03             [7] = { 0.130, 0.140, 0.07, 0.09, 0.110, 0.110, 0.140 };
  double effective_area_03  = 0.0;
  
  for (int i = 0; i < 7; ++i ){
    double bin_minimum = effective_area_eta_minimums[i];
    double bin_maximum = effective_area_eta_maximums[i];
    if ( fabs(k_sceta) >= bin_minimum && fabs(k_sceta) < bin_maximum ) {
      effective_area_03 = effective_areas_03 [i];
    }
  }
  double egamma_pfiso_03 = k_pf_chargedhad_iso03  + std::max ( k_pf_photon_iso03 + k_pf_neutral_iso03 - ( rho * effective_area_03 ), 0.0 );
  egamma_pfiso_03 = egamma_pfiso_03/   elpt;
  return egamma_pfiso_03;
}


float KElectron::RelIso04( float rho, float elpt){
  double effective_area_eta_minimums    [7] = { 0.000, 1.000, 1.479, 2.000, 2.200, 2.300, 2.400 };
  double effective_area_eta_maximums    [7] = { 1.000, 1.479, 2.000, 2.200, 2.300, 2.400, 999.0 };

  double effective_areas_04             [7] = {0.208, 0.209, 0.115, 0.143, 0.183, 0.194, 0.261};
  double effective_area_04  = 0.0;

  for (int i = 0; i < 7; ++i ){
    double bin_minimum = effective_area_eta_minimums[i];
    double bin_maximum = effective_area_eta_maximums[i];
    if ( fabs(k_sceta) >= bin_minimum && fabs(k_sceta) < bin_maximum ) {
      effective_area_04 = effective_areas_04 [i];
    }
  }
  double egamma_pfiso_04 = k_pf_chargedhad_iso04  + std::max ( k_pf_photon_iso04 + k_pf_neutral_iso04 - ( rho * effective_area_04 ), 0.0 );
  egamma_pfiso_04 = egamma_pfiso_04/   elpt;
  return egamma_pfiso_04;
}
