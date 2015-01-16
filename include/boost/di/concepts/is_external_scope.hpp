
template<class TExpected, class TGiven, class TScope>
class requires_external_concepts {
    static_assert(std::is_same<TExpected, TGiven>{}, "Explicit implementation type doesn't have sens for external scope");
    static_assert(std::is_same<TScope, scopes::deduce>{}, "External scope can not be bound explicitly in a different scope");
};
