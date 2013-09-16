/**
 * @code
 * struct s { s(int); };
 * struct k { k(shared_ptr<s>); };
 * struct c (shared_ptr<s>, k); //evolution order is not specified
 * injector<
 *     bind<int>::in_call<k>::to(i);
 *   , singleton<s>
 * >().create<c>();
 * @endcode
 */

