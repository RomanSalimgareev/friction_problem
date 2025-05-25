#include "inputFunctions/inputFunctions.h"

void MFE::inputChoice(char& choice)
{
	std::string input = "";
	while (true)
	{
		std::getline(std::cin, input);
		if (input.size() == 0 || input.size() > 1)
		{
			std::cout << "Incorrect selection, enter y/n: \n";
			continue;
		}
		else
		{
			if (input[0] == 'y' || input[0] == 'Y' ||
				input[0] == 'n' || input[0] == 'N')
			{
				choice = input[0];
				break;
			}
			else
			{
				std::cout << "Incorrect selection, enter y/n: \n";
				continue;
			}
		}
	}
}