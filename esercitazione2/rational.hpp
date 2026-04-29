#include <iostream>
#include "rational.hpp"

template<typename I> requires std::integral<I>
class rational
{
	I num_;
	I den_;
};