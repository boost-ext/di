All code examples include `boost/di.hpp` as well as declare a convienent msm namespace alias.

```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

###0. [Pre] Refactor towards DI

NOT -> carry dependencies

###1. [Basic] Create objects tree

[![Design](images/uml_dumper.png)](images/uml_dumper.png)

auto injector = make_injector();
injector.create<tree>();

###2. [Basic] First steps with bindings

bind interface
bind value
dynamic bindings

object creation ggurantee
understand error messages

###3. [Basic] Decide the life times

deduce scope
unique scope
singleton scope

###4. [Basic] Annotations to the rescue

ambigious constructors
named name

###5. [Basic] Split your configuration

auto module = [] { ... };

###6. [Advanced] Dump/Limit your types

uml dumper
limit types

###7. [Advanced] Customize it

custom scope
custom provider
custom policies

###8. [Advanced] Extend it

extensions

