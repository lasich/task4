#include <stack>
#include <sstream>
#include <iomanip>
#include "format.h"

/*
* FormatParser parses a format string into tokens which are understandable for functions below
*/

namespace formatImpl
{

	class FormatParser
	{
	public:
		const char *const formatString;
		const char *next;

		FormatParser(const char *format) : next(format), formatString(format) { };

		void nextToken(Format &fmt)
		{
			if (*next == '%')
			{
				next++;
				//проверка для "%%"
				if (*next == '%')
				{
					fmt.str = "%";
					next++;
					return;
				}
				if (*next == '\0')
					throw std::invalid_argument("Invalid format: unexpected end of string");

				readFlags(fmt);
				//reading width
				if (isdigit(*next))
					fmt.width = readNumber();
				else if (*next == '*')
					next++, fmt.width = WP_READ;

				//reading precision
				if (*next == '.') {
					next++;

					if (isdigit(*next))
						fmt.precision = readNumber();
					else if (*next == '*')
						next++, fmt.precision = WP_READ;
					else
						fmt.precision = 0;  //"%.d" is equal to "%.0d"
				}

				readLength(fmt);
				readType(fmt);

			}
			else
			{
				//считывает строку до следующего спецификатора или конца строки
				while (*next != '%' && *next != '\0')
				{
					fmt.str.push_back(*next);
					next++;
				}
			}
		}

	private:
		void readFlags(Format &fmt) {
			bool ok = true;
			while (ok) {
				switch (*next) {
				case '0':
					if (!fmt.minus)     //"-" flag discards "0" flag;
						fmt.zero = true;
					break;
				case '+':
					fmt.plus = true;    //"+" flag discards " " flag;
					fmt.space = false;
					break;
				case '-':
					fmt.minus = true;   //"-" flag discards "0" flag;
					fmt.zero = false;
					break;
				case '#':
					fmt.sharp = true;
					break;
				case ' ':
					if (!fmt.plus)      //"+" flag discards " " flag;
						fmt.space = true;
					break;

				case '\0':
					throw std::invalid_argument("Invalid format: unexpected end of string");

				default:
					ok = false;
					next--;
				}
				next++;
			}
		}

		int readNumber() {
			int num = 0;
			while (isdigit(*next)) {
				num *= 10;
				if (num < 0)
					throw std::invalid_argument("Invalid format: overflow while parsing number");

				num += *next - '0';
				next++;
			}
			return num;
		}

		void readLength(Format &fmt) {
			switch (*next) {
			case 'l':
				if (next[1] == 'l')
					next++, fmt.length = ll;
				else
					fmt.length = l;
				break;
			case 'h':
				if (next[1] == 'h')
					next++, fmt.length = hh;
				else
					fmt.length = h;
				break;
			case 'j':
				fmt.length = f;
				break;
			case 'z':
				fmt.length = z;
				break;
			case 't':
				fmt.length = t;
				break;
			case 'L':
				fmt.length = L;

			case '\0':
				throw std::invalid_argument("Invalid format: unexpected end of format");
			default:
				next--;
				break;
			}
			next++;
		}

		void readType(Format &fmt) {
			switch (*next) {
			case 'i':
			case 'd':
				fmt.type = d;
				break;
			case 'u':
				fmt.type = u;
				break;
			case 'o':
				fmt.type = o;
				break;
			case 'x':
				fmt.type = x;
				break;
			case 'X':
				fmt.type = X;
				break;
			case 'f':
				fmt.type = f;
			case 'F':
				fmt.type = fmt.type == f ? f : F;
				if (fmt.precision == -1)
					fmt.precision = 6;
				break;
			case 'e':
				fmt.type = e;
				break;
			case 'E':
				fmt.type = E;
				break;
			case 'g':
				fmt.type = g;
				break;
			case 'G':
				fmt.type = G;
				break;
			case 'a':
				fmt.type = a;
				break;
			case 'A':
				fmt.type = A;
				break;
			case 'c':
				fmt.type = c;
				break;
			case 's':
				fmt.type = s;
				break;
			case 'p':
				fmt.type = p;
				break;
			case 'n':
				fmt.type = n;
				break;
			case '\0':
				throw std::invalid_argument("Unexpected end of format");
			default:
				/*throw std::invalid_argument(
					format("Invalid format: wrong format specifier %%%c in format \"%s\"", *next,
					formatString))*/;
			}
			next++;
		}
	};

	void parseFormatString(std::vector<Format> &fmt, const char *format) {
		FormatParser formatParser(format);
		while (*formatParser.next) {
			fmt.push_back(Format());
			formatParser.nextToken(fmt.back());
		}
	}

	void formatImplementation(Format *fmt, unsigned long size, std::string &str)
	{
		if (size)
		{
			if (fmt->str.size())
			{
				str += fmt->str;
				formatImplementation(fmt + 1, size - 1, str);
			}
			else
				throw std::out_of_range("Too few arguments");
		}
	}
}
