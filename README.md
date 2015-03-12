skimforest
==========

Code to make a smaller forest from a larger one. Currently specialized on skimming on jetpt within |eta| < 1.5 from the center of mass with pcoll and hbhe noise filters. The skimforest.sh will compile the skimming code and generate a condor worker and submission script.

First clone the HiForestAnalysis repo in this folder:
```bash
git clone git@github.com:CmsHI/HiForestAnalysis.git
```

For example:

```bash
# ./skimforest.sh <inputlist> <outdir> <jetcollection> <jetpt> [run]
./skimforest.sh pPb_5TeV_Jet80_split.txt /mnt/hadoop/cms/store/user/velicanu/pPb_5TeV_Jet80_skim akPu3PF 100 run
```
