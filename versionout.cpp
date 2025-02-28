#include <iostream>
#include <filesystem>

int main()
{
	auto currpth = std::filesystem::current_path();
	std::cout << "currpath :%s\n" << currpth.c_str() << std::endl;
	std::cout << __cplusplus << std::endl;
	return 0;
}
