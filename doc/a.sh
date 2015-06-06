#!/bin/bash

find html -iname "*.html" | xargs sed -i -e '/<\/body>/{r comments.html' -e 'd}' -e 's/<\/html>/<\/body><\/html>/'
find html/di -iname "*.html" | xargs sed -i -e 's/<\/div><\/div><\/div>/<___><\/div><\/div><\/div>/' -e '/<___>/{r search.html' -e 'd}'

sed -i -e 's/<div class=\"toc\">/<___>\n<div class=\"toc\">/' html/index.html
sed -i -e '/<___>/{r search.html' -e 'd}' html/index.html

sed -i -e 's/<div class=\"toc\">/<div class=\"toc\">\n<___>/' html/index.html
sed -i -e '/<___>/{r badges.html' -e 'd}' html/index.html
