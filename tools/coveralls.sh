cd $2
$1 --preserve-paths --relative-only $(find $3 -iname "*.gcda") >/dev/null

echo "#!/usr/bin/python
import json,sys,codecs
sys.stdout.write(json.dumps(codecs.getreader('ISO-8859-1')(sys.stdin).read()))
" > /tmp/json_encode.py

chmod +x /tmp/json_encode.py

cat >coverage.json <<EOF
{
  "service_name": "travis-ci",
  "service_job_id": "${TRAVIS_JOB_ID}",
  "run_at": "$(date --iso-8601=s)",
  "repo_token" : "$5",
  "source_files": [
EOF

for file in $(find . -iname "*.gcov" -print)
do
    if [ "`head -1 $file | grep '$4' | grep -v preprocessed`" ]; then
        cat >> coverage.json << EOF
        {
          "name": "$(head -1 $file | sed 's/.*\/include\/\(.*\)/include\/\1/')",
          "source": $(tail -n +3 ${file} | cut -d ':' -f 3- | python /tmp/json_encode.py),
          "coverage": [$(tail -n +3 ${file} | cut -d ':' -f 1 | sed -re 's%^ +%%g; s%-%null%g; s%^[#=]+$%0%;' | tr $'\n' ',' | sed -re 's%,$%%')]
        },
EOF
    fi
done

mv coverage.json coverage.json.tmp
cat > coverage.json <(head -n -1 coverage.json.tmp) <(echo -e "    }\n  ]\n}")
rm *.gcov coverage.json.tmp

curl -F json_file=@coverage.json https://coveralls.io/api/v1/jobs

