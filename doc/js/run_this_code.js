//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
var cpp_code;
var cpp_output;
var di = get_cpp_file("https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp");

function get_cpp_file(file) {
     var cpp_file = new XMLHttpRequest();
     cpp_file.open("GET", file, false);
     cpp_file.send();
     return cpp_file.responseText;
}

function toggle(file) {
    run_it_btn.firstChild.data = 'Run this code!';
    run_it_btn.removeChild;
    run_it_btn.onclick = function() { show(file); };
    code_listing.style.display = 'block';
    cpp_code.toTextArea();
    code.style.display = 'none';
    cpp_output.toTextArea();
    output.style.display = 'none';
    document.getElementById("compile_and_run").remove();
    document.getElementById("powered_by").remove();
}

function show(file) {
    run_it_btn.firstChild.data = 'Exit';
    run_it_btn.onclick = function() { toggle(file); };
    code_listing.style.display = 'none';
    var powered_by = document.createElement("text");
    var powered_text = document.createTextNode("Powered by Wandbox");
    powered_by.appendChild(powered_text);
    powered_by.setAttribute("id", "powered_by");
    powered_by.setAttribute('style', 'font-size: 10px; text-align: right; display: block;');
    var compile_btn = document.createElement("BUTTON");
    var compile_txt = document.createTextNode("Compile & Run (Ctrl+Enter)");
    compile_btn.setAttribute("id", "compile_and_run");
    compile_btn.setAttribute("class", "TryItBtn");
    compile_btn.appendChild(compile_txt);
    compile_btn.onclick = compile_and_run;
    run_it_btn.parentNode.insertBefore(compile_btn, run_it_btn.nextSibling);
    run_it_btn.parentNode.insertBefore(powered_by, compile_btn.nextSibling);
    get_example(file);
    compile_and_run();
}

function compile_and_run() {
    document.getElementById("compile_and_run").firstChild.data = "Compiling...";
    cpp_output.setValue("");
    var http = new XMLHttpRequest();
    http.open("POST", "http://melpon.org/wandbox/api/compile.json", true);
    http.onreadystatechange = function(){
        if (http.readyState == 4 && http.status == 200) {
            var output_json = JSON.parse(http.response);
            if ('status' in output_json && output_json.status == "0") {
                if ('program_message' in output_json) {
                    cpp_output.setValue(output_json.program_message);
                }
                cpp_output.setValue(cpp_output.getValue() + "\n-------\nExit: " + output_json.status);
            } else if ('compiler_error' in output_json) {
                cpp_output.setValue(output_json.compiler_error);
            } else if ('signal' in output_json) {
                if ('program_message' in output_json) {
                    cpp_output.setValue(output_json.program_message);
                }
            }
            document.getElementById("compile_and_run").firstChild.data = "Compile & Run (Ctrl+Enter)";
        }
    }

    http.send(
        JSON.stringify({
          "code" : cpp_code.getValue()
        , "codes" : [{
              "file" : "boost/di.hpp"
            , "code" : di
           }]
         , "options": "warning,cpp-pedantic-errors,optimize,boost-nothing,c++14"
         , "compiler": "clang-head"
         , "compiler-option-raw": "-I."
    }));
}

function get_example(file) {
    cpp_code = CodeMirror.fromTextArea(code, {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        mode: "text/x-c++src"
      });

    cpp_code.setSize(1100, 500);
    cpp_code.addKeyMap({"Ctrl-Enter": function(cm){ compile_and_run(); }});

    cpp_output = CodeMirror.fromTextArea(output, {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        mode: "text/x-c++src"
      });

    cpp_output.setSize(1100, 200);
    cpp_output.setOption("theme", 'zenburn');

    cpp_code.setValue(get_cpp_file(file));
}

Element.prototype.remove = function() {
    this.parentElement.removeChild(this);
}

NodeList.prototype.remove = HTMLCollection.prototype.remove = function() {
    for(var i = 0, len = this.length; i < len; i++) {
        if(this[i] && this[i].parentElement) {
            this[i].parentElement.removeChild(this[i]);
        }
    }
}

