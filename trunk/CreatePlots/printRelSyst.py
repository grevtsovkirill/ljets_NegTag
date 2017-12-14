#!/usr/bin/env python2.7

import ROOT
import pprint as PP
import re

import LaTeX as L

WPs = ['w30', 'w50', 'w60', 'w70', 'w77', 'w85']
etas = ['eta1', 'eta2']
taggers = ['MV2c10','MV2c20']
ptBins = [20, 30, 40, 60, 110, 270, 450, 750, 1000]

def getUncDef():
    print "getDef"
    uncDef = {}
    with open("../config/systematics.txt", "r") as f:
        lines = f.readlines()
    for line in lines:
        line = line.strip()
        search = re.search("\#", line)
        if search is not None:
            continue

        split = line.split()
        uncDef[split[1]] = {"mode": split[0], "type": split[2], "var": split[3]}
    return uncDef

def getUncertainties(path, name):
    rootFile = ROOT.TFile(path)
    uncMap = {}
    for tagger in taggers:
        uncMap[tagger] = {}
        for wp in WPs:
            uncMap[tagger][wp] = {}
            for eta in etas:
                uncMap[tagger][wp][eta] = {}
                histo = rootFile.Get("rel_{}_{}_{}_{}".format(name, tagger, wp, eta))
                if not histo:
                    print "rel_{}_{}_{}_{}".format(name, tagger, wp, eta), "not found in {}".format(path)
                # print(histo.Integral())
                for ibin in range(1,histo.GetNbinsX()+1):
                    uncMap[tagger][wp][eta][ibin] = histo.GetBinContent(ibin)
    # for histoKey in rootFile.GetDirectory(rootFile.GetPath()).GetListOfKeys():
    #     print histo.GetName()
    #     for ibin in range(1, histo.GetNbinsX()+1):
    #         print hist.GetBinContent(ibin)
    return uncMap

def getPTRow(uncDict, tagger, wp, eta, unc, **kwargs):
    import math
    row = [unc]
    pts = uncDict[unc][tagger][wp][eta]
    for ipt in pts:
        val = pts[ipt]
        aval = math.fabs(val)
        if aval > 0.1:
            text = "\color{{red}}{{{:.2%}}}".format(pts[ipt])
        elif aval > 0.05:
            text = "\color{{orange}}{{{:.2%}}}".format(pts[ipt])
        elif aval > 0.01:
            text = "\color{{blue}}{{{:.2%}}}".format(pts[ipt])
        else:
            text = "{:.2%}".format(pts[ipt])
        row.append(text)
    return row

if __name__ == '__main__':
    uncertainties = {}

    uncDef = getUncDef()

    import os
    for iunc in uncDef:
        iDict = uncDef[iunc]
        if iDict["type"] == "no_rel":
            continue
        if iunc[-4:] == "down":
            name = iunc[:-4]
        elif iunc[-2:] == "up":
            continue
        else:
            name = iunc
        print name
        uncertainties[name] = getUncertainties("../CalculateNTVariables/rel_systematics/rel_{}.root".format(name), uncDef[iunc]["var"])
    # PP.pprint(uncertainties, width=240)

    # for syst in uncertainties:
    #     print syst,
    #     taggers = uncertainties[syst]
    #     for tagger in taggers:
    #         print tagger,
    #         wps = taggers[tagger]
    #         for wp in wps:
    #             # if wp != "w85":
    #             #     continue
    #             print wp,
    #             etas = wps[wp]
    #             for eta in etas:
    #                 print eta
    #                 pts = etas[eta]
    #                 row = []
    #                 for ipt in pts:
    #                     print ipt, "{:.2%}".format(pts[ipt])
    #                     row.append("{:.2%}".format(pts[ipt]))
    #                 print row

    tables = []

    for wp in uncertainties["datastat"]["MV2c20"]:
        for eta in uncertainties["datastat"]["MV2c20"]["w85"]:
            table = []
            table.append([wp, eta, "", "\color{red}{>10%}", "", "\color{orange}{>5%}", "", "\color{blue}{>1%}", ""])
            ptRow = [""]
            for i in range(len(ptBins)-1):
                ptRow.append("{}-{} GeV".format(ptBins[i], ptBins[i+1]))
            table.append(ptRow)

            body = []
            for syst in uncertainties:
                print wp, eta
                body.append(getPTRow(uncertainties, "MV2c10", wp, eta, syst))
            body.sort(key=lambda row: row[0])
            table += body
            tables.append(table)
            L.dumpToPdf(table, "tables/{}_{}.tex".format(wp, eta))
