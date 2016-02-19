###0. [Pre] Refactor towards DI

If you write a new application you can just skip this step and go directly into step 1.
However, if you have a lot code which is not using DI and you wonder what can you do,
this step is for you.

Basically, there is a only one (big) step to get all benefits of Dependency Injection.
You have to separate creation logic from business logic, which means that your code
should be free of...

```cpp
class controller {
public:
  controller() : m(new model{}) {}
  ~controller() { delete model; }
  void run();

private:
  model* m;
};

int main() {
  controller controller_;
  controller_.run();
}
```

Instead, DI approach would look like that...

```cpp
class controller {
public:
  explicit controller(model& m) : model_(m) {}
  void run();

private:
  model& model_;
};

int main() {
  model model_;
  controller controller_{model_};
  controller_.run();
}
```

So, what happened here? We just took out dependencies from inside the `controller`. In other worlds,
we split creation logic and business logic.

That's basically everything you have to remember to create applications using DI.
Nevertheless, please, be careful and don't 'carry' your dependencies. What I mean by that,
is that do NOT pass an object into constructor if you don't store it ([Law of Demeter]).

```cpp
class app {
public:
  explicit app(model& m) : controller_(m) {}

private:
  controller controller_;
};
class controller {
public:
  explicit controller(model&);
};

int main() {
  model model_;
  app app_{model_};
}
```

###1. [Basic] Create objects tree

Before we will get into creating objects tree, let's first create a 'dummy' example.
In order to do so, firstly, we have to include (one and only) `boost/di.hpp` header and
declare a convienent di namespace alias.

```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

That is enough to try out `Boost.DI`!

To have a first complete working example we just have to add `main` function as usual.

```cpp
int main() {}
```

and compile our code using compiler supporting C++14 standard.

```
$CXX -std=c++14 example.cpp
```

Congrats, you are now ready to check out `Boost.DI` features!

---

Let's move on to creating objects tree. Application, usually, consists of a number of objects
which have to be instantiated. For example, let's consider a simplified Model View Controller code.

[![Create objects tree](images/create_objects_tree.png)](images/create_objects_tree.png)

The usual approach to create `app` would be following...

```cpp
button button_;
view view_{"", button_};
model model_;
controller controller_{model_, view_};
user user_;
app app_{controller_, user_};
```

Which is alright for a really small applications. However, it's really tedious to maintain.
Just imagine, that we have to change something here. For instance, `view` may need a new object `sprite`
or, even worse, we refactored the code and dependencies order has changed - yea ORDER of above is important!
Any change in these classes constructors require developer input to maintain above boilerplate!
Not fun, not fun at all :( 

Right now imagine that your maintain effort will be minimized almost to none. How does it sound?
Well, that might be achieved with `Boost.DI`!

The same result might be achieved with one liner. Doesn't matter how big the hierarchy will be.
We just have to create [injector] using [make_injector] and create the `app`.

```cpp
auto app_{make_injector().create<app>()};
```

Moreover, changes in the constructor of created objects will be handled automatically, so in our case
when we add a `sprite` into `view` required effort will be exactly 0. `Boost.DI` will take care of everything
for us!

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Boost.DI can inject dependencies using direct initialization `T(...)` or uniform initialization `T{...}` for aggregate types.
</div>

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_create_objects_tree.cpp)

Check out also other examples. Please, notice that the diagram was also generated using `Boost.DI` but we will get into that a bit later.

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)
![CPP(BTN)](Run_UML_Dumper_Extension|https://raw.githubusercontent.com/boost-experimental/di/cpp14/extension/policies/uml_dumper.cpp)

<br /><hr />

###2. [Basic] First steps with bindings

But objects tree is not everything. A lot of classes uses interfaces or required a value to be passed.
`Boost.DI` solution for this demand are [bindings].

For purpose of this tutorial, let's change `view` class into interface `iview` in order to support `text_view` and `gui_view`.

```cpp
class iview {
public:
  virtual ~iview() noexcept = default;
  virtual void render() =0;
};

class gui_view: public iview {
public:
  gui_view(std::string title, const button&) {}
  void render() {}
};

class text_view: public iview {
public:
  void render() {}
};
```cpp

Please, notice that text_view doesn't require any constructor parameters.

So, what will happen right now, when we try to create an `app`?

```cpp
auto app_{make_injector().create<app>()};
```

COMPILE error!

```cpp
basic_first_steps_with_bindings.cpp:61:12: warning: 'create<app>' is deprecated: creatable constraint not satisfied
  injector.create<app>();
           ^
../../include/boost/di.hpp:870:2: error: 'boost::di::v1_0_0::concepts::abstract_type<iview>::is_not_bound::error'
 error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?");
```

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
You can get more info about error by increasing BOOST_DI_CFG_DIAGNOSTICS_LEVEL [0-2] value (default=1).
</div>

Ah, okay, we haven't bound `iview` which means that `BOOST.DI` can't figure out whether we want `text_view` or `gui_view`?
It's really simple to fix it, we just use the suggestion provided.

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>()
);
```

Let's try again. Yay! It seems to be compiling.

But what about values? `button` requires `label` and `status`, which, by default, were zero initialized.

```cpp
struct button {
  std::string label;
  bool status;
};
```

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
If you want change the default behaviour and be sure that all required dependencies are bound and not zero initialized
take a look at `constructible` policy.
</div>

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to<gui_view>()
, di::bind<std::string>.to("button name") // button label
, di::bind<bool>.to(true) // button status
);
```

<div class="warning">
<h3><span class="fa fa-eye wy-text-neutral"></span>&nbsp; Note</h3>
Boost.DI is a compile time beast which means that it guarantees that if your code compiles that all dependencies will be resolved
correctly. No runtime exceptions, assert ever!
</div>

Check it out yourself!
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/tutorial/basic_first_steps_with_bindings.cpp)

Great, but my code is more dynamic then that! I mean that I want to choose `gui_view` or `text_view` at runtime.
`Boost.DI` can handle that too!

```cpp
auto injector = di::make_injector(
  di::bind<iview>.to
, di::bind<std::string>.to("button name") // button label
, di::bind<bool>.to(true) // button status
);
```

dynamic bindings

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

[bindings]: user_guide.md#bindings
[injector]: user_guide.md#make_injector
[make_injector]: user_guide.md#make_injector
[Law of Demeter]: https://en.wikipedia.org/wiki/Law_of_Demeter
