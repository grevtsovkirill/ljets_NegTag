echo
echo "------------------------------------------------------"
echo "|                                                    |"
echo "|   Link Data and MC txt files + apply proper lumi   |"
echo "|                                                    |"
echo "------------------------------------------------------"
echo

# default settings: 2015+2016
##################################
data_files="data_files_15and16.txt"
mc_files="mc_files_15c_cpu.txt"
mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
lumi="36470.16"

# read command line configuration
##################################
if [ $# = 1 ]
then
    if [ "$1" = '15' ]
    then
        data_files="data_files_15.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="3212.96"
    elif [ "$1" = '16' ]
    then
        data_files="data_files_16.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="32861.6"
    elif [ "$1" = '1516' ]
    then
        data_files="data_files_15and16.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="36074.56"
    elif [ "$1" = '15D' ]
    then
        data_files="data_files_15D.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="51.6461"
    elif [ "$1" = '15E' ]
    then
        data_files="data_files_15E.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="435.605"
    elif [ "$1" = '15F' ]
    then
        data_files="data_files_15F.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="301.693"
    elif [ "$1" = '15G' ]
    then
        data_files="data_files_15G.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="724.399"
    elif [ "$1" = '15H' ]
    then
        data_files="data_files_15H.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="264.362"
    elif [ "$1" = '15J' ]
    then
        data_files="data_files_15J.txt"
        mc_files="mc_files_15c_bpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_bpu.txt"
        lumi="1435.25"
    elif [ "$1" = '16A' ]
    then
        data_files="data_files_16A.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="552.738"
    elif [ "$1" = '16B' ]
    then
        data_files="data_files_16B.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="1949.59"
    elif [ "$1" = '16C' ]
    then
        data_files="data_files_16C.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="2872.72"
    elif [ "$1" = '16D' ]
    then
        data_files="data_files_16D.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="4626.62"
    elif [ "$1" = '16E' ]
    then
        data_files="data_files_16E.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="1481.01"
    elif [ "$1" = '16F' ]
    then
        data_files="data_files_16F.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="3398.89"
    elif [ "$1" = '16G' ]
    then
        data_files="data_files_16G.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="3827.95"
    elif [ "$1" = '16I' ]
    then
        data_files="data_files_16I.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="5802.86"
    elif [ "$1" = '16K' ]
    then
        data_files="data_files_16K.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="2198.23"
    elif [ "$1" = '16L' ]
    then
        data_files="data_files_16L.txt"
        mc_files="mc_files_15c_cpu.txt"
        mcHERWIG_files="mcHERWIG_files_15c_cpu.txt"
        lumi="6151.19"
    fi
fi

# link data and MC files
##################################
rm -f config/data_files.txt 
ln -s "$data_files" config/data_files.txt

rm -f config/mc_files.txt 
ln -s "$mc_files" config/mc_files.txt

rm -f config/mcHERWIG_files.txt 
ln -s "$mcHERWIG_files" config/mcHERWIG_files.txt

# store correct lumi in config
##################################
sed -i "s/int_lumi = 3212.96/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 32861.6/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 36074.56/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 51.6461/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 435.605/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 301.693/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 724.399/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 264.362/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 1435.25/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 552.738/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 1949.59/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 2872.72/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 4626.62/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 1481.01/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 3398.89/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 3827.95/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 5802.86/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 2198.23/int_lumi = $lumi/g" ./config/conf.hpp
sed -i "s/int_lumi = 6151.19/int_lumi = $lumi/g" ./config/conf.hpp

