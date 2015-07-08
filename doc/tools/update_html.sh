#!/bin/bash

ROOT="`readlink -f \`dirname $0\``"
HTML="`readlink -f $ROOT/../html`"

cp -r $ROOT/../js $HTML

rm -f /tmp/di_try_it_online.tmp
FRAME=1 tools/try_it_online.sh ../example/try_it_online/main.cpp > /tmp/di_try_it_online.tmp
find $HTML -iname "*.html" | xargs sed -i -e '/\$try_it_online\$/{r /tmp/di_try_it_online.tmp' -e 'd}'
rm -f /tmp/di_try_it_online.tmp

#*.html
for file in `find $HTML/di -iname "*.html"`; do
    rm -f /tmp/file

    s=`grep "<head>" -n $file | head -1 | cut -d':' -f1`
    f=`grep "</head>" -n $file | head -1 | cut -d':' -f1`
    h="sed -n $s,${f}p $file"
    $h > /tmp/head
    sed -e '/<__head__>/{r /tmp/head' -e 'd}' tools/data/template.html > /tmp/tmp.html

    b=`grep "</div></div></div>" -n $file | head -1 | cut -d':' -f1`
    b=$((++b));
    e=`grep "table.*revision" -n $file | head -1 | cut -d':' -f1`
    e=$((--e));
    e=$((--e));
    command="sed -n $b,${e}p $file"
    $command > /tmp/file
    sed -e '/<__content__>/{r /tmp/file' -e 'd}' /tmp/tmp.html > $file

    sed -i -e 's/\(\.\.\/\)\+libs\/libraries\.htm/http:\/\/boost.org\/doc\/libs/' $file
    sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/' $file
    sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/' $file

    f=`echo $file | sed "s/.*\(di\/.*\)/\1/"`
    s=`echo $f | grep -o '/' | wc -l`
    f_="`basename $file`"
    b_=''
    for ((i=0; i < $s; ++i)); do
        b_=`echo ${b_}..\\\/`
    done

    sed -i 's/\(<span class=\"section\".*href=\".*\/'$f_'\">.*<\/span>\)/<b><u> \1 <\/u><\/b>/' $file
    sed -i 's/\(class=\"section\".*href=\)"\([^"]*\)"\(.*\)/\1 "'$b_'\2" \3/' $file
    sed -i 's/<__root__>/'$b_'/g' $file
    sed -i 's/\$example_begin \([^\$]*\)\$/<button class="TryItBtn" id="run_it_btn" onclick="show(__quote__https:\/\/raw.githubusercontent.com\/krzysztof-jusiak\/di\/cpp14\/\1__quote__)">Run this code!<\/button><textarea style="display: none" id="code"><\/textarea><br \/><textarea style="display: none" id="output"><\/textarea><div id="code_listing">/g' $file
    sed -i "s/__quote__/'/g" $file
    sed -i 's/\$example_end.*/<\/div>/g' $file
done

echo '
<script>
    window.location.replace("di/introduction.html");
</script>
' > $HTML/index.html

