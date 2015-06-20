#!/bin/bash

HTML="`readlink -f \`dirname $0\`/../html`"

#index.html
sed -i -e 's/<div class=\"toc\">/<div class=\"toc\">\n<__badges__>\n<__search__>/' $HTML/index.html
sed -i -e '/<__badges__>/{r tools/data/badges.html' -e 'd}' $HTML/index.html
sed -i -e '/<__search__>/{r tools/data/search.html' -e 'd}' $HTML/index.html

##try it online
#rm -f /tmp/di_try_it_online.tmp
#FRAME=1 tools/try_it_online.sh ../example/try_it_online/main.cpp > /tmp/di_try_it_online.tmp
#find $HTML -iname "*.html" | xargs sed -i -e '/\$try_it_online\$/{r /tmp/di_try_it_online.tmp' -e 'd}'
#rm -f /tmp/di_try_it_online.tmp

#*.html
for file in `find $HTML -iname "*.html"`; do
    sed -i -e 's/<\/body>/<__comments__>\n<\/body>/' $file
    sed -i -e '/<__comments__>/{r tools/data/comments.html' -e 'd}' $file
    sed -i -e 's/\(\.\.\/\)\+libs\/libraries\.htm/http:\/\/boost.org\/doc\/libs/' $file
    sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/' $file
    sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/' $file

    rm -f /tmp/file

    s=`grep "<head>" -n $file | head -1 | cut -d':' -f1`
    f=`grep "</head>" -n $file | head -1 | cut -d':' -f1`
    h="sed -n $s,${f}p $file"
    $h > /tmp/head
    sed -e '/<__head__>/{r /tmp/head' -e 'd}' tools/data/template.html > /tmp/tmp.html

    b=`grep "name=\"di" -n $file | head -1 | cut -d':' -f1`
    b=$((++b));
    b=$((++b));
    e=`grep "table.*revision" -n $file | head -1 | cut -d':' -f1`
    e=$((--e));
    command="sed -n $b,${e}p $file"
    $command > /tmp/file
    sed -e '/<__content__>/{r /tmp/file' -e 'd}' /tmp/tmp.html > $file

done

