test:
	@clang++ -DBOOST_DI_CFG_NO_PREPROCESSED_HEADERS -stdlib=libc++ -O1 -Werror -Wall -Wextra -pedantic -pedantic-errors -fsanitize=undefined -fsanitize=memory -Icommon -I../include ut/di_ut.cpp -std=c++1y && ./a.out
	#@clang++ -DBOOST_DI_CFG_NO_PREPROCESSED_HEADERS -Ofast -Werror -Wall -Wextra -pedantic -pedantic-errors -Icommon -I../include ut/di_ut.cpp -std=c++1y && ./a.out

all:
	@clang++ -O3 -I../include -DBOOST_DI_CFG_NO_PREPROCESSED_HEADERS -std=c++1y pt/di.cpp

