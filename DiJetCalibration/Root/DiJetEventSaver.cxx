#include "DiJetCalibration/DiJetEventSaver.h"
#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"
#include "TopConfiguration/TopConfig.h"
#include "TopEvent/EventTools.h"

#include <iostream>

namespace top{
  ///-- Constrcutor --///
  DiJetEventSaver::DiJetEventSaver():
    m_trigDecTool("Trig::TrigDecisionTool"),
    m_BTS_DL1(),
    m_BTS_MV2c10()
    //m_BTS("BTaggingSelectionTool::BTaggingSelectionTool")
    //m_HLT_j15(-999),
    //m_eve_HLT_j15_ps(-999)
  {
    branchFilters().push_back(std::bind(&getBranchStatus, std::placeholders::_1, std::placeholders::_2));
    m_jet_truthflavExtended = std::vector<int>();
    m_jet_MV2c10Flip = std::vector<float>();
    m_jet_DL1Flip_pb = std::vector<float>();
    m_jet_DL1Flip_pc = std::vector<float>();
    m_jet_DL1Flip_pu = std::vector<float>();
    m_jet_SMT_discriminant = std::vector<float>();
    m_jet_SV1_masssvx = std::vector<float>();
    m_jet_SV1_Lxy = std::vector<float>();
    m_jet_SV1_L3d = std::vector<float>();
    //m_jet_DL1_h = std::vector<float>();
    m_jet_DL1Flip = std::vector<float>();
    //trig_ps = std::map<std::string, std::pair<int,float>>();
    m_trigger = std::map<std::string, Int_t>();
    m_trigger_ps = std::map<std::string, Float_t>();
    
    //m_jet_tagWeightBin_DL1_Continuous_h = std::vector<int>();
    m_jet_tagWeightBin_DL1Flip_Continuous = std::vector<int>();

    //m_jet_tagWeightBin_MV2c10_Continuous_h = std::vector<int>();
    m_jet_tagWeightBin_MV2c10Flip_Continuous = std::vector<int>();
  }

  ///-- initialize - done once at the start of a job before the loop over events --///
  void DiJetEventSaver::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches)
  {

    std::cout<< "in DiJetEventSaver::initialize" << std::endl;
    m_useJets = config->useJets();
    m_useTrackJets = config->useTrackJets();
    m_isMC = config->isMC();
    m_sgKeyJets = config->sgKeyJets();
    // description of TopConfig:
    //https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/TopPhys/xAOD/TopConfiguration/Root/TopConfig.cxx#L1203
    //std::cout<< "m_useJets "<< m_useJets << ", m_sgKeyJets "<< config->sgKeyJets()<< std::endl; 
    ///-- Let the base class do all the hard work --///
    ///-- It will setup TTrees for each systematic with a standard set of variables --///
    top::EventSaverFlatNtuple::initialize(config, file, extraBranches);
    

    //*
    m_BTS_DL1 = new BTaggingSelectionTool("DL1_cont_tool");
    top::check( m_BTS_DL1->setProperty("FlvTagCutDefinitionsFileName", "xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2018-02-09_v1.root")  ,"BTaggingSelectionTool failed to set FlvTagCutDefinitionsFileName!"); 
    top::check( m_BTS_DL1->setProperty("MaxEta", 2.5)  , "BTaggingSelectionTool failed to set MaxEta!"); 
    top::check( m_BTS_DL1->setProperty("MinPt", 20000.)  , "BTaggingSelectionTool failed to set MinPt!"); 
    top::check( m_BTS_DL1->setProperty("JetAuthor", m_sgKeyJets), "BTaggingSelectionTool failed to set JetAuthor!"); 
    top::check( m_BTS_DL1->setProperty("TaggerName", "DL1")  ,"BTaggingSelectionTool failed to set TaggerName!"); 
    top::check( m_BTS_DL1->setProperty("OperatingPoint", "Continuous")  ,"BTaggingSelectionTool failed to set WP!");
    top::check( m_BTS_DL1->initialize(), "BTaggingSelectionTool failed to initialize!"); 

    m_BTS_MV2c10 = new BTaggingSelectionTool("MV2c10_cont_tool");
    top::check( m_BTS_MV2c10->setProperty("FlvTagCutDefinitionsFileName", "xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2018-02-09_v1.root")  ,"BTaggingSelectionTool failed to set FlvTagCutDefinitionsFileName!"); 
    top::check( m_BTS_MV2c10->setProperty("MaxEta", 2.5)  , "BTaggingSelectionTool failed to set MaxEta!"); 
    top::check( m_BTS_MV2c10->setProperty("MinPt", 20000.)  , "BTaggingSelectionTool failed to set MinPt!"); 
    top::check( m_BTS_MV2c10->setProperty("JetAuthor", m_sgKeyJets), "BTaggingSelectionTool failed to set JetAuthor!"); 
    top::check( m_BTS_MV2c10->setProperty("TaggerName", "MV2c10")  ,"BTaggingSelectionTool failed to set TaggerName!"); 
    top::check( m_BTS_MV2c10->setProperty("OperatingPoint", "Continuous")  ,"BTaggingSelectionTool failed to set WP!");
    top::check( m_BTS_MV2c10->initialize(), "BTaggingSelectionTool failed to initialize!"); 

    //*/

    ///-- Loop over the systematic TTrees and add the custom variables --///
    for (auto systematicTree : treeManagers()) {
      std::cout<< "     in systematicTree loop" << std::endl;
      systematicTree->makeOutputVariable(m_jet_truthflavExtended, "jet_truthflavExtended");
      systematicTree->makeOutputVariable(m_jet_MV2c10Flip, "jet_mv2c10Flip");
      //systematicTree->makeOutputVariable(m_jet_DL1_h, "jet_DL1_h");
      systematicTree->makeOutputVariable(m_jet_DL1Flip, "jet_DL1Flip");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pb, "jet_DL1Flip_pb");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pc, "jet_DL1Flip_pc");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pu, "jet_DL1Flip_pu");
      systematicTree->makeOutputVariable(m_jet_SMT_discriminant, "jet_SMT_discriminant");
      systematicTree->makeOutputVariable(m_jet_SV1_masssvx, "jet_SV1_masssvx");
      systematicTree->makeOutputVariable(m_jet_SV1_Lxy, "jet_SV1_Lxy");
      systematicTree->makeOutputVariable(m_jet_SV1_L3d, "jet_SV1_L3d");

      //systematicTree->makeOutputVariable(m_jet_tagWeightBin_DL1_Continuous_h, "jet_tagWeightBin_DL1_Continuous_h");
      systematicTree->makeOutputVariable(m_jet_tagWeightBin_DL1Flip_Continuous, "jet_tagWeightBin_DL1Flip_Continuous");

      //systematicTree->makeOutputVariable(m_jet_tagWeightBin_MV2c10_Continuous_h, "jet_tagWeightBin_MV2c10_Continuous_h");
      systematicTree->makeOutputVariable(m_jet_tagWeightBin_MV2c10Flip_Continuous, "jet_tagWeightBin_MV2c10Flip_Continuous");
      
      for (auto &trigName : bin_trigger) {
	const std::string trig_name = trigName; 
	const std::string trig_name_ps = "eve_"+trigName+"_ps"; 
	systematicTree->makeOutputVariable(m_trigger[trigName],trig_name);
	systematicTree->makeOutputVariable(m_trigger_ps[trigName],trig_name_ps);
      }



    }
  }

  ///-- saveEvent - run for every systematic and every event --///
  void DiJetEventSaver::saveEvent(const top::Event& event)
  {
    ///-- fill them --///

    if (m_useJets) {

      m_jet_truthflavExtended.resize(event.m_jets.size());
      m_jet_MV2c10Flip.resize(event.m_jets.size());
      m_jet_DL1Flip_pb.resize(event.m_jets.size());
      m_jet_DL1Flip_pc.resize(event.m_jets.size());
      m_jet_DL1Flip_pu.resize(event.m_jets.size());
      m_jet_SMT_discriminant.resize(event.m_jets.size());
      m_jet_SV1_masssvx.resize(event.m_jets.size());
      m_jet_SV1_Lxy.resize(event.m_jets.size());
      m_jet_SV1_L3d.resize(event.m_jets.size());

      //m_jet_DL1_h.resize(event.m_jets.size());
      m_jet_DL1Flip.resize(event.m_jets.size());
      //m_jet_tagWeightBin_DL1_Continuous_h.resize(event.m_jets.size());
      m_jet_tagWeightBin_DL1Flip_Continuous.resize(event.m_jets.size());
      //m_jet_tagWeightBin_MV2c10_Continuous_h.resize(event.m_jets.size());
      m_jet_tagWeightBin_MV2c10Flip_Continuous.resize(event.m_jets.size());

      //std::cout<< " ==================== event number  = " << event.m_info->auxdata<unsigned int>("runNumber") << std::endl;

      //const std::string bin_trigger[2] = {"HLT_j15", "HLT_j25"}; //, "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j420", "HLT_j420", "HLT_j420"};
      //for(int i=0;i<bin_trigger.size();i++){	
      //	std::cout<< " ==================== bin_trigger  = " << bin_trigger[i] << std::endl;      
      //}

      // int to_print=0;
      // if(m_trigDecTool->getChainGroup("HLT_j15")->isPassed(TrigDefs::Physics) || m_trigDecTool->getChainGroup("HLT_j25")->isPassed(TrigDefs::Physics)) to_print=1;

      // if(to_print==1) {std::cout<< " +++ new ev  ; reference j15/j25 hardcode =  "<< m_trigDecTool->getChainGroup("HLT_j15")->isPassed(TrigDefs::Physics)<< " /"<< m_trigDecTool->getChainGroup("HLT_j25")->isPassed(TrigDefs::Physics)
      // 	       << ";  ps 15/25 = "<<
      // 	       m_trigDecTool->getChainGroup("HLT_j15")->getPrescale(TrigDefs::Physics)
      // 	       <<"/"<<
      // 		   m_trigDecTool->getChainGroup("HLT_j25")->getPrescale(TrigDefs::Physics)
      // 	       << std::endl;   
      // }

      for (auto &trigName : bin_trigger) {
	m_trigger[trigName]=0;
	m_trigger_ps[trigName]=0;
      }
      int trig_check=0;
      for (auto &kv : m_trigger) {
      	const std::string trigName = kv.first;
      	kv.second = m_trigDecTool->getChainGroup(trigName)->isPassed(TrigDefs::Physics);
	//check that at least one trigger fired
	if(kv.second!=0) trig_check++;
      }

      if( trig_check==0) return ;

      for (auto &kv : m_trigger_ps) {
      	const std::string trigName = kv.first;
      	kv.second = m_trigDecTool->getChainGroup(trigName)->getPrescale(TrigDefs::Physics);
      }
      
      // TriggerDecision and pre-scales by hand
      //m_HLT_j15 = m_trigDecTool->getChainGroup("HLT_j15")->isPassed(TrigDefs::Physics);
      //m_eve_HLT_j15_ps = m_trigDecTool->getChainGroup("HLT_j15")->getPrescale(TrigDefs::Physics);
      
      //float fc=0.08;
      double tagweight=0;	double tagweightF=0;
      unsigned int i(0);
      for (const auto* const jetPtr : event.m_jets) {
	m_jet_truthflavExtended[i] = -999;
	m_jet_MV2c10Flip[i] = -999;
	m_jet_DL1Flip_pb[i] = -999;
	m_jet_DL1Flip_pc[i] = -999;
	m_jet_DL1Flip_pu[i] = -999;
	m_jet_SMT_discriminant[i] = -999;
	m_jet_SV1_masssvx[i] = -999;
	m_jet_SV1_Lxy[i] = -999;
	m_jet_SV1_L3d[i] = -999;
	//m_jet_DL1_h[i] = -999;
	m_jet_DL1Flip[i] = -999;
	  
	//m_jet_tagWeightBin_DL1_Continuous_h[i]=-999;
	m_jet_tagWeightBin_DL1Flip_Continuous[i]=-999;
	//m_jet_tagWeightBin_MV2c10_Continuous_h[i]=-999;
	m_jet_tagWeightBin_MV2c10Flip_Continuous[i]=-999;

	// Official jet truth frlavour:
	// jet_truthflav = HadronConeExclTruthLabelID
	//https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/TopPhys/xAOD/TopAnalysis/Root/EventSaverFlatNtuple.cxx#L2046
	// recommendations: https://indico.cern.ch/event/737758/contributions/3044248/attachments/1683499/2705963/news_ftcalib_09072018.pdf
	if (m_isMC) {
	  if(jetPtr->isAvailable<int>("HadronConeExclExtendedTruthLabelID")){
	    jetPtr->getAttribute("HadronConeExclExtendedTruthLabelID", m_jet_truthflavExtended[i]);
	  }
	}

	// if(jetPtr->isAvailable<float>("AnalysisTop_JVT")){
	//   std::cout<< "--JVT ["<<i<<"]  = " << jetPtr->auxdataConst<float>("AnalysisTop_JVT") << std::endl; 
	//   m_jet_JVT[i] = jetPtr->auxdataConst<float>("AnalysisTop_JVT") ;
	// }
	  
	if(jetPtr->btagging()->isAvailable<double>("MV2c10Flip_discriminant")) m_jet_MV2c10Flip[i] = jetPtr->btagging()->auxdata<double>("MV2c10Flip_discriminant");
	if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pb")) m_jet_DL1Flip_pb[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pb");
        if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pc")) m_jet_DL1Flip_pc[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pc");
        if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pu")) m_jet_DL1Flip_pu[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pu");


	// access Continuous info for flipped version of taggers:

	//bts code:
	//https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency/Root/BTaggingSelectionTool.cxx
	// way through BTS:


        if(jetPtr->btagging()->isAvailable<double>("SMT_discriminant")) m_jet_SMT_discriminant[i] = jetPtr->btagging()->auxdata<double>("SMT_discriminant");
        if(jetPtr->btagging()->isAvailable<float>("SV1_masssvx")) m_jet_SV1_masssvx[i] = jetPtr->btagging()->auxdata<float>("SV1_masssvx");
        if(jetPtr->btagging()->isAvailable<float>("SV1_Lxy")) m_jet_SV1_Lxy[i] = jetPtr->btagging()->auxdata<float>("SV1_Lxy");
        if(jetPtr->btagging()->isAvailable<float>("SV1_L3d")) m_jet_SV1_L3d[i] = jetPtr->btagging()->auxdata<float>("SV1_L3d");


	/*
	double jet_DL1_pb=0;	double jet_DL1_pc=0;	double jet_DL1_pu=0;
	if(jetPtr->btagging()->isAvailable<double>("DL1_pb")) jet_DL1_pb = jetPtr->btagging()->auxdata<double>("DL1_pb");
        if(jetPtr->btagging()->isAvailable<double>("DL1_pc")) jet_DL1_pc = jetPtr->btagging()->auxdata<double>("DL1_pc");
        if(jetPtr->btagging()->isAvailable<double>("DL1_pu")) jet_DL1_pu = jetPtr->btagging()->auxdata<double>("DL1_pu");
	//*/
	

	//*
	tagweight=-999;	
	tagweightF=-999;
	//with tool
	top::check(m_BTS_DL1->getTaggerWeight( *jetPtr ,tagweight),"can't retrieve getTaggerWeight");
	//m_jet_DL1_h[i]=tagweight;
	// by hand
	//m_jet_DL1_h[i] = log(jet_DL1_pb / (fc* jet_DL1_pc+(1-fc)* jet_DL1_pu) );

	//with tool
	top::check(m_BTS_DL1->getTaggerWeight( jetPtr->btagging()->auxdata<double>("DL1Flip_pb"),jetPtr->btagging()->auxdata<double>("DL1Flip_pc"),jetPtr->btagging()->auxdata<double>("DL1Flip_pu"),tagweightF),"can't retrieve getTaggerWeight");
	m_jet_DL1Flip[i]=tagweightF;
	// by hand 
	//m_jet_DL1Flip[i]= log(jetPtr->btagging()->auxdata<double>("DL1Flip_pb") / (fc* jetPtr->btagging()->auxdata<double>("DL1Flip_pc")+(1-fc)* jetPtr->btagging()->auxdata<double>("DL1Flip_pu")) );

	//top::check(m_jet_tagWeightBin_DL1_Continuous_h[i]=m_BTS_DL1->getQuantile( *jetPtr), "can't retrieve getQuantile");
	//validate that (*jetPtr) equal to (pt,eta,weight) accessors on nominal DL1;
	//top::check(m_jet_tagWeightBin_DL1_Continuous_h2[i]=m_BTS_DL1->getQuantile( jetPtr->pt(), jetPtr->eta(),m_jet_DL1_h[i]),"can't retrieve getTaggerWeight");
	top::check(m_jet_tagWeightBin_DL1Flip_Continuous[i]=m_BTS_DL1->getQuantile( jetPtr->pt(), jetPtr->eta(),m_jet_DL1Flip[i]),"can't retrieve getTaggerWeight");
	


	//top::check(m_jet_tagWeightBin_MV2c10_Continuous_h[i]=m_BTS_MV2c10->getQuantile( *jetPtr), "can't retrieve getQuantile");
	top::check(m_jet_tagWeightBin_MV2c10Flip_Continuous[i]=m_BTS_MV2c10->getQuantile( jetPtr->pt(), jetPtr->eta(),m_jet_MV2c10Flip[i]),"can't retrieve getTaggerWeight");

	//*/
	++i;

	/* Example of getTaggerWeight usage
	  DL1Flip_tagweight = -999;
	  BTSTool[24]->getTaggerWeight(DL1Flip_pb,DL1Flip_pc,DL1Flip_pu, DL1Flip_tagweight);
	  Props::DL1Flip_w.set( outJet , DL1Flip_tagweight );	  
	//*/
      }

    }


    ///-- Let the base class do all the hard work --///
    top::EventSaverFlatNtuple::saveEvent(event);
  }

  //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HTop2018WorkshopAnalysisTopTutorial#7.3%20%20Filtering%20out%20unwanted%20bran
  int DiJetEventSaver::getBranchStatus(top::TreeManager const * treeManager, std::string const & variableName) {
    //we can use the treeManager to remove these branches only for some of the TTrees
    // e.g. add a condition like if (m_config->systematicName(treeManager->name() != "nominal")
    //if (variableName == "el_pt") return 0;
    if (variableName.find("el_")!=std::string::npos || variableName.find("mu_")!=std::string::npos) return 0;
    if (variableName == "jet_mv2c00" || variableName == "jet_mv2c20") return 0;
    if (variableName.find("jet_MV2")!=std::string::npos) return 0;
    if (variableName.find("jet_DL1r")!=std::string::npos) return 0;
    if (variableName.find("weight_lepton")!=std::string::npos || variableName.find("weight_oldTrigger")!=std::string::npos || variableName.find("weight_indiv")!=std::string::npos) return 0;
    return -1;
  }
}

//  LocalWords:  getTaggerWeight
