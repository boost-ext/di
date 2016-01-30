> Let's assume all examples below include `boost/di.hpp` header and define `di` namespace alias.
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
//
struct i1 { virtual ~i1() = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };
struct impl : i1, i2 { void dummy1() override { } void dummy2() override { } };
```
| Type | Deduced scope |
|------|-------|
| T | unique |
| T& | singleton |
| const T& | unique (temporary)/singleton |
| T* | unique (ownership transfer) |
| const T* | unique (ownership transfer) |
| T&& | unique |
| unique\_ptr<T> | unique |
| shared\_ptr<T> | singleton |
| weak\_ptr<T> | singleton |

| Type/Scope | unique | singleton | external |
|------------|--------|--------|-----------|---------|----------|
| T | ✔ | - | ✔ |
| T& | - | ✔  | ✔ |
| const T& | ✔ (temporary) | ✔ | ✔ |
| T* (transfer ownership) | ✔ | - | - | - | ✔ |
| const T* | ✔ | - | ✔ |
| T&& | ✔ | - | - |
| unique\_ptr<T> | ✔ |  - | ✔ |
| shared\_ptr<T> | ✔ | ✔ | ✔ |
| weak\_ptr<T> | - | ✔ | ✔ |
