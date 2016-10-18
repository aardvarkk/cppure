#include "cppure.h"

#include <fstream>
#include <set>
#include <vector>

using namespace std;

namespace cppure
{
	// If "a" is an impure word, ensure "a b" is not also a wordlist option
	// Otherwise, we will let "a" pass through if we see the combination "a c"
	vector<string> wordlist;

	enum state {
		WaitForWord,
		ProcessingWord
	};

	bool load_wordlist(string const& path)
	{
		wordlist.clear();

		ifstream ifs(path);
		if (!ifs.good()) return false;

		string word;
		while (ifs.good()) {
			getline(ifs, word);
			wordlist.push_back(word);
		}

		return true;
	}

	bool icase_match(string const& a, string const& b) {
		// Must be same length
		if (a.size() != b.size()) return false;

		// Check for full match between a and b in 'lowercase' sense
		for (int j = 0; j < a.size(); ++j) {
			if (tolower(a[j]) != tolower(b[j])) {
				return false;
			}
		}

		return true;
	}

	int icase_match_any(string const& buf, set<int> const& possible) {
		// We can process the remainder of the buffer now (the last word)
		for (int i : possible) {
			if (icase_match(buf, wordlist[i])) return i;
		}
		return -1;
	}

	string cleanse(string const& str)
	{
		state st = WaitForWord;

		string ret, buf;
		set<int> all_possible, possible;

		for (int i = 0; i < wordlist.size(); ++i)
			all_possible.insert(i);

		// Walk once through input string
		for (size_t idx = 0; idx < str.size(); ++idx) {

			char c = str[idx];

			// Is this character a space or punctuation or are we at eof?
			bool iss = isspace(c);
			bool isp = ispunct(c);

			// Still waiting for start of word...
			if (iss && (st == WaitForWord)) {
				ret += c;
				continue;
			}

			// Starting a word!
			if (!iss && (st == WaitForWord)) {
				st = ProcessingWord;
				possible = all_possible;
			}

			// Processing a word...
			if (st == ProcessingWord) {

				// If no more possibilities remain
				// then we can add what we have in the buffer to the output
				if (possible.empty()) {
					buf += c;

					// We can clear out the buffer and start over
					if (iss || isp) {
						ret += buf;
						buf.clear();
						st = WaitForWord;
					}

					continue;
				}

				// We've encountered a new space, punctuation, or eof character, so we process this word
				if (iss || isp) {

					// We match a possible word, so replace our buffer
					if (icase_match_any(buf, possible) >= 0) {
						fill(buf.begin(), buf.end(), '*');
					}

					buf += c;
					ret += buf;
					buf.clear();
					st = WaitForWord;
					continue;
				}

				// Don't yet have a match, so add the character
				buf += c;

				// Filter remaining possibilities based on current character
				for (auto it = possible.begin(); it != possible.end();) {
					// Current string is too long for match
					if (wordlist[*it].size() < buf.size()) {
						it = possible.erase(it);
						continue;
					}

					// Current character doesn't match (in lowercase)
					if (tolower(wordlist[*it][buf.size()-1]) != tolower(buf[buf.size()-1])) {
						it = possible.erase(it);
						continue;
					}

					// Next possible word
					++it;
				}
			}
		}

		// Process the last word
		if (icase_match_any(buf, possible) >= 0) {
			fill(buf.begin(), buf.end(), '*');
		}

		ret += buf;

		return ret;
	}
}
