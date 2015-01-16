template<class>
class requires_unique_bindings;

template<class... Ts>
class requires_unique_bindings<aux::type_list<Ts...>> {
    template<class T>
    struct expected {
        using type = aux::pair<
            aux::pair<typename T::expected, typename T::name>
          , std::integral_constant<bool, T::scope::priority>
        >;
    };

    pool<aux::type_list<typename expected<Ts>::type...>> bindings;
};


