#/bin/bash
make > /dev/null

echo >> history.txt
echo >> history.txt
printf 'Commit hash: ' >> history.txt; git rev-parse HEAD >> history.txt
printf 'Date       : ' >> history.txt; date >> history.txt

./miniRT rt/dragon.rt | tee -a history.txt
