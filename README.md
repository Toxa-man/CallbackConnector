# Callback Connector  
## Description
Tiny **header-only** library that allows passing member function and lambdas as C-Api callbacks  
## Usage  
1. Include `CallbackConnector.h` into your project
2. Use `cbc::obtain_connector` function to retrieve C-Api function pointer
3. Pass obtained pointer as callback function
### Example:
```C++
#include <iostream>
#include "CallbackConnector.h"

//callback type
using cb_t = void(*)();

//function that takes pointer to function as callback
void c_api_func_with_callback(cb_t cb)
{
    cb();
}

//class mith members you want to pass
struct Foo
{
    void some_member() {std::cout << "some_member " << std::endl;}
    void const_member() const {std::cout << "const_member" << std::endl;}
    void another_member_with_same_signature() {std::cout << "another_member_with_same_signature" << std::endl;}
};

int main()
{
    Foo obj;
    using namespace cbc;
    //you can omit non-type Tag parameter if you are using member function with unique signature
    auto func_ptr = obtain_connector(&obj, &Foo::some_member);
    c_api_func_with_callback(func_ptr);


    Foo obj2;
    //use Tag parameter to disambiguate call
    auto new_func_ptr = obtain_connector<1>(&obj2, &Foo::another_member_with_same_signature);
    c_api_func_with_callback(new_func_ptr);


    //you can use const member functions as well
    auto const_ptr = obtain_connector(&obj2, &Foo::const_member);
    c_api_func_with_callback(const_ptr);


    //usage with capturing lambda
    std::string str = "it works";
    auto capturing_closure_ptr = obtain_connector([str](){ std::cout << str << std::endl;});
    c_api_func_with_callback(capturing_closure_ptr);

    return 0;
}
```
