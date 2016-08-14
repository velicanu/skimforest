NAME="skimforest"
if [ $# -lt 5 ]
then
  echo "Usage: ./${NAME}.sh <inputlist> <outdir> <jetcollection> <jetpt> <increment> [run]"
  echo "hint: <jetcollection> = ak3PF for pp and akPu3PF for pPb and PbPb"
  exit 1
fi
Queue=200
now="_${NAME}_$(date +"%Y_%m_%d__%H_%M_%S")_proddir"
mkdir $now
mkdir -p $2

# g++ ${NAME}.C $(root-config --cflags --libs) -std=c++11 -Werror -Wall -O2 -o "${NAME}.exe"

cat <<EOF > "run${NAME}.sh"
if [ \$# -ne 6 ]
then
  echo "Usage: ./run${NAME}.sh <filenum> <inputlist> <outdir> <jetcollection> <minjetpt> <increment>"
  exit 1
fi
if [ -f /net/hisrv0001/home/dav2105/.x509up_u1914 ]; then
    export X509_USER_PROXY=/net/hisrv0001/home/dav2105/.x509up_u1914
fi

start=\$((\$6*$Queue))
filenum=\$((\$1+\$start+1))
echo \$start
echo \$filenum
echo filename=\`head -n\${filenum} \$2 | tail -n1\`
filename=\`head -n\${filenum} \$2 | tail -n1\`
xrdcp \${filename} .
echo rootfilename=\`echo \${filename} | awk -F '/' '{print \$11}'\`
rootfilename=\`echo \${filename} | awk -F '/' '{print \$11}'\`
echo ./${NAME}.exe \$rootfilename skim\${5}_\${filenum}.root \${4} \${5}
./${NAME}.exe \$rootfilename skim\${5}_\${filenum}.root \${4} \${5}
mv skim\${5}_\${filenum}.root \${3}
rm \$rootfilename $1 ${NAME}.exe
EOF

chmod +x run${NAME}.sh

cat <<EOF > "${NAME}.condor"
Universe     = vanilla
Initialdir   = $PWD/$now
Notification = Error
Executable   = $PWD/$now/run${NAME}.sh
Arguments    = \$(Process) $1 $2 $3 $4 $5
GetEnv       = True
Output       = /net/hisrv0001/home/$USER/logs/$now-\$(Process).out
Error        = /net/hisrv0001/home/$USER/logs/$now-\$(Process).err
Log          = /net/hisrv0001/home/$USER/logs/$now-\$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.user_flag"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = ${NAME}.exe,$1

Queue ${Queue}

EOF

cp ${NAME}.exe $1 run${NAME}.sh ${NAME}.condor $now

if [[ "${6}" ]]
then
  echo "\$6 = ${6}"
  condor_submit $now/${NAME}.condor
fi
