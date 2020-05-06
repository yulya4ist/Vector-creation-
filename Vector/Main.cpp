#include<vector>
#include"Vector.h"
#include<algorithm>
#include<string>
#include<iostream>


int main()
{
    mcv::Vector<double> vec(10,0);
	for (size_t i = 0; i < vec.Size(); i++)
	{
		vec.At(i) = i;
		std::cout << vec[i] << "\t";
	}
	std::cout << '\n';
	vec=+vec;
	vec=-vec;
	+vec = -vec;
	vec = vec*2;
	vec = 2. * vec;

	vec = vec + vec;

	vec = vec - 2.*vec;

	for (size_t i = 0; i < vec.Size(); i++)
	{
		std::cout << vec[i] << "\t";
	}



}