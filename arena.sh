#!/bin/sh

set -ue


die() {
  echo $1
  exit 1
}

abspath() {
  D=`dirname "$1"`
  B=`basename "$1"`
  echo "`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
}

first=$(abspath "$1")
second=$(abspath "$2")
field=$3

INFO=../info

[ ! -x $first  ] && die "Bad first player"
[ ! -x $second ] && die "Bad second player"
[ ! -f $field  ] && die "Bad field"

echo -e "First player:\t$first"
echo -e "Second player:\t$second"


echo "Preparing arena..."
mkdir -p arena
rm -f arena/*
cp $field arena/matrix.txt

cd arena
echo "Start:" 

while true; do
  eval $($INFO --info matrix.txt)

  $INFO --pretty matrix.txt
  echo

  [ x$state != xU ] && break
  [ $turn -gt 60  ] && break

  rem=$((turn%2))
  if [ $rem -eq 1 ]; then
    this_player=A
    this_player_exe="$first"
    this_player_score="$player1score"
    this_player_time="$player1time"
  else
    this_player=B
    this_player_exe="$second"
    this_player_score="$player2score"
    this_player_time="$player2time"
  fi

  echo "Turn $((1 + turn/2))/$this_player (${this_player_time}s left):"
  $this_player_exe
done
