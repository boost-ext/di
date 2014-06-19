#!/bin/bash

function generate_example() {
    num=`ls $1 | wc -l`
    nice_name=`echo $1 | sed "s/.*\/\(.*\).cpp/\1/" | tr '_' ' ' | sed -r 's/\<./\U&/g'`

    echo "[section $nice_name]"
        echo "[table"
        echo -n "["
        for std in `ls $1`; do
            echo -n "[`echo $std | sed "s/cpp/C++/g" | tr '_' ' '`] "
        done
        echo "]"
        echo "["
            for std in `ls $1`; do
                echo "[\`\`\`"
                cat $1/$std
                echo  "\`\`\`]"
            done
        echo "]"
        echo "["
            for std in `ls $1`; do
                echo "[full code example:  [@example/$2 $2]]"
            done
        echo "]"
    echo "]"
    echo "[endsect]"
}

function generate_examples() {
    echo "[section Examples]"
    for file in `find $1 -type f -iname "*.cpp"`; do
        std=`echo $file | sed "s/.*\/\(.*\)\/.*/\1/"`
        name=`basename $file`
        dir=`dirname $file`
        mkdir -p /tmp/di/$name
        cat $file > /tmp/di/$name/$std
    done
    for dir in `ls /tmp/di/`; do
        generate_example /tmp/di/$dir $name
    done

    echo "[endsect]"
}


dir=`readlink -f \`dirname $0\``

generate_examples `readlink -f $dir/../../example`

