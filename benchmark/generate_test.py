#!/usr/bin/env python

import os

def base(output, x, b = 0, e = 0):
  o = "struct c" + str(x) + " {"
  o += "  c" + str(x) + "("
  for i in range(b, e):
    o += "c" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def base_instance(output, x, b = 0, e = 0):
  o = "c" + str(x) + " c" + str(x) + "_{"
  for i in range(b, e):
    o += "c" + str(i) + "_" + ("," if i < e - 1 else "")
  o += "};"
  output.append(o)

def hypodermic(output, x, b = 0, e = 0):
  o = "struct c" + str(x) + " {"
  o += "  c" + str(x) + "("
  for i in range(b, e):
    o += "const std::shared_ptr<c" + str(i) + ">&" + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def di(output, x, b = 0, e = 0):
  o = "struct c" + str(x) + " {"
  o += "  BOOST_DI_INJECT(c" + str(x) + ","
  for i in range(b, e):
    o += "c" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def fruit(output, x, b = 0, e = 0):
  o = "struct c" + str(x) + " {"
  o += "  INJECT(c" + str(x) + "("
  for i in range(b, e):
    o += "c" + str(i) + ("," if i < e - 1 else "")
  o += ")) { ++g; } };"
  output.append(o)

def create_data(types, ctor_args, struct):
    output = []
    g = 1
    for i in range(0, types * 2):
      if i >= g:
        break
      if g > types:
        struct(output, i)
      else:
        struct(output, i, g, g + ctor_args)
        g = g + ctor_args
    return reversed(output)

def create_test(types, ctor_args, iterations, expected, file, struct, header, module, main, impl):
  with open(file, 'w') as f:
    f.write("#if !defined(CHECK_COMPILE_TIME)\n")
    f.write("#include <cassert>\n")
    f.write("#endif\n\n")
    f.write("#include \"benchmark.hpp\"\n")
    f.write(header)
    f.write("volatile unsigned long g = 0;\n")
    for o in create_data(types, ctor_args, struct):
      f.write(o + "\n")
    f.write("\n" + module + "\n")
    f.write("int main() {\n")
    f.write(main)
    f.write("benchmark_execution_speed([&]{")
    f.write("for (auto i = 0; i < " + str(iterations) + "; ++i) {")
    f.write(impl)
    f.write("}});\n")
    f.write("#if !defined(CHECK_COMPILE_TIME)\n")
    f.write("std::cout << g << std::endl; assert(" + str(expected) + " == g);\n")
    f.write("#endif\n")
    f.write("}\n")

if os.environ['TEST'] == "unique":
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
  , file = "base.cpp"
  , struct = base
  , header = ""
  , module = ""
  , main = ""
  , impl = '\n'.join(o for o in create_data(int(os.environ['TYPES']), int(os.environ['CTOR_ARGS']), base_instance))
  )
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
  , file = "di.cpp"
  , struct = di
  , header = "#include <boost/di.hpp>\nnamespace di = boost::di;"
  , module = ""
  , main = ""
  , impl = "auto injector = di::make_injector(); injector.create<c0>();"
  )
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
  , file = "fruit.cpp"
  , struct = fruit
  , header = "#include <fruit/fruit.h>\n"
  , module = "fruit::Component<c0> module() { return fruit::createComponent(); }"
  , main = ""
  , impl = "fruit::Injector<c0> injector{module()}; injector.get<c0>();"
  )
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
  , file = "hypodermic.cpp"
  , struct = hypodermic
  , header = "#include <Hypodermic/ContainerBuilder.h>\n"
  , module = ""
  , main = "Hypodermic::ContainerBuilder builder; auto injector = builder.build();"
  , impl = "injector->resolve<c0>();"
  )
elif os.environ['TEST'] == "shared":
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['TYPES']) + 1
  , file = "base.cpp"
  , struct = base
  , header = ""
  , module = ""
  , main = '\n'.join(o for o in create_data(int(os.environ['TYPES']), int(os.environ['CTOR_ARGS']), base_instance))
  , impl =  "auto& c = c0_;"
  )
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['TYPES']) + 1
  , file = "di.cpp"
  , struct = di
  , header = "#include <boost/di.hpp>\nnamespace di = boost::di;"
  , module = ""
  , main = "auto injector = di::make_injector();"
  , impl = "injector.create<c0&>();"
  )
  create_test(
    types = int(os.environ['TYPES'])
  , ctor_args = int(os.environ['CTOR_ARGS'])
  , iterations = int(os.environ['ITERATIONS'])
  , expected = int(os.environ['TYPES']) + 1
  , file = "fruit.cpp"
  , struct = fruit
  , header = "#include <fruit/fruit.h>\n"
  , module = "fruit::Component<c0> module() { return fruit::createComponent(); }"
  , main = "fruit::Injector<c0> injector{module()}; "
  , impl = "injector.get<c0&>();"
  )
