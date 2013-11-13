def makeConfigFile():
    
    config='{\n'
    config+='  string maindir = getenv("MAINDIR");\n'
    config+='  string includedir = getenv("INCLUDEDIR");\n'
    config+='  string includedir_snu = getenv("INCLUDEDIR_snu");\n'
    config+='  gSystem->AddIncludePath(("-I" + includedir).c_str());\n'
    config+='  gSystem->AddIncludePath(("-I" + includedir_snu).c_str());\n'
    config+='\n'
    config+='  gSystem->ChangeDirectory((maindir + "/core/").c_str());\n'
    config+='  gROOT->ProcessLine(".L Data.cc+g");\n'
    config+='  gSystem->ChangeDirectory((maindir + "/src/").c_str());\n'
    config+='  gROOT->ProcessLine(".L Reweight.cc+g");\n'
    config+='  gROOT->ProcessLine(".L AnalysisBase.cc+g");\n'
    config+='\n'
    
    config+='  gSystem->ChangeDirectory((maindir+ "/SNUTree/src/").c_str());\n'
    config+='  gROOT->ProcessLine(".L KParticle.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KJet.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KMuon.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KElectron.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KEvent.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KTruth.cc+g");\n'
    config+='  gROOT->ProcessLine(".L KTau.cc+g");\n'
    config+='\n'
    config+='  gSystem->ChangeDirectory((maindir+ "/Selection/").c_str());\n'
    config+='  gROOT->ProcessLine(".L SelectionFunctions.cc+g");\n'
    config+='\n'
    config+='  gSystem->ChangeDirectory((maindir + "/core/").c_str());\n'
    config+='  gROOT->ProcessLine(".L LQEvent.cc+g");\n'
    config+='  gROOT->ProcessLine(".L SNUTreeFiller.cc+g");\n'
    config+='  /// Selection Code compilation;\n'
    config+='  gSystem->ChangeDirectory((maindir+ "/Selection/").c_str());\n'
    config+='  gROOT->ProcessLine(".L BaseSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L GenSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L ElectronSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L MuonSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L TauSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L JetSelection.cc+g");\n'
    config+='  gROOT->ProcessLine(".L EventSelection.cc+g");\n'
    config+='\n'
    config+='  gSystem->ChangeDirectory((maindir + "/core/").c_str());\n'
    config+='  gROOT->ProcessLine(".L EventBase.cc+g");\n'
    config+='\n'
    config+='  gSystem->ChangeDirectory((maindir +("/Plotting/")).c_str());\n'
    config+='  /// Plotting code compilation\n'
    config+='  gROOT->ProcessLine(".L StdPlots.cc+g");\n'
    config+='  gROOT->ProcessLine(".L ElectronPlots.cc+g");\n'
    config+='  gROOT->ProcessLine(".L MuonPlots.cc+g");\n'
    config+='  gROOT->ProcessLine(".L JetPlots.cc+g");\n'
    config+='  gROOT->ProcessLine(".L SignalPlots.cc+g");\n'
    config+='\n'
    
    config+='  gSystem->ChangeDirectory((maindir + "/src/").c_str());\n'
    config+='  gROOT->ProcessLine(".L Analyzer.cc+g");\n'
    config+='  //gROOT->ProcessLine(".L Analyzer_Ele.cc+g");\n'
    config+='  //gROOT->ProcessLine(".L FakeRateCalculator.cc+g");\n'
    config+='  //gROOT->ProcessLine(".L FakeRateCalculator_Ele.cc+g");\n'
    config+='  //gROOT->ProcessLine(".L EfficiencyCalculator.cc+g");\n'
    config+='  gSystem->ChangeDirectory((maindir + "/core/").c_str());\n'
    config+='  gROOT->ProcessLine(".L ChainMaker.C+g");\n'
    config+='  //////////////////////////////////////////////////////////\n'
    config+='  //                          MC                          //\n'
    config+='  //////////////////////////////////////////////////////////\n'
    config+='\n'
    config+='  /// Signal samples\n'
    config+='  if (1) {\n'
    config+='\n'
    config+='      // Create Chains\n'
    config+='      TChain* chain50 = ChainMaker((maindir + "/scripts/filelist.txt"));\n'
    config+='\n' 
    config+='      Analyzer Pippo(Analyzer::ZTest);\n /// create analyzer class object'
    config+='      Pippo.Init(chain50);\n'
    config+='      Pippo.SetName("Majorana_50",1);\n'
    config+='      Pippo.SetTargetLumi(1.);\n'
    config+='      Pippo.SetWeight(1., 1.);\n'
    config+='      Pippo.Run();\n'
    config+=' }\n'
    config+='}\n'

    return config
