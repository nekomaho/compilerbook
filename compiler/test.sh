#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 "0;"
try 42 "42;"
try 2 "1+1;"
try 3 "1+1+1;"
try 5 "1+1+1+1+1;"
try 8 "-2+10;"
try 4 "-2*-2;"
try 21 "5+20-4;"
try 21 ' 5 +   20 - 4 ;'
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 8 "a=3+5; a;"
try 8 "a=3; b=5; a+b;"
try 9 "a=3*5; b=9/3; c=(a+b)/2;"
try 1 "1==1;"
try 0 "1==2;"
try 1 "1!=2;"
try 0 "1!=1;"
try 0 "1<=0;"
try 1 "1<=2;"
try 0 "1<1;"
try 1 "1<2;"
try 0 "0>=1;"
try 1 "2>=1;"
try 0 "1>1;"
try 1 "2>1;"
try 5 "return 5;"
try 3 "a=1+2; return a;"
echo 0