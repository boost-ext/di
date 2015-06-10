#!/bin/bash

function json_escape() {
    cat $1 | python -c 'import json,sys; print json.dumps(sys.stdin.read())'
}

json() {
    echo -e -n '{ "code" : '
        json_escape $1

    echo -e -n ',
      "codes": [
          { "file": "di.hpp", "code": '
              json_escape `readlink -f \`dirname $0\``/../../include/boost/di.hpp

    echo -e '}],
      "options": "warning,cpp-pedantic-errors,optimize,boost-nothing,c++14",
      "compiler": "clang-head",
      "save": true
    }'
}

curl -H "Content-type: application/json" -d "`json $1`" http://melpon.org/wandbox/api/compile.json

