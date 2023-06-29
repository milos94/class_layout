#include <string>

struct test_struct
{
    int a;
    bool b;
    std::string s;
};

struct first_base
{

};

struct second_base
{
    
};


struct third_base
{
    
};


struct derived : first_base, second_base, third_base
{
    first_base f;
};

struct composition
{
    [[no_unique_address]]first_base f{};
    [[no_unique_address]]second_base s{};
    [[no_unique_address]]third_base t{};
};




int main()
{
    return sizeof(composition) + sizeof(derived) + sizeof(test_struct);
}