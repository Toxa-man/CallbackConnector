#include <iostream>
#include "CallbackConnector.h"

using namespace std;


using cb_t = void(*)();

void c_api_func_with_callback(cb_t cb)
{
    cb();
}


struct Foo
{
    void some_member() {std::cout << "some_member " << std::endl;}
    void const_member() const {std::cout << "const_member" << std::endl;}
    void another_member_with_same_signature() {std::cout << "another_member_with_same_signature" << std::endl;}
};

int main()
{
    Foo obj;

    //you can omit non-type Tag parameter if you are using member function with unique signature
    auto func_ptr = obtaintConnector(&obj, &Foo::some_member);
    c_api_func_with_callback(func_ptr);


    Foo obj2;
    //use Tag parameter to disambiguate call
    auto new_func_ptr = obtaintConnector<1>(&obj2, &Foo::another_member_with_same_signature);
    c_api_func_with_callback(new_func_ptr);


    //you can use const member functions as well
    auto const_ptr = obtaintConnector(&obj2, &Foo::const_member);
    c_api_func_with_callback(const_ptr);


    //you can use const member functions as well
    std::string str = "it works";
    auto capturing_closure_ptr = obtaintConnector([str](){ std::cout << str << std::endl;});
    c_api_func_with_callback(capturing_closure_ptr);

    return 0;
}
