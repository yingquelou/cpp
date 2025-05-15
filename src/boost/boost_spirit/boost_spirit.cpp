#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <iterator>
namespace aaa
{
    using namespace boost::spirit;
    template <typename Iterator>
    class fourComputer : public qi::grammar<Iterator, double,qi::locals<double>>
    {
    private:
        qi::rule<Iterator, double,qi::locals<double>> exp;

    public:
        fourComputer() : fourComputer::base_type(exp)
        {
            using qi::_1;
            using qi::_2;
            using qi::_val;
            using qi::lit;
            using qi::_a;
            exp = qi::double_
            ;
        }
    };

} // namespace aaa

int main(int argc, char const *argv[])
{
    using aaa::fourComputer;
    char str[] = "1.0";
    fourComputer<char *> f;
    double r;
    boost::spirit::qi::parse((char *)str, (char *)str + sizeof(str), f, r);
    std::cout << r;
    return 0;
}
