// template.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "types.hpp"
#include <iostream>
int main()
{
	using type = types::types_u_t<std::tuple<>, std::tuple<int,
														   const char, int,
														   const char, int,
														   const char, int,
														   char, short>>;
	std::cout << typeid(type).name() << '\n';
	using type2 = types::types_n_t<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>;
	std::cout << typeid(type2).name() << '\n';
	using type3 = types::types_x_t<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>;
	std::cout << typeid(type3).name() << '\n';

	using input_types = types::types<int, double, char, float, long>;
	using filtered = types::filter_t<std::is_integral, input_types>;

	// 验证结果，filtered 应该是 types<int, char, long>
	std::cout << std::boolalpha;
	std::cout << std::is_same<filtered, types::types<int, char, long>>::value << std::endl; // 输出 true
}