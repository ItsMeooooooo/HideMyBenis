#include "argparser.h"

namespace HideMyBenis {
ArgParser::ArgParser(int argc, char* argv[])
{
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			int j = i;
			while(i + 1 < argc && argv[i + 1][0] != '-') { args[argv[j]].push_back(argv[++i]); }
			args[argv[j]];
		}
	}
}

ArgParser::~ArgParser() {}

ArgParser::ArgParser(const ArgParser& other): args(other.args) {}

ArgParser::ArgParser(ArgParser&& other) noexcept: args(std::move(other.args)) {}

ArgParser& ArgParser::operator=(const ArgParser& other)
{
	if(this == &other)
		return *this;
	args = other.args;
	return *this;
}

ArgParser& ArgParser::operator=(ArgParser&& other) noexcept
{
	if(this == &other)
		return *this;
	args = std::move(other.args);
	return *this;
}

bool ArgParser::exists(const std::string& arg) const
{
	return args.count("-" + arg) > 0;
}

bool ArgParser::exists(const std::string& arg, size_t num_elements)
{
	return args.count("-" + arg) > 0 && args.at("-" + arg).size() == num_elements;
}

bool ArgParser::existsAtLeast(const std::string& arg, size_t num_elements)
{
	return args.count("-" + arg) > 0 && args.at("-" + arg).size() >= num_elements;
}

size_t ArgParser::numParams(const std::string& arg)
{
	if(args.count("-" + arg) > 0) { return args.at("-" + arg).size(); }
	return 0;
}

const std::vector<std::string>& ArgParser::operator[](const std::string& arg)
{
	return args.at("-" + arg);
}

void ArgParser::printDebugInfo(std::ostream& stream)
{
	for(auto& o: args)
	{
		stream << o.first << ":" << std::endl;
		for(auto& p: args.at(o.first))
		{
			stream << "\t" << p << std::endl;
		}
	}
}
}
