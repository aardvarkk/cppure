#include "cppure.h"

#include <cassert>

int main(int argc, char const* argv[])
{
	assert(cppure::load_wordlist("wordlist.txt"));
	assert(cppure::cleanse("ass") == "***");
	assert(cppure::cleanse("ass¡") == "***¡");
	assert(cppure::cleanse("¡ass¡") == "¡***¡");
	assert(cppure::cleanse("a.s.s") == "a.s.s");
	assert(cppure::cleanse("ASS") == "***");
	assert(cppure::cleanse("Ass") == "***");
	assert(cppure::cleanse("aSS") == "***");
	assert(cppure::cleanse(" ass") == " ***");
	assert(cppure::cleanse("  ass") == "  ***");
	assert(cppure::cleanse("ass.") == "***.");
	assert(cppure::cleanse("ass. ") == "***. ");
	assert(cppure::cleanse(" ass. ") == " ***. ");
	assert(cppure::cleanse(".ass.") == ".***.");
	assert(cppure::cleanse("ass!") == "***!");
	assert(cppure::cleanse("you ass") == "you ***");
	assert(cppure::cleanse("tit") == "***");
	assert(cppure::cleanse("title") == "title");
	assert(cppure::cleanse("ass tit") == "*** ***");
	assert(cppure::cleanse("fuck you!") == "**** you!");
	assert(cppure::cleanse("fuck you, cocksucker!") == "**** you, **********!");
}