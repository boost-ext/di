#!/bin/bash

HTML="`readlink -f \`dirname $0\`/../html`"

#index.html
sed -i -e 's/<div class=\"toc\">/<div class=\"toc\">\n<__badges__>\n<__search__>/' $HTML/index.html
sed -i -e '/<__badges__>/{r tools/data/badges.html' -e 'd}' $HTML/index.html
sed -i -e '/<__search__>/{r tools/data/search.html' -e 'd}' $HTML/index.html

#try it online
rm -f /tmp/di_try_it_online.tmp
FRAME=1 tools/try_it_online.sh ../example/try_it_online/main.cpp > /tmp/di_try_it_online.tmp
find $HTML -iname "*.html" | xargs sed -i -e '/\$try_it_online\$/{r /tmp/di_try_it_online.tmp' -e 'd}'
rm -f /tmp/di_try_it_online.tmp

#*.html
find $HTML/di -iname "*.html" | xargs sed -i -e 's/<\/div><\/div><\/div>/<__search__><\/div><\/div><\/div>/'
find $HTML/di -iname "*.html" | xargs sed -i -e '/<__search__>/{r tools/data/search.html' -e 'd}'
find $HTML -iname "*.html" | xargs sed -i -e 's/<\/body>/<__comments__>\n<\/body>/'
find $HTML -iname "*.html" | xargs sed -i -e '/<__comments__>/{r tools/data/comments.html' -e 'd}'
find $HTML -iname "*.html" | xargs sed -i -e 's/\(\.\.\/\)\+libs\/libraries\.htm/http:\/\/boost.org\/doc\/libs/'
find $HTML -iname "*.html" | xargs sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/'
find $HTML -iname "*.html" | xargs sed -i -e 's/\(\.\.\/\)\+more\/index\.htm/http:\/\/boost.org\/users\/index.html/'

