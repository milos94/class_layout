#include <string>
#include <stdexcept>

struct my_struct {
   int value_1_;
   bool is_value_1_set_;
   int value_2_;
   bool is_value_2_set_;

  private:
   double value_3_;
   bool is_value_3_set_{false};

  public:
   std::string name;

    double get_value_3() {
       if (!is_value_3_set_) {
           throw std::runtime_error("Value not set");
       }
       return value_3_;
   }

   void set_value_3(double value) {
       value_3_ = value;
       is_value_3_set_ = true;
   }
};

struct first_base {
   int b;
};

struct second_base {};

struct third_base {
   int c;
};

struct inheritance : public first_base, second_base, third_base {};

struct composition_without_no_unique_address {
   first_base f;
   second_base s;
   third_base t;
};

struct composition_with_no_unique_address {
   first_base f;
   [[no_unique_address]] second_base s;
   third_base t;
};

struct composition_with_no_unique_address_2 {
   first_base f;
   [[no_unique_address]] second_base s1;
   [[no_unique_address]] second_base s2;
   [[no_unique_address]] second_base s3;
   third_base t;
};

int main()
{
    return sizeof(my_struct) + sizeof(inheritance) + sizeof(composition_without_no_unique_address) + sizeof(composition_with_no_unique_address) + sizeof(composition_with_no_unique_address_2);
}