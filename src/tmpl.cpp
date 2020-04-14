#include "tmpl.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unordered_map>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Must specify template and output file" << std::endl;
		return 1;
	}

	std::ifstream template_file(argv[1], std::ios::binary);
	if (template_file.fail())
	{
		std::cerr << "Could not open template file " << argv[1] << std::endl;
		return 1;
	}

	std::ofstream output_file(argv[2], std::ios::binary);
	if (output_file.fail())
	{
		std::cerr << "Could not open output file " << argv[2] << std::endl;
		template_file.close();
		return 1;
	}

	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	std::unordered_map<std::string, std::string> macros = {
		{ "TEMPLATE", argv[1] },
		{ "OUTPUT", argv[2] }
	};

	char c;

	while (template_file.get(c) && !template_file.eof())
	{
		if (c == '$')
		{
			if (template_file.peek() == EOF)
			{
				std::cerr << "Incomplete macro" << std::endl;

				template_file.close();
				output_file.close();

				return 1;
			}

			char next_char;
			template_file.get(next_char);

			if (next_char == '$')
			{
				output_file << "$";
			}
			else if (next_char == '(')
			{
				std::string token;

				char token_char;
				bool incomplete = true;

				while (template_file.get(token_char) && !template_file.eof())
				{
					if (token_char != ')')
					{
						token.push_back(token_char);
					}
					else
					{
						incomplete = false;
						break;
					}
				}

				if (incomplete)
				{
					std::cerr << "Incomplete macro" << std::endl;

					template_file.close();
					output_file.close();

					return 1;
				}

				if (!token.empty() && std::isdigit(token[0]))
				{
					int arg_index = std::stoi(token) + 3;

					if (arg_index <= 2 || arg_index >= argc)
					{
						std::cerr << "Invalid macro token: arg index out of range" << std::endl;

						template_file.close();
						output_file.close();

						return 1;
					}

					output_file << argv[arg_index];
				}
				else if (!token.empty() && macros.count(token))
				{
					output_file << macros.at(token);
				}
				else if (!token.empty())
				{
					output_file << std::put_time(&tm, token.c_str());
				}
				else
				{
					std::cerr << "Invalid macro token" << std::endl;

					template_file.close();
					output_file.close();

					return 1;
				}
			}
			else
			{
				std::cerr << "Incomplete macro" << std::endl;

				template_file.close();
				output_file.close();

				return 1;
			}
		}
		else
		{
			output_file.put(c);
		}
	}

	template_file.close();
	output_file.close();

	return 0;
}
