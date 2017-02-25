#pragma once
#include <ostream>
#include <string>
#include <vector>
#include <map>

namespace HideMyBenis {
class ArgParser
{
public:
	ArgParser(int argc, char* argv[]);
	~ArgParser();

	ArgParser(const ArgParser& other);
	ArgParser(ArgParser&& other) noexcept;
	ArgParser& operator=(const ArgParser& other);
	ArgParser& operator=(ArgParser&& other) noexcept;

	bool exists(const std::string& arg) const;
	bool exists(const std::string& arg, size_t num_elements);
	bool existsAtLeast(const std::string& arg, size_t num_elements);
	size_t numParams(const std::string& arg);
	const std::vector<std::string>& operator[](const std::string& arg);

	void printDebugInfo(std::ostream& stream);

private:
	std::map<std::string, std::vector<std::string>> args;
};
}
