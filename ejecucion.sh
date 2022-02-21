if ! test $1 -gt 1
    echo " Valor 1 menor de lo esperado"
    fi
if  test $1 -gt 100
    echo "Valor 1 mayor de lo esperado"
    fi

if ! test $2 -gt 1
    echo " Valor 2 menor de lo esperado"
    fi

if  test $2 -gt 100
    echo "Valor 2 mayor de lo esperado"
    fi
    
if ! test $3 -gt 1
    echo " Valor 3 menor de lo esperado"
    fi

if  test $3 -gt 100
    echo "Valor 3 mayor de lo esperado"
    fi

if ! test $4 -gt 1
    echo " Valor 4 menor de lo esperado"
    fi

if  test $4 -gt 100
    echo "Valor 4 mayor de lo esperado"
    fi

if ! test $5 -gt 1
    echo " Valor 5 menor de lo esperado"
    fi

if  test $5 -gt 100
    echo "Valor 5 mayor de lo esperado"
    fi

echo "Ejecución con D = " $1
./program $1

echo "Ejecución con D = " $2
./program $2

echo "Ejecución con D = " $3
./program $3

echo "Ejecución con D = " $4
./program $4

echo "Ejecución con D = " $5
./program $5
