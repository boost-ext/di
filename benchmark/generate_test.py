#!/usr/bin/env python
#
# Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
import os

def base(output, x, b = 0, e = 0):
  o = "struct C" + str(x) + " {"
  o += "  C" + str(x) + "("
  for i in range(b, e):
    o += "C" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def base_instance(output, x, b = 0, e = 0):
  o = "C" + str(x) + " C" + str(x) + "_{"
  for i in range(b, e):
    o += "C" + str(i) + "_" + ("," if i < e - 1 else "")
  o += "};"
  output.append(o)

def hypodermic(output, x, b = 0, e = 0):
  o = "struct C" + str(x) + " {"
  o += "  C" + str(x) + "("
  for i in range(b, e):
    o += "const std::shared_ptr<C" + str(i) + ">&" + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def di(output, x, b = 0, e = 0):
  o = "struct C" + str(x) + " {"
  o += "  BOOST_DI_INJECT(C" + str(x) + ","
  for i in range(b, e):
    o += "C" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++g; } };"
  output.append(o)

def fruit(output, x, b = 0, e = 0):
  o = "struct C" + str(x) + " {"
  o += "  INJECT(C" + str(x) + "("
  for i in range(b, e):
    o += "C" + str(i) + ("," if i < e - 1 else "")
  o += ")) { ++g; } };"
  output.append(o)

def javax(output, x, b = 0, e = 0):
  o = "class C" + str(x) + " {"
  o += " @Inject C" + str(x) + "("
  for i in range(b, e):
    o += "C" + str(i) + " arg" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++Global.g; } }"
  output.append(o)

def csharp(output, x, b = 0, e = 0):
  o = "class C" + str(x) + " {"
  o += " public C" + str(x) + "("
  for i in range(b, e):
    o += "C" + str(i) + " arg" + str(i) + ("," if i < e - 1 else "")
  o += ") { ++Global.g; } }"
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

def create_cpp_test(types, ctor_args, iterations, expected, file, struct, header, module, main, impl):
  with open(file, 'w') as f:
    f.write(header)
    f.write("volatile unsigned long g = 0;\n")
    for o in create_data(types, ctor_args, struct):
      f.write(o + "\n")
    f.write("\n" + module + "\n")
    f.write("int main() {\n")
    f.write(main)
    f.write("for (auto i = 0; i < " + str(iterations) + "; ++i) {")
    f.write(impl)
    f.write("};\n")
    f.write("return " + str(expected) + " != g;\n")
    f.write("}\n")

def create_java_test(types, ctor_args, iterations, expected, file, struct, header, module, main, impl):
  with open(file, 'w') as f:
    f.write(header)
    f.write("class Global { public static long g = 0; }\n")
    for o in create_data(types, ctor_args, struct):
      f.write(o + "\n")
    f.write("\n" + module + "\n")
    f.write("\npublic class " + os.path.splitext(file)[0] + " {\n")
    f.write("\npublic static void main(String[] args) {\n")
    f.write(main)
    f.write("\nfor (int i = 0; i < " + str(iterations) + "; ++i) {\n")
    f.write(impl)
    f.write("}\nSystem.exit(" + str(expected) + "!= Global.g ? 1 : 0);")
    f.write("\n}\n}")

def create_csharp_test(types, ctor_args, iterations, expected, file, struct, header, module, main, impl):
  with open(file, 'w') as f:
    f.write(header)
    f.write("class Global { public static long g = 0; }\n")
    for o in create_data(types, ctor_args, struct):
      f.write(o + "\n")
    f.write("\n" + module + "\n")
    f.write("\nclass " + os.path.splitext(file)[0] + " {\n")
    f.write("\nstatic int Main(string[] args) {\n")
    f.write(main)
    f.write("\nfor (int i = 0; i < " + str(iterations) + "; ++i) {\n")
    f.write(impl)
    f.write("}\nreturn " + str(expected) + " != Global.g ? 1 : 0;")
    f.write("\n}\n}")

if os.environ['TEST'] == "unique":

    if os.environ['FRAMEWORK'] == "guice":
      create_java_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "guice.java"
      , struct = javax
      , header = "import com.google.inject.*;\n"
      , module = "class Module extends AbstractModule { @Override protected void configure() { } }"
      , main = "Injector injector = Guice.createInjector(new Module());"
      , impl = "injector.getInstance(C0.class);"
      )

    if os.environ['FRAMEWORK'] == "dagger2":
      create_java_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "dagger2.java"
      , struct = javax
      , header = "import dagger.*;\nimport javax.inject.Inject;\n"
      , module = "@Module class AppModule { } @Component(modules = AppModule.class) interface AppComponent { C0 build(); }"
      , main = ""
      , impl = "Dagger_AppComponent.create().build();"
      )

    if os.environ['FRAMEWORK'] == "ninject":
      create_csharp_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "ninject.cs"
      , struct = csharp
      , header = "using Ninject;\n"
      , module = "class Module : Ninject.Modules.NinjectModule { public override void Load() { } }"
      , main = "Ninject.IKernel kernel = new StandardKernel(new Module());"
      , impl = "kernel.Get<C0>();"
      )

    if os.environ['FRAMEWORK'] == "base":
      create_cpp_test(
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

    if os.environ['FRAMEWORK'] == "di":
      create_cpp_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "di.cpp"
      , struct = di
      , header = "#include <boost/di.hpp>\nnamespace di = boost::di;"
      , module = ""
      , main = "auto injector = di::make_injector();"
      , impl = "injector.create<C0>();"
      )

    if os.environ['FRAMEWORK'] == "fruit":
      create_cpp_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "fruit.cpp"
      , struct = fruit
      , header = "#include <fruit/fruit.h>\n"
      , module = "fruit::Component<C0> module() { return fruit::createComponent(); }"
      , main = ""
      , impl = "fruit::Injector<C0> injector{module()}; injector.get<C0>();"
      )

    if os.environ['FRAMEWORK'] == "hypodermic":
      create_cpp_test(
        types = int(os.environ['TYPES'])
      , ctor_args = int(os.environ['CTOR_ARGS'])
      , iterations = int(os.environ['ITERATIONS'])
      , expected = int(os.environ['ITERATIONS']) * (int(os.environ['TYPES']) + 1)
      , file = "hypodermic.cpp"
      , struct = hypodermic
      , header = "#include <Hypodermic/ContainerBuilder.h>\n"
      , module = ""
      , main = "Hypodermic::ContainerBuilder builder; auto injector = builder.build();"
      , impl = "injector->resolve<C0>();"
      )
