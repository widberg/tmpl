#include "tmpl.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Must specify template and output file" << std::endl;
		return 1;
	}

	const std::filesystem::path template_path(argv[1]);
	const std::filesystem::path output_path(argv[2]);

	// Setup replacement values

	// Default macros
	macro_map_t macros = {
		{ "TEMPLATE_ARGUMENT", argv[1] },
		{ "OUTPUT_ARGUMENT", argv[2] },
		{ "TEMPLATE_DIRECTORY_ABSOLUTE", std::filesystem::absolute(template_path.parent_path()).string() },
		{ "OUTPUT_DIRECTORY_ABSOLUTE", std::filesystem::absolute(output_path.parent_path()).string() },
		{ "TEMPLATE_DIRECTORY_RELATIVE", std::filesystem::relative(template_path.parent_path()).string() },
		{ "OUTPUT_DIRECTORY_RELATIVE", std::filesystem::relative(output_path.parent_path()).string() },
		{ "WORKING_DIRECTORY", std::filesystem::current_path().string() },
		{ "$", "$" } // Escape sequence
	};

	// Add args
	for (int i = 3; i < argc; i++)
	{
		macros.insert(std::make_pair<std::string, std::string>(std::to_string(i - 3), argv[i]));
	}

	// Add times
	const std::time_t t = std::time(nullptr);
	const std::tm* time = std::localtime(&t);

	macros.insert({ "E", std::to_string(t) });

	const char time_format_specifiers[] = "aAbBcCdDeFgGhHIjmMprRSTuUVwWxXyYzZ";
	const int time_format_specifiers_size = sizeof(time_format_specifiers) / sizeof(char);

	char format_string[] = "% ";
	for (int i = 0; i < time_format_specifiers_size; i++)
	{
		char time_format_specifier = time_format_specifiers[i];
		std::stringstream buffer;
		format_string[1] = time_format_specifier;
		buffer << std::put_time(time, format_string);
		macros.insert({ std::string(1, time_format_specifier), buffer.str() });
	}

	return replace(template_path, output_path, macros);
}

int replace(const std::filesystem::path& template_path, const std::filesystem::path& output_path, macro_map_t& macros)
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

	// Output file

	std::filesystem::path new_output_path(replace_string(output_path.string(), macros));

	if (directory_mode)
	{


		if (!std::filesystem::is_directory(new_output_path) && std::filesystem::exists(new_output_path))
		{
			std::cerr << "Output path must be a directory or not exist in directory mode" << std::endl;
			return 1;
		}
	}
	else
	{
		output_file.open(new_output_path, std::ios::binary);
		if (output_file.fail())
		{
			std::cerr << "Could not open output file " << new_output_path << std::endl;
			template_file.close();
			return 1;
		}
	}

	if (directory_mode)
	{
		macros["TEMPLATE_DIRECTORY_ABSOLUTE"] = std::filesystem::absolute(template_path.parent_path()).string();
		macros["OUTPUT_DIRECTORY_ABSOLUTE"] = std::filesystem::absolute(new_output_path.parent_path()).string();
		macros["TEMPLATE_DIRECTORY_RELATIVE"] = std::filesystem::relative(template_path.parent_path()).string();
		macros["OUTPUT_DIRECTORY_RELATIVE"] = std::filesystem::relative(new_output_path.parent_path()).string();

		std::filesystem::directory_iterator end_itr;
		for (std::filesystem::directory_iterator itr(template_path); itr != end_itr; ++itr)
		{
			status = replace(itr->path(), new_output_path / itr->path().stem(), macros);
			if (status != 0)
			{
				break;
			}
		}
	}
	else
	{
		macros["TEMPLATE_ABSOLUTE"] = std::filesystem::absolute(template_path).string();
		macros["OUTPUT_ABSOLUTE"] = std::filesystem::absolute(new_output_path).string();
		macros["TEMPLATE_RELATIVE"] = std::filesystem::relative(template_path).string();
		macros["OUTPUT_RELATIVE"] = std::filesystem::relative(new_output_path).string();

		status = replace_stream(template_file, output_file, macros);

		template_file.close();
		output_file.close();
	}

	return status;
}

int replace_stream(std::istream& template_stream, std::ostream& output_stream, const macro_map_t& macros)
{
	char c;

	while (template_stream.get(c) && !template_stream.eof())
	{
		if (c == '$')
		{
			char next_char;
			template_stream.get(next_char);

			if (next_char == '(')
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

				if (!token.empty() && macros.count(token))
				{
					output_stream << macros.at(token);
				}
				else
				{
					std::cerr << "Invalid macro token" << std::endl;
					return 1;
				}
			}
			else if (std::isdigit(next_char))
			{
				std::string token;
				token.push_back(next_char);

				char token_char;

				while (template_stream.get(token_char) && !template_stream.eof() && std::isdigit(token_char))
				{
					token.push_back(token_char);
				}

				if (macros.count(token))
				{
					output_stream << macros.at(token);
				}
				else
				{
					std::cerr << "Invalid macro token" << std::endl;
					return 1;
				}

				if (!template_stream.eof())
				{
					output_stream << token_char;
				}
			}
			else
			{
				std::string char_as_string(1, next_char);

				if (macros.count(char_as_string))
				{
					output_stream << macros.at(char_as_string);
				}
				else
				{
					std::cerr << "Incomplete macro" << std::endl;
					return 1;
				}
			}
		}
		else
		{
			output_stream.put(c);
		}
	}
}

std::string replace_string(std::string& template_string, const macro_map_t& macros)
{
	std::ostringstream result;
	replace_stream(std::istringstream(template_string), result, macros);
	return result.str();
}
