#!/bin/sh

assert () {
  program=$1
  input=$2
  expected=$3
  echo "$program (input '$input') -> expected '$expected'"
  actual=`echo "$input" | ./bfi "$program"`
  result=$?
  if [ "$expected" != "$actual" ]; then
    echo "expected '$expected', got '$actual'"
    exit 1
  fi
  if [ $result -ne 0 ]; then
    echo "Error code $result"
    exit $result
  fi
  echo "passed"
}

assert ',.' 'a' 'a'
assert ',>[<.>]' 'B' ''
assert ',>[[[<.>]]]' 'B' ''
assert ',>+++++[<.+>-]' 'a' 'abcde'
assert '>+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.[-]>++++++++[<++++>-]<.>+++++++++++[<+++++>-]<.>++++++++[<+++>-]<.+++.------.--------.[-]>++++++++[<++++>-]<+.[-]++++++++++.' '' 'Hello World!'

echo "OK"
