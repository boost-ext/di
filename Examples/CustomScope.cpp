#if 0
    class Singleton : public Scope< std::shared_ptr<boost::mpl::_1> >
    {
    public:
        template<typename T, typename... Args>
        std::shared_ptr<T> create(Args&&... p_args)
        {
            if (!m_instance)
            {
                m_instance = std::make_shared(std::forward<Args>(p_args)...);
            }

            return m_instance;
        }

    private:
        std::shared_ptr<void> m_instance;
    };

#endif
int main()
{
    return 0;
}

