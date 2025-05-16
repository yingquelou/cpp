// template.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "types.hpp"
int main()
{
	using type = types::tuple_u_t<std::tuple<>, std::tuple<int,
														   const char, int,
														   const char, int,
														   const char, int,
														   char, short>>;
	type t;
	std::cout << typeid(type).name() << '\n';
	using type2 = types::tuple_n_t<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>;
	std::cout << typeid(type2).name() << '\n';
	using type3 = types::tuple_x_t<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>;
	std::cout << typeid(type3).name() << '\n';
}