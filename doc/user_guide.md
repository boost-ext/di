###Injector

***Header***

    #include <boost/msm-lite.hpp>

***Description***

Requirements for transition.

***Synopsis***

    template <class T>
    concept bool transitional() {
      return requires(T transition) {
        typename T::src_state;
        typename T::dst_state;
        typename T::event;
        typename T::deps;
        T::has_initial;
        { transition.execute() } -> bool;
      }
    }

***Semantics***

    transitional<T>

***Example***

    using namespace msm;
    auto transition = ("idle"_s == terminate);
    static_assert(transitional<decltype(transition)>::value);

![CPP(BTN)](Run_Transitional_Example|https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_transitional.cpp)

---

###Bindings
###Injections
###Annotations
###Scopes
###Modules
###Providers
###Policies
###Concepts
###Config
