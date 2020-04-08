#ifndef CALLBACKCONNECTOR_H
#define CALLBACKCONNECTOR_H

#include <functional>

namespace cbc {
namespace Details {

template<std::size_t Tag, typename T, typename Ret, typename... Args>
class FuncMemberWrapper
{
public:
    FuncMemberWrapper() = delete;
    FuncMemberWrapper(const FuncMemberWrapper&) = delete;
    FuncMemberWrapper(FuncMemberWrapper&&) = delete;
    using member_fun_t = Ret (T::*)(Args...);
    using const_member_fun_t = Ret (T::*)(Args...) const;
    static auto instantiate(T* t, member_fun_t ptr)
    {
        obj = t;
        member = ptr;
        return MetaCall;
    }

    static auto instantiate(T* t, const_member_fun_t ptr)
    {
        obj = t;
        const_member = ptr;
        return ConstMetaCall;
    }

private:
    static auto MetaCall(Args... args)
    {
        return (*obj.*member)(args...);
    }
    static auto ConstMetaCall(Args... args)
    {
        return (*obj.*const_member)(args...);
    }
    static T*                 obj;
    static member_fun_t       member;
    static const_member_fun_t const_member;
};
template<std::size_t Tag, typename T, typename Ret, typename... Args>
T* FuncMemberWrapper<Tag, T, Ret, Args...>::obj{};

template<std::size_t Tag, typename T, typename Ret, typename... Args>
typename FuncMemberWrapper<Tag, T, Ret, Args...>::member_fun_t
    FuncMemberWrapper<Tag, T, Ret, Args...>::member{};

template<std::size_t Tag, typename T, typename Ret, typename... Args>
typename FuncMemberWrapper<Tag, T, Ret, Args...>::const_member_fun_t
    FuncMemberWrapper<Tag, T, Ret, Args...>::const_member{};

template<typename Functor, typename Ret, typename... Args>
struct FunctorWrapper
{
public:
    FunctorWrapper() = delete;
    FunctorWrapper(const FunctorWrapper&) = delete;
    FunctorWrapper(FunctorWrapper&&) = delete;
    static auto instantiate(Functor fn)
    {
        m_functor = std::move(fn);
        return MetaCall;
    }

private:
    static std::function<Ret(Args...)> m_functor;

    static auto MetaCall(Args... args)
    {
        return m_functor(args...);
    }
};

template<typename Functor, typename Ret, typename... Args>
std::function<Ret(Args...)> FunctorWrapper<Functor, Ret, Args...>::m_functor;

template<typename Functor, typename Ret, typename T, typename... Args>
auto deducer(Functor obj, Ret (T::*)(Args...) const)
{
    return FunctorWrapper<Functor, Ret, Args...>::instantiate(std::move(obj));
}

template<typename Functor, typename Ret, typename T, typename... Args>
auto deducer(Functor obj, Ret (T::*)(Args...))
{
    return FunctorWrapper<Functor, Ret, Args...>::instantiate(std::move(obj));
}

} // namespace Details

template<std::size_t tag = 0, typename T, typename Ret, typename... Args, typename U>
auto obtain_connector(T* t, Ret (U::*ptr)(Args...))
{
    return Details::FuncMemberWrapper<tag, T, Ret, Args...>::instantiate(t, ptr);
}

template<std::size_t tag = 0, typename T, typename Ret, typename... Args, typename U>
auto obtain_connector(T* t, Ret (U::*ptr)(Args...) const)
{
    return Details::FuncMemberWrapper<tag, T, Ret, Args...>::instantiate(t, ptr);
}

template<typename Functor>
auto obtain_connector(Functor functor)
{
    return Details::deducer(std::move(functor), &Functor::operator());
}
} // namespace cbc

#endif // CALLBACKCONNECTOR_H
