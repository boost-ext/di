/**
 * @code
 * struct c { c(int*); };
 *
 * @code
 * singleton<int>
 * struct c { c(int&); }; // not ok
 *
 * singleton<int>
 * struct c { c(int&); }; // ok
 * struct d { d(shared_ptr<int>); }; // ok
 * @endcode
 */

