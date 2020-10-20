#include "Roundelay.h"

Roundelay::Roundelay()
{
}

Roundelay::~Roundelay()
{
}


void Roundelay::Hop()
{
	std::cout << "-------------------------------------------------\n";
	std::cout << "The names you enter should be enclosed in quotes.\n";
	std::cout << "-------------------------------------------------\n";

	while (true)
	{
		char command[maxCommandLength];
		std::cout << '>';
		std::cin >> command;

		if (strcmp(command, "EXIT") == 0)
		{
			break;
		}
		else if (strcmp(command, "RELEASE") == 0)
		{
			char participant[32];
			char option[6];
			std::cin.ignore(10, '\"');
			std::cin.getline(participant, 32, '\"');
			std::cin >> option;

			std::cin.ignore(10000, '\n');

			if (strcmp(option, "left") == 0||
				strcmp(option, "right") == 0||
				strcmp(option, "both") == 0)
			{
				hs.ReleaseParticipant(participant, option);
			}
			else
			{
				std::cerr << "Options are \"left\", \"right\", \"both\".\n";
				std::cin.ignore(10000, '\n');
			}
		}
		else if (strcmp(command, "GRAB") == 0)
		{
			char participant[32];
			char option[6];
			std::cin.ignore(10, '\"');
			std::cin.getline(participant, 32, '\"');
			std::cin >> option;

			std::cin.ignore(10000, '\n');

			if (strcmp(option, "left") == 0 ||
				strcmp(option, "right") == 0 ||
				strcmp(option, "both") == 0)
			{
				hs.GrabParticipant(participant, option);
			}
			else
			{
				std::cerr << "Options are \"left\", \"right\", \"both\".\n";
				std::cin.ignore(10000, '\n');
			}
		}
		else if (strcmp(command, "INFO") == 0)
		{
			char participant[32]; 
			std::cin.ignore(10, '\"');
			std::cin.getline(participant, 32, '\"');

			std::cin.ignore(10000, '\n');

			hs.InfoAboutParticipant(participant);
		}
		else if (strcmp(command, "ADD") == 0)
		{
			char participant[32];
			char leftN[32];
			char rightN[32];
			std::cin.ignore(10, '\"');
			std::cin.getline(participant, 32,'\"');
			std::cin.ignore(10, '\"');
			std::cin.getline(leftN, 32, '\"');
			std::cin.ignore(10, '\"');
			std::cin.getline(rightN, 32, '\"');

			std::cin.ignore(10000, '\n');

			hs.AddParticipant(participant, leftN, rightN);
		}
		else if (strcmp(command, "REMOVE") == 0)
		{
			char participant[32];
			std::cin.ignore(10, '\"');
			std::cin.getline(participant, 32, '\"');

			std::cin.ignore(10000, '\n');

			hs.RemoveParticipant(participant);
		}
		else if (strcmp(command, "SWAP") == 0)
		{
			char participant1[32];
			char participant2[32];

			std::cin.ignore(10, '\"');
			std::cin.getline(participant1, 32, '\"');
			std::cin.ignore(10, '\"');
			std::cin.getline(participant2, 32, '\"');

			std::cin.ignore(10000, '\n');
			
			hs.SwapParticipants(participant1, participant2);
		}
		else if (strcmp(command, "PRINT") == 0)
		{
			hs.PrintHash();
		}
		else
		{
			std::cerr << "Wrong command!\n";
			std::cin.ignore(10000, '\n');
		}
	}
}
