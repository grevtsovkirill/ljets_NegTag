#ifndef DIJETCALIBRATION_DIJETEVENTSAVER_H
#define DIJETCALIBRATION_DIJETEVENTSAVER_H

#include <vector>
#include "TopAnalysis/EventSaverFlatNtuple.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

/**
 * This class extends the flat ntuple to include your own variables
 *
 * It inherits from top::EventSaverFlatNtuple, which will be doing all the hard work
 *
 */

namespace top{
  class DiJetEventSaver : public top::EventSaverFlatNtuple {
    public:
      ///-- Default constrcutor with no arguments - needed for ROOT --///
      DiJetEventSaver();
      ///-- Destructor does nothing --///
      virtual ~DiJetEventSaver(){}

      ///-- initialize function for top::EventSaverFlatNtuple --///
      ///-- We will be setting up out custom variables here --///
      virtual void initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches) override;

      ///-- Keep the asg::AsgTool happy --///
      virtual StatusCode initialize(){return StatusCode::SUCCESS;}

      ///-- saveEvent function for top::EventSaverFlatNtuple --///
      ///-- We will be setting our custom variables on a per-event basis --///
      virtual void saveEvent(const top::Event& event) override;

      ToolHandle<Trig::TrigDecisionTool>     m_trigDecTool;
      //ToolHandle<BTaggingSelectionTool*>     m_BTS;
      //std::map<TString, BTaggingSelectionTool*> m_BTS;

    private:

      ///
      const std::vector<std::string> bin_trigger= {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j420"};
      bool m_isMC;
      bool m_useJets;
      bool m_useTrackJets;

      ///-- Some additional custom variables for the output --///
      std::vector<int> m_jet_truthflavExtended;
      std::vector<double> m_jet_MV2c10Flip;

  
      std::vector<double> m_jet_DL1Flip_pb;
      std::vector<double> m_jet_DL1Flip_pc;
      std::vector<double> m_jet_DL1Flip_pu;

      std::vector<double> m_jet_SMT_discriminant;

      std::vector<float> m_jet_SV1_masssvx;
      std::vector<float> m_jet_SV1_Lxy;
      std::vector<float> m_jet_SV1_L3d;

      //New variables for dijet:
      std::vector<double> m_jet_DL1Flip;
      std::vector<double> m_jet_DL1_h;

      std::map<std::string, Int_t> m_trigger;
      std::map<std::string, Float_t> m_trigger_ps;
      //int m_HLT_j15;
      //float m_eve_HLT_j15_ps;

      ///-- Tell RootCore to build a dictionary (we need this) --///
      ClassDef(top::DiJetEventSaver, 0);
  };
}

#endif
