| Injector | Requirements | Description |
| ---------- | ------------ | ----------- |
| [di::make_injector](TDeps...\) | [di::concepts::boundable] <TDeps...\> | Creates [di::injector] |
| [di::make_injector]<TConfig\> (TDeps...) | [di::concepts::boundable]<TDeps...\> && [di::concepts::configurable]<TConfig\> | Creates [di::injector] with [di::config] |

| Bindings | Requirements | Description |
| ---------- | ------------ | ----------- |
| [di::bind]<Interface\>.to<Implementation\> | [di::concepts::boundable]<Interface, Implementation\> | Creates [di::bindings] |
| [di::bind]<T\>.to(value) | [di::concepts::boundable]<T, decltype(value)\> | Creates [di::bindings] |

| Injections | Requirements | Description |
| ---------- | ------------ | ----------- |
| struct Ctor { Ctor(TArgs...); }; | None | [default] Injects Ctor(TArgs...) |
| struct Ctor { [BOOST_DI_INJECT](Ctor, TArgs...\); }; | None | Injects Ctor(TArgs...) |
| struct Ctor { Ctor(TArgs...); [BOOST_DI_INJECT_TRAITS](Ctor, TArgs...\); }; | None | Injects Ctor(TArgs...) |

| Annotations | Requirements | Description |
| ---------- | ------------ | ----------- |
| struct Ctor { [BOOST_DI_INJECT](Ctor, (named = name) Type\) }; | is_object(name) | Injects Ctor(Type) where `Type` is named `name` |

| Scopes | Requirements | Description |
| ---------- | ------------ | ----------- |

| Modules | Requirements | Description |
| ---------- | ------------ | ----------- |

| Providers | Requirements | Description |
| ---------- | ------------ | ----------- |

| Policies | Requirements | Description |
| ---------- | ------------ | ----------- |

[di::injector]: user_guide.md#injector
[BOOST_DI_INJECT]: user_guide.md#boost_di_inject
[BOOST_DI_INJECT_TRAITS]: user_guide.md#boost_di_inject_traits
[di::make_injector]: user_guide.md#make_injector
[di::bind]: user_guide.md#bind
[di::bindings]: user_guide.md#bindings
[di::config]: user_guide.md#config
[di::concepts::boundable]: user_guide.md#boundable
[di::concepts::scopable]: user_guide.md#scopable
[di::concepts::callable]: user_guide.md#callable
[di::concepts::configurable]: user_guide.md#configurable
