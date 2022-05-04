gcc -o II -O0 p2_apartado2.c
gcc -o III -mavx2 p2_apartado3.c

echo -e "II"
./II 250 ex.r>a.txt
echo -e "\n---------------------------------------\nIII"
./III 250 ex.r>t.txt

diff a.txt t.txt