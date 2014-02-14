#ifndef BOOST_DI_PROVIDER_HPP
#define BOOST_DI_PROVIDER_HPP


namespace boost {
namespace di {

template<typename T>
class provider
{
public:
    virtual ~provider() { }
    virtual T get() const = 0;
};

}
}

#endif

