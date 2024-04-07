
#include "Commands.hpp"

std::vector<std::string> split(std::string & str, char delim)
{
	std::vector<std::string> tokens;

	std::string token;

	std::istringstream tokenStream(str.c_str());

	while (std::getline(tokenStream, token, delim))
	{
		if (token[0] == ':')
		{
			token = token.substr(1);
			std::string final_token;
			while (std::getline(tokenStream, final_token, delim))
			{
				token += (std::string)" " + final_token;
			}
		}
		tokens.push_back(token);
	}
	return (tokens);
}

std::vector<std::string> basicSplit(std::string & str, char delim)
{
	std::vector<std::string> tokens;

	std::string token;

	std::istringstream tokenStream(str.c_str());

	while (std::getline(tokenStream, token, delim))
	{
		tokens.push_back(token);
	}
	return (tokens);
}

bool	caseInsensitiveCheck(std::string const & str, std::string const & str2)
{
	std::string strToUpper = str;
	std::transform(strToUpper.begin(), strToUpper.end(), strToUpper.begin(), ::toupper);

	std::string str2ToUpper = str2;
	std::transform(str2ToUpper.begin(), str2ToUpper.end(), str2ToUpper.begin(), ::toupper);

	if (strToUpper == str2ToUpper)
		return (true);
	return (false);
}

std::string intToString(int nb)
{
	std::ostringstream oss;
	oss << nb;
	return (oss.str());
}

std::string	getCurrentDateTime()
{
	time_t     now = time(0);
	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", localtime(&now));
	return buf;
}
