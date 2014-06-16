#!/bin/bash

function generate_example() {
    echo "[section `echo $1 | sed "s/.*\/\(.*\).cpp/\1/" | tr '_' ' ' | sed -r 's/\<./\U&/g'`]"
        echo "[table"
        echo "[[C++98/03] [C++11]]"
        echo "["
            echo "[\`\`\`"
            cat $1
            echo  "\`\`\`]"
            echo "[\`\`\`"
            cat $1
            echo  "\`\`\`]"
        echo "]"
        echo "["
            echo "[full code example:  [@http://examples/hello_world.cpp hello_world_03.cpp]]"
            echo "[full code example:  [@http://examples/hello_world.cpp hello_world_03.cpp]]"
        echo "]"
    echo "]"
    echo "[endsect]"
}

function generate_examples() {
    echo "[section Examples]"
    for file in `find $1 -type f -iname "*.cpp"`; do
        generate_example $file
    done
    echo "[endsect]"
}


dir=`readlink -f \`dirname $0\``

generate_examples `readlink -f $dir/../../example`

