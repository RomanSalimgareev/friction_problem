#ifndef INPUT_FUNCS_H
#define INPUT_FUNCS_H

#include <iostream>
#include <string>
#include <limits>
#include <type_traits>

#include <MFE_lib/types/types.h>

namespace MFE
{
	template <typename T>
	void inputValue(T& value)
	{
		while (true)
		{
			std::string input = "";
			std::getline(std::cin, input);

			try
			{
				if constexpr (std::is_integral<T>::value)
				{
					UnsignedType pos = 0;
					T temp = std::stoi(input, &pos);
					if (pos != input.size())
						throw std::invalid_argument("Extra chars");
					value = static_cast<T>(temp);

					break;
				}
				else if constexpr (std::is_floating_point<T>::value)
				{ 
					UnsignedType pos = 0;
					T temp = std::stod(input, &pos);
					if (pos != input.size())
						throw std::invalid_argument("Extra chars");
					value = static_cast<T>(temp);

					break;
				}
				else
				{
					static_assert(false, "Unsupported type")
				}
			}
			catch (...)
			{
				std::cout << "Incorrect value, enter it again: \n";
			}
		}

		std::cout << "The value is set: " << value << "\n\n";
	}

	void inputChoice(char& choice);
}

#endif
