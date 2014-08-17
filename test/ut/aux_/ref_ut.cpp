BOOST_AUTO_TEST_CASE(is_reference_wrapper_unwrap_reference) {
    BOOST_CHECK((is_reference_wrapper<std::reference_wrapper<int> >::value));
    BOOST_CHECK((is_same<unwrap_reference<std::reference_wrapper<int> >::type, int>::value));
    BOOST_CHECK((is_reference_wrapper<boost::reference_wrapper<int> >::value));
    BOOST_CHECK((is_same<unwrap_reference<boost::reference_wrapper<int> >::type, int>::value));
}
