#include "DiJetCalibration/DiJetEventSaver.h"
#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"
#include "TopConfiguration/TopConfig.h"

#include <iostream>

namespace top{
  ///-- Constrcutor --///
  DiJetEventSaver::DiJetEventSaver():
    m_trigDecTool("Trig::TrigDecisionTool")
    //m_BTS("BTaggingSelectionTool::BTaggingSelectionTool")
    //m_HLT_j15(-999),
    //m_eve_HLT_j15_ps(-999)
  {
    branchFilters().push_back(std::bind(&getBranchStatus, std::placeholders::_1, std::placeholders::_2));
    m_jet_truthflavExtended = std::vector<int>();
    m_jet_MV2c10Flip = std::vector<double>();
    m_jet_DL1Flip_pb = std::vector<double>();
    m_jet_DL1Flip_pc = std::vector<double>();
    m_jet_DL1Flip_pu = std::vector<double>();
    m_jet_SMT_discriminant = std::vector<double>();
    m_jet_SV1_masssvx = std::vector<float>();
    m_jet_SV1_Lxy = std::vector<float>();
    m_jet_SV1_L3d = std::vector<float>();
    m_jet_DL1_h = std::vector<double>();
    m_jet_DL1Flip = std::vector<double>();
    //trig_ps = std::map<std::string, std::pair<int,float>>();
    m_trigger = std::map<std::string, Int_t>();
    m_trigger_ps = std::map<std::string, Float_t>();
    
  }

  ///-- initialize - done once at the start of a job before the loop over events --///
  void DiJetEventSaver::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches)
  {

    std::cout<< "in DiJetEventSaver::initialize" << std::endl;
    m_useJets = config->useJets();
    m_useTrackJets = config->useTrackJets();
    m_isMC = config->isMC();

    ///-- Let the base class do all the hard work --///
    ///-- It will setup TTrees for each systematic with a standard set of variables --///
    top::EventSaverFlatNtuple::initialize(config, file, extraBranches);
    
    // BTaggingSelectionTool * m_BTS = new BTaggingSelectionTool("dl1flip");

    // m_BTS->setProperty("MaxEta", 2.5);
    // m_BTS->setProperty("MinPt", 20000.);
    // m_BTS->setProperty("JetAuthor", "AntiKt4EMTopoJets");
    // m_BTS->setProperty("TaggerName", "DL1Flip" );
    // m_BTS->setProperty("FlvTagCutDefinitionsFileName", "xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2018-02-09_v1.root" );
    // m_BTS->setProperty("OperatingPoint", "FixedCutBEff_85" );

    // m_BTS->initialize();
    // m_BTS->print();


    ///-- Loop over the systematic TTrees and add the custom variables --///
    for (auto systematicTree : treeManagers()) {
      std::cout<< "     in systematicTree loop" << std::endl;
      systematicTree->makeOutputVariable(m_jet_truthflavExtended, "jet_truthflavExtended");
      systematicTree->makeOutputVariable(m_jet_MV2c10Flip, "jet_mv2c10Flip");
      systematicTree->makeOutputVariable(m_jet_DL1_h, "jet_DL1_h");
      systematicTree->makeOutputVariable(m_jet_DL1Flip, "jet_DL1Flip");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pb, "jet_DL1Flip_pb");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pc, "jet_DL1Flip_pc");
      systematicTree->makeOutputVariable(m_jet_DL1Flip_pu, "jet_DL1Flip_pu");
      systematicTree->makeOutputVariable(m_jet_SMT_discriminant, "jet_SMT_discriminant");
      systematicTree->makeOutputVariable(m_jet_SV1_masssvx, "jet_SV1_masssvx");
      systematicTree->makeOutputVariable(m_jet_SV1_Lxy, "jet_SV1_Lxy");
      systematicTree->makeOutputVariable(m_jet_SV1_L3d, "jet_SV1_L3d");

      
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

      m_jet_DL1_h.resize(event.m_jets.size());
      m_jet_DL1Flip.resize(event.m_jets.size());

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

      //m_HLT_j15 = m_trigDecTool->getChainGroup("HLT_j15")->isPassed(TrigDefs::Physics);
      //m_eve_HLT_j15_ps = m_trigDecTool->getChainGroup("HLT_j15")->getPrescale(TrigDefs::Physics);
      
      double fc=0.08;
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
	m_jet_DL1_h[i] = -999;
	m_jet_DL1Flip[i] = -999;
	  
	// Official jet truth frlavour:
	// jet_truthflav = HadronConeExclTruthLabelID
	//https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/TopPhys/xAOD/TopAnalysis/Root/EventSaverFlatNtuple.cxx#L2046
	// recommendations: https://indico.cern.ch/event/737758/contributions/3044248/attachments/1683499/2705963/news_ftcalib_09072018.pdf
	if (m_isMC) {
	  if(jetPtr->isAvailable<int>("HadronConeExclExtendedTruthLabelID")){
	    jetPtr->getAttribute("HadronConeExclExtendedTruthLabelID", m_jet_truthflavExtended[i]);
	  }
	}
	  
	//m_jet_DL1_h[i]= 999;

	// if(jetPtr->isAvailable<float>("AnalysisTop_JVT")){
	//   std::cout<< "--JVT ["<<i<<"]  = " << jetPtr->auxdataConst<float>("AnalysisTop_JVT") << std::endl; 
	//   m_jet_JVT[i] = jetPtr->auxdataConst<float>("AnalysisTop_JVT") ;
	// }
	  
	if(jetPtr->btagging()->isAvailable<double>("MV2c10Flip_discriminant")) m_jet_MV2c10Flip[i] = jetPtr->btagging()->auxdata<double>("MV2c10Flip_discriminant");
	if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pb")) m_jet_DL1Flip_pb[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pb");
        if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pc")) m_jet_DL1Flip_pc[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pc");
        if(jetPtr->btagging()->isAvailable<double>("DL1Flip_pu")) m_jet_DL1Flip_pu[i] = jetPtr->btagging()->auxdata<double>("DL1Flip_pu");

	
	//std::cout << " = == = = pb " << m_jet_DL1Flip_pb[i] << ", pc = "<<  m_jet_DL1Flip_pc[i] << ",  pu= "<<m_jet_DL1Flip_pu[i]<< ",  statement = " << m_jet_DL1Flip_pb[i] / (fc* m_jet_DL1Flip_pc[i]+(1-fc)* m_jet_DL1Flip_pu[i]) <<  ",  log (stat) = "<< log(m_jet_DL1Flip_pb[i] / (fc* m_jet_DL1Flip_pc[i]+(1-fc)* m_jet_DL1Flip_pu[i]) )<< std::endl;
	// double flipttt=0;
	m_jet_DL1Flip[i]= log(jetPtr->btagging()->auxdata<double>("DL1Flip_pb") / (fc* jetPtr->btagging()->auxdata<double>("DL1Flip_pc")+(1-fc)* jetPtr->btagging()->auxdata<double>("DL1Flip_pu")) );
	
	

        if(jetPtr->btagging()->isAvailable<double>("SMT_discriminant")) m_jet_SMT_discriminant[i] = jetPtr->btagging()->auxdata<double>("SMT_discriminant");
        if(jetPtr->btagging()->isAvailable<float>("SV1_masssvx")) m_jet_SV1_masssvx[i] = jetPtr->btagging()->auxdata<float>("SV1_masssvx");
        if(jetPtr->btagging()->isAvailable<float>("SV1_Lxy")) m_jet_SV1_Lxy[i] = jetPtr->btagging()->auxdata<float>("SV1_Lxy");
        if(jetPtr->btagging()->isAvailable<float>("SV1_L3d")) m_jet_SV1_L3d[i] = jetPtr->btagging()->auxdata<float>("SV1_L3d");

	// const xAOD::BTagging* btag(nullptr);
	// btag = jetPtr->btagging();
	// double DL1Flip_tagweight=0;
	// double DL1Flip_tagweight_get=0;
	// double DL1Flip_tagweight_hand=0;
	// double fc=0.08;
	// DL1Flip_tagweight_hand = log(m_jet_DL1Flip_pb[i]/ (fc* m_jet_DL1Flip_pc[i]+(1-fc)* m_jet_DL1Flip_pu[i]) );
	// double mvx = -999;
	// btag->MVx_discriminant("DL1Flip", mvx);
	// DL1Flip_tagweight = mvx;

	// //btag->getTaggerWeight(m_jet_DL1Flip_pb[i],m_jet_DL1Flip_pc[i],m_jet_DL1Flip_pu[i],DL1Flip_tagweight_get);
	// std::cout << "jet N = << "<<i<<";  DL1Flip hand = "<< DL1Flip_tagweight_hand  << ",  - - - btag equal to "<< DL1Flip_tagweight << ", tool using getTaggerWeight = "<< DL1Flip_tagweight_get << std::endl;
	// //m_bts->getTaggerWeight(m_jet_DL1Flip_pb[i],m_jet_DL1Flip_pc[i],m_jet_DL1Flip_pu[i], DL1Flip_tagweight)
	//std::cout << " DL1Flip_tagweight = "<<  jetPtr->btagging()->getTaggerWeight(m_jet_DL1Flip_pb[i],m_jet_DL1Flip_pc[i],m_jet_DL1Flip_pu[i], DL1Flip_tagweight)<< std::endl;

	//test
	double jet_DL1_pb=0;	double jet_DL1_pc=0;	double jet_DL1_pu=0;
	if(jetPtr->btagging()->isAvailable<double>("DL1_pb")) jet_DL1_pb = jetPtr->btagging()->auxdata<double>("DL1_pb");
        if(jetPtr->btagging()->isAvailable<double>("DL1_pc")) jet_DL1_pc = jetPtr->btagging()->auxdata<double>("DL1_pc");
        if(jetPtr->btagging()->isAvailable<double>("DL1_pu")) jet_DL1_pu = jetPtr->btagging()->auxdata<double>("DL1_pu");

	m_jet_DL1_h[i] = log(jet_DL1_pb / (fc* jet_DL1_pc+(1-fc)* jet_DL1_pu) );

	//std::cout << " DL1Flip" << m_jet_DL1Flip[i] << ", DL1 = "<<  m_jet_DL1_h[i]<< std::endl;

	++i;

	/*
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
    return -1;
  }
}
