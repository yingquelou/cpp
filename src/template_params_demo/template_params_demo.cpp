// template.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "template_params.hpp"
int main()
{
	using type = typename tuple_init<int,
									 const char, int,
									 const char, int,
									 const char, int,
									 char, short>::type;
	type t;
	std::cout << typeid(type).name() << '\n';
	using type2 = typename tuple_n<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>::type;
	std::cout << typeid(type2).name() << '\n';
	using type3 = typename tuple_x<
		std::tuple<long, unsigned, int, char, double>,
		std::tuple<int, char, float, long, double>>::type;
	std::cout << typeid(type3).name() << '\n';
}