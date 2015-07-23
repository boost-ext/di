//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
var cpp_code = Array();
var cpp_output = Array();

function get_cpp_file(file) {
     var cpp_file = new XMLHttpRequest();
     cpp_file.open("GET", file, false);
     cpp_file.send();
     return cpp_file.responseText;
}

function toggle(id, file) {
    document.getElementById("run_it_btn_" + id).firstChild.data = 'Run this code!';
    document.getElementById("run_it_btn_" + id).removeChild;
    document.getElementById("run_it_btn_" + id).onclick = function() { show(id, file); };
    document.getElementById("code_listing_" + id).style.display = 'block';
    cpp_code[id].toTextArea();
    document.getElementById("code_" + id).style.display = 'none';
    cpp_output[id].toTextArea();
    document.getElementById("output_" + id).style.display = 'none';
    document.getElementById("compile_and_run_" + id).remove();
    document.getElementById("powered_by_" + id).remove();
}

function show(id, file) {
    document.getElementById('run_it_btn_' + id).firstChild.data = 'Exit';
    document.getElementById("run_it_btn_" + id).onclick = function() { toggle(id, file); };
    document.getElementById("code_listing_" + id).style.display = 'none';
    var powered_by = document.createElement("text");
    var powered_text = document.createTextNode("Powered by Wandbox.");
    powered_by.appendChild(powered_text);
    powered_by.setAttribute("id", "powered_by_" + id);
    powered_by.setAttribute('style', 'font-size: 10px; text-align: right; display: block;');
    var compile_btn = document.createElement("BUTTON");
    var compile_txt = document.createTextNode("Compile & Run (Ctrl+Enter)");
    compile_btn.setAttribute("id", "compile_and_run_" + id);
    compile_btn.setAttribute("class", "TryItBtn");
    compile_btn.appendChild(compile_txt);
    compile_btn.onclick = function() { compile_and_run(id) };
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(compile_btn, document.getElementById("run_it_btn_" + id).nextSibling);
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(powered_by, compile_btn.nextSibling);
    get_example(id, file);
    compile_and_run(id);
}

function compile_and_run(id) {
    document.getElementById("compile_and_run_" + id).firstChild.data = "Compiling...";
    cpp_output[id].setValue("");
    var http = new XMLHttpRequest();
    http.open("POST", "http://melpon.org/wandbox/api/compile.json", true);
    http.onreadystatechange = function(){
        if (http.readyState == 4 && http.status == 200) {
            var output_json = JSON.parse(http.response);
            if ('status' in output_json && output_json.status == "0") {
                if ('program_message' in output_json) {
                    cpp_output[id].setValue(output_json.program_message);
                }
                cpp_output[id].setValue(cpp_output[id].getValue() + "\n-------\nExit: " + output_json.status);
            } else if ('compiler_error' in output_json) {
                cpp_output[id].setValue(output_json.compiler_error);
            } else if ('signal' in output_json) {
                if ('program_message' in output_json) {
                    cpp_output[id].setValue(output_json.program_message);
                }
            }
            document.getElementById("compile_and_run_" + id).firstChild.data = "Compile & Run (Ctrl+Enter)";
        }
    }

    http.send(
        JSON.stringify({
          "code" : cpp_code[id].getValue()
        , "codes" : [{
              "file" : "boost/di.hpp"
            , "code" : get_cpp_file("https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/include/boost/di.hpp")
           }]
         , "options": "warning,cpp-pedantic-errors,optimize,boost-nothing,c++14"
         , "compiler": "clang-head"
         , "compiler-option-raw": "-I."
    }));
}

function get_example(id, file) {
    cpp_code[id] = CodeMirror.fromTextArea(document.getElementById("code_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        mode: "text/x-c++src"
      });

    cpp_code[id].setSize(1100, 500);
    cpp_code[id].addKeyMap({"Ctrl-Enter": function(cm){ compile_and_run(id); }});

    cpp_output[id] = CodeMirror.fromTextArea(document.getElementById("output_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        readOnly : true,
        mode: "text/x-c++src"
      });

    cpp_output[id].setSize(1100, 200);
    cpp_output[id].setOption("theme", 'mdn-like');

    cpp_code[id].setValue(get_cpp_file(file));
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

