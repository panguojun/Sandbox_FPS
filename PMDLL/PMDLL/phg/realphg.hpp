// **************************************************************************
// Default Script
// **************************************************************************
#define GROUP			RealPHG
#define ELEMENT			real

namespace RealPHG
{
	#include "phg_head.hpp"

	inline void _PHGPRINT(const std::string& pre, real v)
	{
		PRINT(pre << ":" << v);
	}
	// ------------------------------------------
	#include "phg.hpp"
	// ------------------------------------------
	// 运算
	static var _act(code& cd, int args)
	{
		opr o = cd.oprstack.pop();

		PRINT("act:" << o << " args = " << args)

		switch (o) {
		case '+': {
			if (args > 1) {
				var b = cd.valstack.pop();
				var a = cd.valstack.pop();
				return a + b;
			}
			else {
				return cd.valstack.pop();
			}
		}
		case '-': {
			if (args > 1) {
				var b = cd.valstack.pop();
				var a = cd.valstack.pop();
				return a - b;
			}
			else {
				return -cd.valstack.pop();
			}
		}
		case '*': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a * b;
		}
		case '/': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a / b;
		}
		case '>': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a > b;
		}
		case '<': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a < b;
		}
		case '=': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a == b;
		}
		case '&': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a && b;
		}
		case '|': {
			var b = cd.valstack.pop();
			var a = cd.valstack.pop();
			return a || b;
		}
		case '!': {
			var a = cd.valstack.pop();
			return !a;
		}
		default: {return 0; }
		}
	}
	void setup();
}

// ------------------------------------------
// API
// ------------------------------------------
#define PARAM(index)	cd.valstack.get(args - index)
// math
static int rnd(RealPHG::code& cd, int args)
{
	if (args == 2)
	{
		real min = PARAM(1);
		real max = PARAM(2);

		return rrnd(min, max);
	}
	else
		return rrnd(0, 1);
	return 1;
}
static int bnd(RealPHG::code& cd, int args)
{
	real min = PARAM(1);
	real max = PARAM(2);
	real a = PARAM(3);

	real pw = 1;
	if (args >= 4)
		pw = PARAM(4);

	return blend(min, max, a, pw);
}
static int atan2(RealPHG::code& cd, int args)
{
	real y = PARAM(1);
	real x = PARAM(2);

	return atan2(y, x);
}
static int pow(RealPHG::code& cd, int args)
{
	real x = PARAM(1);
	real y = PARAM(2);

	return pow(x, y);
}

// logic
var test(RealPHG::code& cd, int args)
{
	PRINTV(args);
	var param1 = cd.valstack.get(args - 1);
	var param2 = cd.valstack.get(args - 2);
	var param3 = cd.valstack.get(args - 3);
	PRINTV(param1);
	PRINTV(param2);
	PRINTV(param3);

	return INVALIDVAR;
}

// -----------------------------------
// setup
// -----------------------------------
void RealPHG::setup()
{
	PRINT("setup RealPHG");

	act = _act;

	RealPHG::register_api("test", test);
}

// ====================================
// test
// ====================================
//void test233344()
//{
//	RealPHG::dofile("main.r");
//}