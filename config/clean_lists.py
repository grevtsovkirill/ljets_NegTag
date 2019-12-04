import os, math, sys, subprocess, argparse

parser = argparse.ArgumentParser(description='clean up list of derivations')
parser.add_argument('--inputfile','-i', type=str,
                    help='File with list of grid samples')
parser.add_argument('--proj_name','-p',type=str,
                    help='Project name')
args = parser.parse_args()
in_file=args.inputfile
project_name=args.proj_name

if not in_file:
    in_file = 'test.list'
    print 'empty, use test.list'

list_deriv =[]
with open(in_file) as f:
    content = f.readlines()
    p_rep=project_name+':'
    print p_rep
    for x in content:
        z= x.replace(p_rep,'')
        y= z.replace(' ','')
        y= y.replace('SCOPE:NAME','')
        y= y.replace('[DIDTYPE]','')
        y= y.replace('--','')
        y= y.replace('|','')
        y= y.replace('CONTAINER','')
        y= y.replace('+','')
        y= y.replace('\n','')
        if y:
            list_deriv.append(y)
            print y

list_deriv.sort()
out_file='cor_'+in_file
with open(out_file,'w') as f:
    f.write("\n".join(list_deriv))
