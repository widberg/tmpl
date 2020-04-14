#include "tmpl.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::string> macro_map_t;
typedef std::vector<std::string> arg_vector_t;

int replace(const std::filesystem::path& template_path, const std::filesystem::path& output_path, const std::tm* time, const macro_map_t& macros, const arg_vector_t& args);
int replace_stream(std::istream& template_path, std::ostream& output_path, const std::tm* time, const macro_map_t& macros, const arg_vector_t& args);
std::filesystem::path reparent_path(const std::filesystem::path& path, const std::filesystem::path& old_root, const std::filesystem::path& new_root);

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Must specify template and output file" << std::endl;
		return 1;
	}

	// Setup replacement values
	const std::time_t t = std::time(nullptr);
	const std::tm* time = std::localtime(&t);

	const macro_map_t macros = {
		{ "TEMPLATE", argv[1] },
		{ "OUTPUT", argv[2] }
	};

	const arg_vector_t args(argv + 3, argv + argc);

	return replace(argv[1], argv[2], time, macros, args);
}

int replace(const std::filesystem::path& template_path, const std::filesystem::path& output_path, const std::tm* time, const macro_map_t& macros, const arg_vector_t& args)
{
	bool directory_mode = false;
	std::ifstream template_file;
	std::ofstream output_file;

	int status = 0;

	// Template file

	if (std::filesystem::is_directory(template_path))
	{
		directory_mode = true;
	}
	else
	{
		template_file.open(template_path, std::ios::binary);
		if (template_file.fail())
		{
			std::cerr << "Could not open template file " << template_path << std::endl;
			return 1;
		}
	}

	// replace on template path to get output path

	// Output file

	if (directory_mode)
	{
		if (std::filesystem::exists(output_path))
		{
			std::cerr << "Output path must not exist in directory mode" << std::endl;
			return 1;
		}
	}
	else
	{
		output_file.open(output_path, std::ios::binary);
		if (output_file.fail())
		{
			std::cerr << "Could not open output file " << output_path << std::endl;
			template_file.close();
			return 1;
		}
	}

	if (directory_mode)
	{
		std::filesystem::directory_iterator end_itr;
		for (std::filesystem::directory_iterator itr(template_path); itr != end_itr; ++itr)
		{
			status = replace(itr->path(), reparent_path(itr->path(), template_path, output_path), time, macros, args);
			if (status != 0)
			{
				break;
			}
		}
	}
	else
	{
		status = replace_stream(template_file, output_file, time, macros, args);

		template_file.close();
		output_file.close();
	}

	return status;
}

int replace_stream(std::istream& template_stream, std::ostream& output_stream, const std::tm* time, const macro_map_t& macros, const arg_vector_t& args)
{
	char c;

	while (template_stream.get(c) && !template_stream.eof())
	{
		if (c == '$')
		{
			char next_char;
			template_stream.get(next_char);

			if (next_char == '$')
			{
				output_stream << "$";
			}
			else if (next_char == '(')
			{
				std::string token;

				char token_char;
				bool incomplete = true;

				while (template_stream.get(token_char) && !template_stream.eof())
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
					return 1;
				}

				if (!token.empty() && std::isdigit(token[0]))
				{
					int arg_index = std::stoi(token);

					if (arg_index < 0 || arg_index >= args.size())
					{
						std::cerr << "Invalid macro token: arg index out of range" << std::endl;
						return 1;
					}

					output_stream << args.at(arg_index);
				}
				else if (!token.empty() && macros.count(token))
				{
					output_stream << macros.at(token);
				}
				else if (!token.empty())
				{
					output_stream << std::put_time(time, token.c_str());
				}
				else
				{
					std::cerr << "Invalid macro token" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cerr << "Incomplete macro" << std::endl;
				return 1;
			}
		}
		else
		{
			output_stream.put(c);
		}
	}
}

std::filesystem::path reparent_path(const std::filesystem::path& path, const std::filesystem::path& old_root, const std::filesystem::path& new_root)
{

}
