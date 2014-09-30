template<typename T>
struct type__ { static void id() { } };

template<typename T>
std::size_t type_id() { return reinterpret_cast<std::size_t>(&type__<T>::id); }

