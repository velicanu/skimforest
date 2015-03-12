NAME="skimforest"
if [ $# -lt 4 ]
then
  echo "Usage: ./${NAME}.sh <inputlist> <outdir> <jetcollection> <jetpt> [run]"
  echo "hint: <jetcollection> = ak3PF for pp and akPu3PF for pPb and PbPb"
  exit 1
fi

now="_${NAME}_$(date +"%Y_%m_%d__%H_%M_%S")_proddir"
mkdir $now

g++ ${NAME}.C $(root-config --cflags --libs) -std=c++11 -Werror -Wall -O2 -o "${NAME}.exe"

cat <<EOF > "run${NAME}.sh"
if [ \$# -ne 5 ]
then
  echo "Usage: ./run${NAME}.sh <filenum> <inputlist> <outdir> <jetcollection> <minjetpt>"
  exit 1
fi
filenum=\$((\$1+1))
echo filename=\`head -n\${filenum} \$2 | tail -n1\`
filename=\`head -n\${filenum} \$2 | tail -n1\`
echo ./${NAME}.exe \$filename skim\${5}_\${1}.root \${4} \${5}
./${NAME}.exe \$filename skim\${5}_\${1}.root \${4} \${5}
mv skim\${5}_\${1}.root \${3}

EOF

chmod +x run${NAME}.sh

cat <<EOF > "${NAME}.condor"
Universe     = vanilla
Initialdir   = $PWD/$now
Notification = Error
Executable   = $PWD/$now/run${NAME}.sh
Arguments    = \$(Process) $1 $2 $3 $4
GetEnv       = True
Output       = /net/hisrv0001/home/$USER/logs/$now-\$(Process).out
Error        = /net/hisrv0001/home/$USER/logs/$now-\$(Process).err
Log          = /net/hisrv0001/home/$USER/logs/$now-\$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.user_flag"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = ${NAME}.exe,$1

Queue `wc -l $1 | awk '{print $1}'`

EOF

cp ${NAME}.exe $1 run${NAME}.sh ${NAME}.condor $now

if [[ "${5}" ]]
then
  echo "\$5 = ${5}"
  condor_submit $now/${NAME}.condor
fi
