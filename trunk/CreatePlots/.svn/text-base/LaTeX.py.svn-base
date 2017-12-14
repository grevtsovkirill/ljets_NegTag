# -*- coding: utf-8 -*-
import re

import logging
logger = logging.getLogger(__name__)

replacements = {
    'ä':'\\"a',
    'ö':'\\"o',
    'ü':'\\"u',
    'Ä':'\\"A',
    'Ö':'\\"O',
    'Ü':'\\"U',
    '€':'\\euro{}',
    '_':'\_',
    '<':r'\\textless',
    '>':r'\\textgreater',
    '%':'\%'
    }

def cleanLaTeX(string):
    for repl in replacements:
        string = re.sub(repl, replacements[repl], string)

    return string

def getTable(rows, **kwargs):
    nColumns = None
    texBody = ''
    for row in rows:
        if nColumns is None:
            nColumns = len(row)
        elif nColumns != len(row):
            logger.error('{} has different lengths'.format(row))

        texBody += cleanLaTeX(getRow(row))
        for cell in row:
            print (cell)
    tex = getHeader(nColumns, **kwargs)
    tex += texBody
    tex += getFooter(**kwargs)
    return tex

def getHeader(nColumns, **kwargs):
    tex = ''
    if kwargs.get('fullDoc', False):
        tex += '\\documentclass[12pt]{article}\n'
        packages = kwargs.get('packages', [])
        for package in packages:
            tex += '\\usepackage[{}]{{{}}}'.format(packages[package], package)
        
        tex += '\\begin{{document}}\n'.format()
    tex += '\\begin{{tabular}}{{ {} }}\n'.format(nColumns*'c')
    return tex

def getFooter(**kwargs):
    tex = ''
    tex += '\\end{{tabular}}\n'.format()
    if kwargs.get('fullDoc', False):
        tex += '\\end{{document}}\n'.format()
    return tex

def getRow(row):
    tex = ''
    for cell in row[:-1]:
        tex += '{}&'.format(cell)
    tex += '{}'.format(row[-1:][0]) + '\\\\ \n'
    return tex

def dumpToPdf(rows, pdfpath):
    with open(pdfpath, 'w') as f:
        f.write(getTable(rows, fullDoc=False, packages={'geometry':'a4paper,landscape, margin=1cm','eurosym': 'official', 'xcolor':''}))


if __name__ == '__main__':
    with open('overview.tex', 'w') as f:
        f.write(getTable([[1,2,3,4,5],['a','b','c','d','e']], fullDoc=True))
