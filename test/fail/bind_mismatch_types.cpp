test bind_not_compatibile_types = [] {
    di::make_injector(di::bind<int, impl1>).create<int>();
};

