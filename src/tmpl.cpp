#include "tmpl.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Must specify template and output file" << std::endl;
		return 1;
	}

	// Setup replacement values

	const std::string template_argument(argv[1]);
	const std::string output_argument(argv[2]);

	const std::filesystem::path template_path(template_argument);
	const std::filesystem::path output_path(output_argument);

	bool template_stdin = template_argument == "--stdin";
	bool output_stdout = output_argument == "--stdout";

	// Default macros
	macro_map_t macros = {
		{ "TEMPLATE_ARGUMENT", template_argument },
		{ "OUTPUT_ARGUMENT", output_argument },
		{ "WORKING_DIRECTORY", std::filesystem::current_path().string() },
		{ "$", "$" } // Escape sequence
	};

	// Add args
	for (int i = 3; i < argc; i++)
	{
		macros.insert({ std::to_string(i - 3), argv[i] });
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

	// Do replacement

	int status = 0;

	if (std::filesystem::is_directory(template_path))
	{
		status = replace_directory(template_path, output_path, macros);
	}
	else
	{
		std::ifstream template_file;
		std::ofstream output_file;

		if (!template_stdin)
		{
			template_file.open(template_path, std::ios::binary);
			if (template_file.fail())
			{
				std::cerr << "Could not open template file " << template_path << std::endl;
				return 1;
			}
		}

		if (!output_stdout)
		{
			std::filesystem::path new_output_path(replace_filename(output_path.string(), macros));
			output_file.open(new_output_path, std::ios::binary);
			if (output_file.fail())
			{
				std::cerr << "Could not open output file " << new_output_path << std::endl;
				template_file.close();
				return 1;
			}
		}

		std::istream& template_stream = template_stdin ? std::cin : template_file;
		std::ostream& output_stream = output_stdout ? std::cout : output_file;

		status = replace_stream(template_stream, output_stream, macros);
	}

	return status;
}

int replace_directory(const std::filesystem::path& template_path, const std::filesystem::path& output_path, macro_map_t& macros)
{
	std::filesystem::path new_output_path(replace_filename(output_path.string(), macros));

	macros["TEMPLATE_DIRECTORY_ABSOLUTE"] = std::filesystem::absolute(template_path).string();
	macros["OUTPUT_DIRECTORY_ABSOLUTE"] = std::filesystem::absolute(new_output_path).string();
	macros["TEMPLATE_DIRECTORY_RELATIVE"] = std::filesystem::relative(template_path).string();
	macros["OUTPUT_DIRECTORY_RELATIVE"] = std::filesystem::relative(new_output_path).string();

	int status = 0;

	std::filesystem::directory_iterator end_itr;
	for (std::filesystem::directory_iterator itr(template_path); itr != end_itr && status == 0; ++itr)
	{
		std::filesystem::path canonical_template_path = itr->path();
		std::filesystem::path canonical_output_path = new_output_path / replace_filename(itr->path().stem().string(), macros);

		if (std::filesystem::is_directory(canonical_template_path))
		{
			status = replace_directory(canonical_template_path, canonical_output_path, macros);
		}
		else
		{
			macros["TEMPLATE_ABSOLUTE"] = std::filesystem::absolute(canonical_template_path).string();
			macros["OUTPUT_ABSOLUTE"] = std::filesystem::absolute(canonical_output_path).string();
			macros["TEMPLATE_RELATIVE"] = std::filesystem::relative(canonical_template_path).string();
			macros["OUTPUT_RELATIVE"] = std::filesystem::relative(canonical_output_path).string();

			std::ifstream template_file(canonical_template_path, std::ios::binary);
			if (template_file.fail())
			{
				std::cerr << "Could not open template file " << canonical_template_path << std::endl;
				return 1;
			}

			std::ofstream output_file(canonical_output_path, std::ios::binary);
			if (output_file.fail())
			{
				std::cerr << "Could not open output file " << new_output_path << std::endl;
				template_file.close();
				return 1;
			}

			status = replace_stream(template_file, output_file, macros);
		}
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

std::string replace_filename(const std::string& template_string, macro_map_t& macros)
{
	// Can't use file system macro in file name
	std::string file_macro_names[] = { "TEMPLATE_DIRECTORY_ABSOLUTE", "OUTPUT_DIRECTORY_ABSOLUTE", "TEMPLATE_DIRECTORY_RELATIVE", "OUTPUT_DIRECTORY_RELATIVE", "TEMPLATE_ABSOLUTE", "OUTPUT_ABSOLUTE", "TEMPLATE_RELATIVE", "OUTPUT_RELATIVE" };
	constexpr int file_macro_names_size = sizeof(file_macro_names) / sizeof(std::string);

	for (int i = 0; i < file_macro_names_size; i++)
	{
		macros.erase(file_macro_names[i]);
	}

	std::ostringstream result;
	replace_stream(std::istringstream(template_string), result, macros);
	return result.str();
}
