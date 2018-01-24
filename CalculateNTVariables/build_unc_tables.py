# ----------------------------------
# Importation for pyROOT
import ROOT

# ----------------------------------
# Input parameter
collection='AntiKt4EMTopoJets'
systematic_file='../config/systematics.txt'

# Dictionnaries, arrays and files

WP_list = {85, 77, 70, 60}
WP_dict = dict = { 85: 0.1758475, 77: 0.645925, 70: 0.8244273, 60: 0.934906} # MV2c10

pt_bins = [20, 60, 100, 200, 300, 500, 750, 1000, 3000]
abseta_bins = [0.0, 1.2, 2.5]

f_nom_stat = ROOT.TFile('tot_systematics/total_sf.root') 

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
            file_name = 'rel_systematics/rel_sf_' + systematic_name + '.root' 
            systematic_name = systematic_name.replace('_up','')
         else                  : 
            if systematic_name.find('_up')==-1 and systematic_name.find('__1up')==-1: continue;
            elif systematic_name.find('conversions')!=-1:
               systematic_name = systematic_name.replace('up','')
               file_name = 'rel_systematics/rel_sf_' + systematic_name + '.root' 
               if systematic_name.find('subleadingjet'): systematic_name = 'subleadingjet_conversions'
               else:                                     systematic_name = 'conversions' 
            else :
               systematic_name = systematic_name.replace('__1up','__1')
               file_name = 'rel_systematics/rel_sf_' + systematic_name + '.root'
               systematic_name = systematic_name.replace('__1','')

         systematic_name = systematic_name.replace('FlavourTagging_','FT_EFF_')
         if systematic_name.find('FT_EFF_')==-1: systematic_name = 'FT_EFF_' + systematic_name

         systematic_list.append(systematic_name)
         systematic_filelist.append( ROOT.TFile(file_name) )
         systematic_typelist.append( type_name )

print systematic_list
#print systematic_typelist

# ----------------------------------
# File builder - prelude
for iWP in WP_list:
 
   # ----------------------------------
   # File builder - eta loop
   for ieta in range(0,len(abseta_bins)-1):
   
      file = open('unc_tables/tables_negtag_v00-07_WP%i_eta%i.tex' % (iWP, ieta+1),'w') 
  
      # eta label and label for caption
      table_label='negTag_uncert_WP%i_eta%i' % (iWP, ieta+1)
      if ieta==0: eta_label='$\\aetajet<1.2$'
      else      : eta_label='$1.2<\\aetajet<2.5$'
 
      # histo from tot_sf.root
      # 1st pT bin already based on subleading jet
      h_nom = f_nom_stat.Get( 'sf_nom_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      h_stat = f_nom_stat.Get( 'rel_stat_total_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      h_JES_up = f_nom_stat.Get( 'rel_calib_up_total_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_JES_down = f_nom_stat.Get( 'rel_calib_down_total_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      h_tot_up = f_nom_stat.Get( 'rel_tot_up_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_tot_down = f_nom_stat.Get( 'rel_tot_down_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      # histo from rel_sf files
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_JET_JER_SINGLE_NP__1up' in s][0]
      h_JER = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'subleadingjet_FT_EFF_JET_JER_SINGLE_NP__1up' in s][0]
      h_JER2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_JVT_effSF' in s][0]
      h_JVT_up = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_JVT_down = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'subleadingjet_FT_EFF_JVT_effSF' in s][0]
      h_JVT_up2 = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_JVT_down2 = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_PRW_DATASF' in s][0]
      h_PRW_up = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_PRW_down = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'subleadingjet_FT_EFF_PRW_DATASF' in s][0]
      h_PRW_up2 = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_PRW_down2 = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'subleadingjet' in s][0]
      h_subleadingjet = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_generator' in s][0]
      h_generator = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_generator' in s][0]
      h_generator2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_conversions' in s][0]
      h_conversions_up = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_conversions_down = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_conversions' in s][0]
      h_conversions_up2 = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_conversions_down2 = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_hadronic' in s][0]
      h_hadronic_up = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_hadronic_down = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_hadronic' in s][0]
      h_hadronic_up2 = systematic_filelist[index].Get('rel_up_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
      h_hadronic_down2 = systematic_filelist[index].Get('rel_down_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_longlivedparticles' in s][0]
      h_longlivedparticles = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_longlivedparticles' in s][0]
      h_longlivedparticles2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_d0smearing' in s][0]
      h_d0smearing = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_d0smearing' in s][0]
      h_d0smearing2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_z0smearing' in s][0]
      h_z0smearing = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_z0smearing' in s][0]
      h_z0smearing2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_faketracks' in s][0]
      h_faketracks = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_faketracks' in s][0]
      h_faketracks2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_fracHF' in s][0]
      h_fracHF = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_fracHF' in s][0]
      h_fracHF2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_eps_c_neg' in s][0]
      h_eps_c_neg = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_eps_c_neg' in s][0]
      h_eps_c_neg2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )
  
      #
      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_eps_b_neg' in s][0]
      h_eps_b_neg = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      index = [i for i, s in enumerate(systematic_list) if 'FT_EFF_subleadingjet_eps_b_neg' in s][0]
      h_eps_b_neg2 = systematic_filelist[index].Get('rel_sf_MV2c10_w%i_eta%i' % (iWP, ieta+1) )

      # initiate the table
      file.write('\\begin{table}[!htbp]\n')
      file.write('\\setlength\\extrarowheight{3pt}\n')
      file.write('\\caption{Mistag rate scale factor from the negative tag method for the %i\\%% fixed-cut efficiency MV2c10 working point for jets with %s and associated relative statistical and systematic uncertainties (in \\%%).}\n' % (iWP, eta_label) ) 
      file.write('  \\centering\n')
      file.write('  \\small\n')
      file.write('  \\begin{tabular}{c|cccccccc}\n') # nb of jet pT hardcoded ...
      file.write('  \\hline\\hline\n')
      file.write('  Source & \\multicolumn{8}{c}{\ptjet\ [GeV] } \\\\ [3.5pt]\n') 

      # pT bin edges
      file.write('    ')
      for ipt in range(0,len(pt_bins)-1):
         file.write('& %.0f-%.0f ' % (pt_bins[ipt], pt_bins[ipt+1]))
      file.write('\\\\ [3.5pt]\n')

      # SF line
      file.write('  \\hline\n')
      file.write('  Mistag rate scale factor ')
      for ipt in range(0,len(pt_bins)-1): 
         # get central value
         sf   = h_nom.GetBinContent(ipt+1) 
         file.write('& %.2f ' % sf)
      file.write('\\\\ [3.5pt]\n') 

      # Stat line
      file.write('  \\hline\n')
      file.write('  Total statistics (data + MC) ')
      for ipt in range(0,len(pt_bins)-1):
         stat = h_stat.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % stat)
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # JES line
      file.write('  Jet energy scale ')
      for ipt in range(0,len(pt_bins)-1):
         calib_up = h_JES_up.GetBinContent(ipt+1)*100 # %
         calib_down = h_JES_down.GetBinContent(ipt+1)*100 # %
         file.write('& $^{+%.1f}_{%.1f}$ ' % (calib_up, calib_down))
      file.write('\\\\ [3.5pt]\n')

      # JER line
      file.write('  Jet energy resolution ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: JER = h_JER2.GetBinContent(ipt+1)*100 # %
         else:      JER = h_JER.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(JER))
      file.write('\\\\\n')

      # JVT line
      file.write('  JVT efficiency ')
      for ipt in range(0,len(pt_bins)-1):

         if ipt==0: 
           JVT_up = h_JVT_up2.GetBinContent(ipt+1)*100 # %
           JVT_down = h_JVT_down2.GetBinContent(ipt+1)*100 # %
         else: 
           JVT_up = h_JVT_up.GetBinContent(ipt+1)*100 # %
           JVT_down = h_JVT_down.GetBinContent(ipt+1)*100 # %

         file.write('& $^{+%.1f}_{%.1f}$ ' % (JVT_up, JVT_down))
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # Photon conversions
      file.write('  Photon conversions ')
      for ipt in range(0,len(pt_bins)-1):

         if ipt==0:
           conversions_up = h_conversions_up2.GetBinContent(ipt+1)*100 # %
           conversions_down = h_conversions_down2.GetBinContent(ipt+1)*100 # % 
         else:
           conversions_up = h_conversions_up.GetBinContent(ipt+1)*100 # %
           conversions_down = h_conversions_down.GetBinContent(ipt+1)*100 # %

         file.write('& $^{+%.1f}_{%.1f}$ ' % (conversions_up, conversions_down))
      file.write('\\\\ [3.5pt]\n')

      # Hadronic material interactions
      file.write('  Hadronic material interactions ')
      for ipt in range(0,len(pt_bins)-1):

         if ipt==0:
           hadronic_up = h_hadronic_up2.GetBinContent(ipt+1)*100 # %
           hadronic_down = h_hadronic_down2.GetBinContent(ipt+1)*100 # %
         else:
           hadronic_up = h_hadronic_up.GetBinContent(ipt+1)*100 # %
           hadronic_down = h_hadronic_down.GetBinContent(ipt+1)*100 # %

         file.write('& $^{+%.1f}_{%.1f}$ ' % (hadronic_up, hadronic_down))
      file.write('\\\\ [3.5pt]\n')

      # Long-lived particles
      file.write('  Long-lived particles ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: longlivedparticles = h_longlivedparticles2.GetBinContent(ipt+1)*100 # %
         else:      longlivedparticles = h_longlivedparticles.GetBinContent(ipt+1)*100 # %
         file.write('& %+.1f ' % longlivedparticles)
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')


      # D0 smearing 
      file.write('  Track resolution ($d_{0}$) ')
      for ipt in range(0,len(pt_bins)-1):
         
         if ipt==0: d0smearing = h_d0smearing2.GetBinContent(ipt+1)*100 # %
         else:      d0smearing = h_d0smearing.GetBinContent(ipt+1)*100 # %
         file.write('& %+.1f ' % d0smearing)
      file.write('\\\\\n')

      # Z0 smearing
      file.write('  Track resolution ($z_{0}$) ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: z0smearing = h_z0smearing2.GetBinContent(ipt+1)*100 # %
         else:      z0smearing = h_z0smearing.GetBinContent(ipt+1)*100 # %
         file.write('& %+.1f ' % z0smearing)
      file.write('\\\\\n')

      # Fake tracks
      file.write('  Fake tracks ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: faketracks = h_faketracks2.GetBinContent(ipt+1)*100 # %
         else:      faketracks = h_faketracks.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(faketracks))
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # FracHF
      file.write('  Heavy flavour fraction ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: fracHF = h_fracHF2.GetBinContent(ipt+1)*100 # %
         else:      fracHF = h_fracHF.GetBinContent(ipt+1)*100 # %
         file.write('& %+.1f ' % fracHF)
      file.write('\\\\\n')

      # Negative tag efficiency for c-jets
      file.write('  $\epsilon_{\\textrm{c}}^{\\textrm{neg}}$ ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: eps_c_neg = h_eps_c_neg2.GetBinContent(ipt+1)*100 # %
         else:      eps_c_neg = h_eps_c_neg.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(eps_c_neg))
      file.write('\\\\\n')

      # Negative tag efficiency for b-jets
      file.write('  $\epsilon_{\\textrm{b}}^{\\textrm{neg}}$ ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: eps_b_neg = h_eps_b_neg2.GetBinContent(ipt+1)*100 # %
         else:      eps_b_neg = h_eps_b_neg.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(eps_b_neg))
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # PRW
      file.write('  Pileup reweighting SF ')
      for ipt in range(0,len(pt_bins)-1):

         if ipt==0:
           PRW_up = h_PRW_up2.GetBinContent(ipt+1)*100 # %
           PRW_down = h_PRW_down2.GetBinContent(ipt+1)*100 # %
         else:
           PRW_up = h_PRW_up.GetBinContent(ipt+1)*100 # %
           PRW_down = h_PRW_down.GetBinContent(ipt+1)*100 # %

         file.write('& $^{+%.1f}_{%.1f}$ ' % (PRW_up, PRW_down))
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # Subleading jet
      file.write('  Sample dependence ')
      for ipt in range(0,len(pt_bins)-1):
         subleadingjet = 0.05*100 #4% flat uncertainty h_subleadingjet.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(subleadingjet))
      file.write('\\\\\n')

      # Generator
      file.write('  Generator dependence ')
      for ipt in range(0,len(pt_bins)-1):
         if ipt==0: generator = 0 # NEGLIGIBLE h_generator.GetBinContent(ipt+1)*100 # %
         else:     generator = 0 # NEGLIGIBLE h_generator.GetBinContent(ipt+1)*100 # %
         file.write('& $\pm$ %.1f ' % abs(generator))
      file.write('\\\\ [3.5pt]\n')

      file.write('\\hline\n')

      # Total 
      file.write('  \\hline\n')
      file.write('  Total uncertainty ')
      for ipt in range(0,len(pt_bins)-1):
         tot_up = h_tot_up.GetBinContent(ipt+1)*100 # %
         tot_down = h_tot_down.GetBinContent(ipt+1)*100 # %
         file.write('& $^{+%.1f}_{%.1f}$ ' % (tot_up, tot_down))
      file.write('\\\\ [3.5pt]\n')

      # Closing      
      file.write('  \\hline\\hline\n')
      file.write('  \\end{tabular}\n')
      file.write('\\label{tab:%s}\n' % table_label)
      file.write('\\end{table}\n')

      file.close() 
