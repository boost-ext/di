struct My : Front::Base::Module
    <
        Externals::Bind         < MyClass, Named<int, dupa>                                             >,
        Scope<Singleton >::Bind < Impl1, Bind<Interface, Impl2>, Bind<Impl2>::InCall<C1, C2>           >,
        Scope<PerRequest>::Bind < int_<32>, mpl::string<'hej'>, Bind<Named<int, dupa>, int_<42> >       >,

        External<MyClass>,
        Singleton<Impl>::InCall<C1>,
        Singleton<I, Impl>::InCall<C1>,
        Singleton<I, Impl>::InCall<C1>,
        PerRequest<Impl>
    >
{ };

Impl<PerRequest, CIf0, CIf0, vector0<>, boost::is_base_of<boost::mpl::_1, CIf0> >

add to pool as a helper:
Pool
(
    set<I>(impl),
    set<int>(32),
    set<
);

