gcc -o IV -fopenmp -O0 p2_apartado4.c
echo -n "NC=c(">>pr.r
for i in {0..100}
do
    ./IV 250 pr.r>>log4.log 4
done
echo ")">>pr.r