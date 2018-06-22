# ----------------------------------
# Importation for pyROOT
import ROOT,math

# ----------------------------------
# Input parameter
collection='AntiKt4EMTopoJets'
systematic_file='../config/systematics.txt'

# Dictionnaries, arrays and files

WP_list = {85, 77, 70, 60}
WP_dict = dict = { 85: 0.11, 77: 0.645, 70: 0.831, 60: 0.939} # MV2c10
tagger_list = {'MV2c10','DL1'}
tagger_list = {'MV2c10'}
pt_bins = [20, 60, 100, 200, 300, 500, 750, 1000, 3000]
abseta_bins = [0.0, 1.2, 2.5]

compaigne = 'a'
f_nom_stat = ROOT.TFile('tot_systematics/total_sf_'+compaigne+'.root') 
f_nom_eps = ROOT.TFile('tot_systematics/total_eps_d_'+compaigne+'.root') 

f_data_tot = ROOT.TFile('../NtupleReader/res/FlavourTagging_Nominal/data_'+compaigne+'.root')
f_data_subldg_tot = ROOT.TFile('../NtupleReader/res/subleadingjet/data_'+compaigne+'.root')

f_mc_frac = ROOT.TFile('../TemplateFit/template_fit.root.pTetaCorrections')
f_mc_subldg_frac = ROOT.TFile('../TemplateFit/template_fit.root.subldg.pTetaCorrections')

h_mc_frac = f_mc_frac.Get('h_fracl_mc')
h_mc_subldg_frac = f_mc_subldg_frac.Get('h_fracl_mc')


# Systematic list 
systematic_list = []
systematic_filelist = []
systematic_typelist = []
with open(systematic_file) as f_txt_syst:

   for line in f_txt_syst.readlines():
      if line[0]!='#' and line.split()[1]!='FlavourTagging_Nominal' and line.split()[2]!='bootstrap': 

         type_name = line.split()[2]
         systematic_name = line.split()[1]
         file_name = ''

         if type_name!='updown': 
            file_name = 'rel_systematics/rel_sf_' + systematic_name + '_'+compaigne+'.root' 
            systematic_name = systematic_name.replace('_up','')
         else                  : 
            if systematic_name.find('_up')==-1 and systematic_name.find('__1up')==-1: continue;
            elif systematic_name.find('conversions')!=-1:
               systematic_name = systematic_name.replace('up','')
               file_name = 'rel_systematics/rel_sf_' + systematic_name + '_'+compaigne+'.root' 
               if systematic_name.find('subleadingjet'): systematic_name = 'subleadingjet_conversions'
               else                                    : systematic_name = 'conversions' 
            else :
               systematic_name = systematic_name.replace('__1up','__1')
               file_name = 'rel_systematics/rel_sf_' + systematic_name + '_'+compaigne+'.root'
               systematic_name = systematic_name.replace('__1','')

         if systematic_name.find('FlavourTagging_')==-1: systematic_name = 'FT_EFF_' + systematic_name
         systematic_name = systematic_name.replace('FlavourTagging_','')

         systematic_list.append(systematic_name)
         systematic_filelist.append( ROOT.TFile(file_name) )
         systematic_typelist.append( type_name )

print systematic_list
print systematic_typelist

# ----------------------------------
# File builder - prelude
for iTAG in tagger_list:
   print iTAG

   for iWP in WP_list:
      file = open('CDI/negtag_v01-06_%s_WP%i.txt' % (iTAG, iWP),'w') 
   
      file.write('Analysis(negative_tags,light,%s,FixedCutBEff_%i,%s){\n' % (iTAG, iWP, collection))
      file.write('\n')
      file.write('        meta_data_s (Hadronization, Pythia8EvtGen)')
      file.write('\n')
      file.write('        meta_data_s (OperatingPoint, %f)\n' % WP_dict[iWP])
      file.write('\n')
      # ----------------------------------
      # File builder - eta loop
      for ieta in range(0,len(abseta_bins)-1):
         
         # get nominal and stat histograms
         h_nom = f_nom_stat.Get( 'sf_nom_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
         h_stat = f_nom_stat.Get( 'rel_stat_total_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
         
         h_eps_d = f_nom_eps.Get( 'eps_d_nom_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
         
         # pT loop
         for ipt in range(0,len(pt_bins)-1): 
            
            # get central value
            sf   = h_nom.GetBinContent(ipt+1) 
            stat = h_stat.GetBinContent(ipt+1)*100 # % 
            
            file.write('        bin(%i<pt<%i,%.1f<abseta<%.1f)\n' % (pt_bins[ipt], pt_bins[ipt+1], abseta_bins[ieta], abseta_bins[ieta+1]))
            file.write('        {\n')
            file.write('                central_value(%.2f,%.2f%%)\n' % (sf, stat))
            
            # for continuous b-tagging
            h_data_tot = f_data_tot.Get( 'w__pt%ieta%i_%s' % (ipt+1, ieta+1, iTAG) )
            h_data_subldg_tot = f_data_subldg_tot.Get( 'w__pt%ieta%i_%s' % (ipt+1, ieta+1, iTAG) )
            
            if ipt==0: 
               data_tot = h_data_subldg_tot.Integral()
               light_frac = h_mc_subldg_frac.GetBinContent(ipt+1,ieta+1)
            else: 
               data_tot = h_data_tot.Integral()
               light_frac = h_mc_frac.GetBinContent(ipt+1,ieta+1)
               
            #print data_tot, light_frac, h_eps_d.GetBinContent(ipt+1)
            N_jets_tot = data_tot*light_frac
            N_jets_tag = h_eps_d.GetBinContent(ipt+1)*N_jets_tot 
            N_jets_tot_unc = math.sqrt(N_jets_tot) # NOT % 
            N_jets_tag_unc = math.sqrt(N_jets_tag) # NOT %
 
            file.write('                meta_data(N_jets total,%.0f,%.1f)\n' % (N_jets_tot, N_jets_tot_unc) )
            file.write('                meta_data(N_jets taggued,%.0f,%.1f)\n' % (N_jets_tag, N_jets_tag_unc) )
               
            # systematics loop
            for isyst in range(0,len(systematic_list)):
                  
               syst_name_current = systematic_list[isyst]
               syst_name_current_up = syst_name_current.replace('subleadingjet_','')
               
               # skip subleadingjet_ systematic if not first pT bin 
               if syst_name_current.find("subleadingjet_")!=-1 and ipt!=0: continue
               
               # skip leading jet systematic if first pT bin
               if ipt==0 and (syst_name_current.find("JET_",0,11)!=-1 or
                              syst_name_current.find("PRW_",0,11)!=-1 or
                              syst_name_current.find("JVT",0,11)!=-1 or
                              syst_name_current=="FT_EFF_generator" or 
                              syst_name_current.find("FT_EFF_conversions",0,18)!=-1 or
                              syst_name_current.find("FT_EFF_hadronic",0,15)!=-1 or 
                              syst_name_current=="FT_EFF_longlivedparticles"!=-1 or
                              syst_name_current=="FT_EFF_d0smearing"!=-1 or 
                              syst_name_current=="FT_EFF_z0smearing"!=-1 or
                              syst_name_current=="FT_EFF_faketracks"!=-1 or 
                              syst_name_current=="FT_EFF_trackrecoeff"!=-1 or
                              syst_name_current=="FT_EFF_fracHF"!=-1 or
                              syst_name_current=="FT_EFF_eps_c_neg"!=-1 or
                              syst_name_current=="FT_EFF_eps_b_neg"!=-1): continue 
               
                  
               if systematic_typelist[isyst]=='updown':
                     
                  h_systup = systematic_filelist[isyst].Get('rel_up_sf_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
                  h_systdown = systematic_filelist[isyst].Get('rel_down_sf_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
                     
                  syst = max(abs(h_systup.GetBinContent(ipt+1)),abs(h_systdown.GetBinContent(ipt+1)))*100 #%
                  #print syst
                  file.write('                sys(%s,%.2f%%)\n' % (syst_name_current_up, syst) ) 
                     
               else:
                     
                  h_syst = systematic_filelist[isyst].Get('rel_sf_%s_w%i_eta%i' % (iTAG, iWP, ieta+1) )
                  syst = h_syst.GetBinContent(ipt+1)*100 #%
                  if syst_name_current=='FT_EFF_notrackrew' or syst_name_current=='FT_EFF_notrackrew_subleadingjet': continue 
                  if syst_name_current=='FT_EFF_subleadingjet': syst = 0.05*100 # 5% flat
                  if syst_name_current=='FT_EFF_generator': continue; 
                  if syst_name_current=='FT_EFF_subleadingjet_generator': continue; 
                  #print syst
   
                  correlation_name = 'sys'
                  if syst_name_current.find('frac')!=-1: correlation_name = 'usys'
                  
                  file.write('                %s(%s,%.2f%%)\n' % (correlation_name, syst_name_current_up, syst) ) 
                     
         file.write('        }\n')
                     

# File builder - closing 
      file.write('}\n')
      file.close() 
                     
