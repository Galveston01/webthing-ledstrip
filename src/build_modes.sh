#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

declare -A MODES
NUM_MODES=0

for file in "$DIR/modes/"*
do
    fname="$(basename -- $file)"
    if [[ -f $file ]] && [[ "$fname" =~ ^[m][\_]([0-9]{2})[\_](.*)\.h$ ]]; then
        MODES["$NUM_MODES,path"]=$file
        MODES["$NUM_MODES,file"]=$fname
        MODES["$NUM_MODES,ns"]="m_${BASH_REMATCH[2]}"
        NUM_MODES=$((NUM_MODES+1))
    fi
done

###### modes.cpp ######

MODES_CPP="$DIR/modes.cpp"
rm "$MODES_CPP"

cat <<EOF >> "$MODES_CPP"
#include "state.h"

EOF

for i in $(seq 0 $((NUM_MODES-1))); 
do 
    printf "#include \"modes/${MODES[$i,file]}\"\n" >> $MODES_CPP
done

cat <<EOF >> "$MODES_CPP"
namespace modes {

    char* modes[] = {
EOF

for i in $(seq 0 $((NUM_MODES-1))); 
do 
    printf "        ${MODES[$i,ns]}::title, \n" >> $MODES_CPP
done

cat <<EOF >> "$MODES_CPP"
        nullptr,
    };

    void init() {
        switch(state::mode) {
EOF

for i in $(seq 0 $((NUM_MODES-1))); 
do 
    printf "            case $i: ${MODES[$i,ns]}::init(); break;\n" >> $MODES_CPP
done

cat <<EOF >> "$MODES_CPP"
        }
    }

    void step() {
        switch(state::mode) {
EOF

for i in $(seq 0 $((NUM_MODES-1))); 
do 
    printf "            case $i: ${MODES[$i,ns]}::step(); break;\n" >> $MODES_CPP
done

cat <<EOF >> "$MODES_CPP"
        }
    }

    state::ModeState defaultState(uint8_t mode) {
        switch(mode) {
EOF

for i in $(seq 0 $((NUM_MODES-1))); 
do 
    printf "            case $i: return ${MODES[$i,ns]}::defaultState;\n" >> $MODES_CPP
done

cat <<EOF >> "$MODES_CPP"
        }
    }

};
EOF


###### modes.h ######

MODES_H="$DIR/modes.h"
rm "$MODES_H"

cat <<EOF >> "$MODES_H"
#pragma once

#include <Arduino.h>
#include "state.h"

namespace modes {
EOF

printf "    const int numberModes = $NUM_MODES;\n" >> $MODES_H

cat <<EOF >> "$MODES_H"
    extern char* modes[];
    void init();
    void step();
    state::ModeState defaultState(uint8_t);
};
EOF
