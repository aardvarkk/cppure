set (CMAKE_CXX_STANDARD 11)

configure_file(wordlist.txt wordlist.txt COPYONLY)
add_executable(tests cppure.cpp tests.cpp)
