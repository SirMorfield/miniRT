#/bin/bash
make > /dev/null

echo >> history.txt
echo >> history.txt
printf 'Commit hash: ' >> history.txt; git rev-parse HEAD >> history.txt
printf 'Date       : ' >> history.txt; date >> history.txt
printf 'uname -a   : ' >> history.txt; uname -a >> history.txt

printf 'Command    : ' >> history.txt; echo "./miniRT rt/dragon.rt" >> history.txt
./miniRT rt/dragon.rt | tee -a history.txt
