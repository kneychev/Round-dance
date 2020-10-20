#include "Hash.h"

const int MAX_FILENAME_LENGTH = 256;

Hash::Hash() : count(0), tableSize(10), firstParticipant("")
{
	// allocating memory for the table
	participants.ResizeDynamicArray(10);
	
	// reading the name of the file
	char fileName[MAX_FILENAME_LENGTH];
	std::cout << "Enter the name of the file: ";
	std::cin.getline(fileName, MAX_FILENAME_LENGTH);
	
	// extracting the names of the participants and adding them to the hash table
	ReadNamesFromFile(fileName);

	// rehashing while we have a good count to table size ratio
	if (GetLoadFactor() > 0.75f)
	{
		ReHash(tableSize * 2);
	}
}

Hash::~Hash()
{
	FreeHash();
}

void Hash::ReleaseParticipant(const char* who, const char* op)
{
	int idx = HashFunction(who, tableSize);	// calculating the index of the person, who releases
	int sizeOfArrAtIdx = participants[idx].GetSize(); // getting the size of the array he/she is in
	
	// finding the object in the hash table
	for (int i = 0; i < sizeOfArrAtIdx; ++i)
	{
		if (strcmp(participants[idx][i].name, who) == 0)
		{
			// releasing, depending on option
			switch (op[0])
			{
			case 'l':
				participants[idx][i].holdingLeft = false; 
				return;
			case 'r':
				participants[idx][i].holdingRight = false;
				return;
			case 'b':
				participants[idx][i].holdingLeft = false;
				participants[idx][i].holdingRight = false;
				return;
			default:
				std::cout << "Options are \"left\", \"right\" or \"both\".\n";
				return;
			}
		}
	}

	std::cerr << who << " does not exist!\n";
}

void Hash::GrabParticipant(const char* who, const char* op)
{
	// calculating the table index of the array where the "who" is in
	int idx = HashFunction(who, tableSize); 

	// getting the size of the array where "who" is in
	int sizeAtIdx = participants[idx].GetSize(); 
	
	// finding the right object and grabbing the neighbour,
	// depending on the option
	for (int i = 0; i < sizeAtIdx; ++i)
	{
		if (strcmp(participants[idx][i].name, who) == 0)
		{
			switch (op[0])
			{
			case 'l':
				participants[idx][i].holdingLeft = true;
				return;
			case 'r':
				participants[idx][i].holdingRight = true;
				return;
			case 'b':
				participants[idx][i].holdingLeft = true;
				participants[idx][i].holdingRight = true;
				return;
			default:
				std::cout << "Options are \"left\", \"right\" or \"both\".\n";
				return;
			}
		}
	}

	std::cerr << who << " does not exist!\n";
}

void Hash::InfoAboutParticipant(const char* who)
{
	// calculating the table index of the array where "who" is contained
	int idx = HashFunction(who, tableSize);

	// getting the array size, where "who" is placed
	int currSize = participants[idx].GetSize();

	bool isHeldByLeft = false;
	bool isHeldByRight = false;

	Participant* par; // participant
	bool found = false;

	// finding the participant and "pointing" him/her
	for (int i = 0; i < currSize; ++i)
	{
		if (strcmp(participants[idx][i].name, who) == 0)
		{
			par = &participants[idx][i];
			found = true;
			break;
		}
	}
	
	if (!found)
	{
		std::cerr << who << " does not exist!\n";
		return;
	}
	
	// calculating the table index of the array where who's left neighbour is placed
	int leftNbrIdx = HashFunction(par->leftNeighbourName, tableSize);

	// getting the size of the array, where who's left neighbour is placed
	int leftNbrIdxSize = participants[leftNbrIdx].GetSize();

	// calculating the table index of the array where who's right neighbour is placed
	int rightNbrIdx = HashFunction(par->rightNeighbourName, tableSize);
	
	// getting the size of the array, where who's rightt neighbour is placed
	int rightNbrIdxSize = participants[rightNbrIdx].GetSize();

	// finding the left neighbour and checking if who is held by him/her
	for (int i = 0; i < leftNbrIdxSize; ++i)
	{
		if (strcmp(participants[leftNbrIdx][i].name, par->leftNeighbourName) == 0)
		{
			isHeldByLeft = participants[leftNbrIdx][i].holdingRight;
			break;
		}
	}

	// finding the right neighbour and checking if who is held by him/her
	for (int i = 0; i < rightNbrIdxSize; ++i)
	{
		if (strcmp(participants[rightNbrIdx][i].name, par->rightNeighbourName) == 0)
		{
			isHeldByRight = participants[rightNbrIdx][i].holdingLeft;
			break;
		}
	}

	// printing the participants and the connections between them
	std::cout << par->leftNeighbourName;
	
	if (par->holdingLeft == true) 
		std::cout << " <--";
	else 
		std::cout << " ---";

	if (isHeldByLeft == true)
		std::cout << "> ";
	else
		std::cout << "- ";

	std::cout << par->name;

	if (isHeldByRight)
		std::cout << " <--";
	else
		std::cout << " ---";

	if (par->holdingRight == true)
		std::cout << "> ";
	else
		std::cout << "- ";

	std::cout << par->rightNeighbourName << std::endl;
}

void Hash::AddParticipant(const char* who, const char* lN, const char* rN)
{
	// calculating the array's index in the table, where right neighbour is pushed
	int rightNbrIdx = HashFunction(rN, tableSize);
	// getting the size of the array, where the left neighbour is in.
	int rightNbrArrSize = participants[rightNbrIdx].GetSize();

	// calculating the array's index in the table, where left neighbour is pushed
	int leftNbrIdx = HashFunction(lN, tableSize);
	// getting the size of the array, where the left neighbour is in.
	int leftNbrArrSize = participants[leftNbrIdx].GetSize();

	Participant* rNbr = nullptr; // right neighbour
	Participant* lNbr = nullptr; // left neighbour

	// finding the right neighbour and "pointing" at him/her
	for (int i = 0; i < rightNbrArrSize; ++i)
	{
		if (strcmp(participants[rightNbrIdx][i].name, rN) == 0)
		{
			rNbr = &participants[rightNbrIdx][i];
			break;
		}
	}

	// finding the left neighbour and "pointing" at him/her
	for (int i = 0; i < leftNbrArrSize; ++i)
	{
		if (strcmp(participants[leftNbrIdx][i].name, lN) == 0)
		{
			lNbr = &participants[leftNbrIdx][i];
			break;
		}
	}

	// Adding the new participant, facing the right direction
	if (strcmp(lNbr->rightNeighbourName, rN) == 0 && strcmp(rNbr->leftNeighbourName, lN) == 0)
	{
		InsertNewParticipant(who, lN, rN);
	}
	else if (strcmp(lNbr->leftNeighbourName, rN) == 0 && strcmp(rNbr->rightNeighbourName, lN) == 0)
	{
		std::cerr << who << " will turn back to the others and won't be added!\n";
		return;
	}
	else
	{
		std::cerr << lN << " and " << rN << " are not neighbours!\n";
		return;
	}

	if (GetLoadFactor() > 0.75f)
	{
		ReHash(tableSize * 2);
	}
}

void Hash::RemoveParticipant(const char* who)
{
	// calculating the array index of the table, where "who" is positioned
	int idx = HashFunction(who, tableSize);
	// getting the size of the array, where "who" is positioned
	int sz = participants[idx].GetSize();
	char leftName[32] = "";
	char rightName[32] = "";
	int whoIdx; // the position of "who"

	// finding the position of the "who", also left and right neighbours
	for (whoIdx = 0; whoIdx < sz; ++whoIdx)
	{
		if (strcmp(participants[idx][whoIdx].name, who) == 0)
		{
			// if who holds any of his neighbours, he/she can't be removed
			if (participants[idx][whoIdx].holdingLeft == true ||
				participants[idx][whoIdx].holdingRight == true)
			{
				std::cerr << "This won't be so easy!\n";
				return;
			}

			if (strcmp(who, firstParticipant) == 0)
			{
				strcpy_s(firstParticipant,
					strlen(participants[idx][whoIdx].leftNeighbourName) + 1,
					participants[idx][whoIdx].leftNeighbourName);
			}

			// getting the name of the left neighbour
			strcpy_s(leftName,
				strlen(participants[idx][whoIdx].leftNeighbourName) + 1,
				participants[idx][whoIdx].leftNeighbourName);

			// getting the name of the right neighbour
			strcpy_s(rightName,
				strlen(participants[idx][whoIdx].rightNeighbourName) + 1,
				participants[idx][whoIdx].rightNeighbourName);

			break;
		}
	}

	// who is not found
	if (strcmp(leftName, "") == 0 && strcmp(rightName, "") == 0)
	{
		std::cerr << who << " does not exist!\n";
		return;
	}

	// calculating the array index of the table, where who's left neighbour is positioned
	int l = HashFunction(leftName, tableSize);
	// getting the size of the array, where "who" is positioned
	int lSize = participants[l].GetSize();
	// calculating the array index of the table, where who's right neighbour is positioned
	int r = HashFunction(rightName, tableSize);
	// getting the size of the array, where "who" is positioned
	int rSize = participants[r].GetSize();

	// finding the object who is considered to be the left neighbour
	int lftPos;
	for (lftPos = 0; lftPos < lSize; ++lftPos)
	{
		if (strcmp(participants[l][lftPos].name, leftName) == 0)
		{
			// if the left neighbour is holding who => not cool
			if (participants[l][lftPos].holdingRight == true)
			{
				std::cerr << "This won't be so easy!\n";
				return;
			}

			break;
		}
	}

	// finding the object who is considered to be the right neighbour
	int rhtPos;
	for (rhtPos = 0; rhtPos < rSize; ++rhtPos)
	{
		// if the right neighbour is holding who => we will can't remove who
		if (strcmp(participants[r][rhtPos].name, rightName) == 0)
		{
			if (participants[r][rhtPos].holdingLeft == true)
			{
				std::cerr << "This won't be so easy!\n";
				return;
			}

			break;
		}
	}
	// assingning the right neighbour, of who's left neighbour
	strcpy_s(participants[l][lftPos].rightNeighbourName, 
		strlen(rightName) + 1, 
		rightName);

	// grabbing the right neighbour
	participants[l][lftPos].holdingRight = true;

	// assingning the left neighbour, of who's right neighbour
	strcpy_s(participants[r][rhtPos].leftNeighbourName,
		strlen(leftName) + 1,
		leftName);

	// grabbing the left neighbour
	participants[r][rhtPos].holdingLeft = true;

	// removing "who
	participants[idx].RemoveAt(whoIdx);
	--count;
	std::cout << "Free at last!\n";

	if (count < 3)
	{
		std::cout << "...and the music stops!\n";
		exit(1);
	}
}

void Hash::SwapParticipants(const char* who1, const char* who2)
{
//// This function might be linear for small data, but if we have more participants
//// and bigger capacity the function will word with complexity w(n)

	// calculating the array's index in the table, where who1 is positioned
	int who1Idx = HashFunction(who1, tableSize);
	// getting the size of the array, where "who1" is positioned
	int who1ArrSize = participants[who1Idx].GetSize();
	
	// calculating the array's index in the table, where who2 is positioned
	int who2Idx = HashFunction(who2, tableSize);
	// getting the size of the array, where "who2" is positioned
	int who2ArrSize = participants[who2Idx].GetSize();

	Participant* w1 = nullptr;	// who1
	Participant* w2 = nullptr;	// who2
	Participant* w1R = nullptr; // who1's right neighbour
	Participant* w1L = nullptr; // who1's left neighbour
	Participant* w2R = nullptr; // who2's right neighbour
	Participant* w2L = nullptr; // who2's left neighbour

//////////////////////////////////////////////////////////////////////////////
// who1's things

	// finding who1 and "pointing" at him/her
	for (int i = 0; i < who1ArrSize; ++i)
	{
		if (strcmp(participants[who1Idx][i].name, who1) == 0)
		{
			w1 = &participants[who1Idx][i];
			break;
		}
	}

	if (w1 == nullptr)
	{
		std::cerr << who1 << " does not exist!\n";
		return;
	}

	// calculating the array's index in the table, where who1's left neighbour is positioned
	int who1LeftIdx = HashFunction(w1->leftNeighbourName, tableSize);
	// getting the size of the array, where who1's left neighbour is positioned
	int who1LeftArrSize = participants[who1LeftIdx].GetSize();

	// calculating the array's index in the table, where who1's right neighbour is positioned
	int who1RightIdx = HashFunction(w1->rightNeighbourName, tableSize);
	// getting the size of the array, where who1's right neighbour is positioned
	int who1RightArrSize = participants[who1RightIdx].GetSize();

	// finding who1's left neighbour and "pointing" at him/her
	for (int i = 0; i < who1LeftArrSize; ++i)
	{
		if (strcmp(participants[who1LeftIdx][i].name, w1->leftNeighbourName) == 0)
		{
			w1L = &participants[who1LeftIdx][i];
			break;
		}
	}

	// finding who1's left neighbour and "pointing" at him/her
	for (int i = 0; i < who1RightArrSize; ++i)
	{
		if (strcmp(participants[who1RightIdx][i].name, w1->rightNeighbourName) == 0)
		{
			w1R = &participants[who1RightIdx][i];
			break;
		}
	}

///////////////////////////////////////////////////////////////////////////////
// who2's things
	
	// finding who2 and "pointing" at him/her
	for (int i = 0; i < who2ArrSize; ++i)
	{
		if (strcmp(participants[who2Idx][i].name, who2) == 0)
		{
			w2 = &participants[who2Idx][i];
			break;
		}
	}

	if (w2 == nullptr)
	{
		std::cerr << who2 << " does not exist!\n";
		return;
	}

	// calculating the array's index in the table, where who2's left neighbour is positioned
	int who2LeftIdx = HashFunction(w2->leftNeighbourName, tableSize);
	
	// getting the size of the array, where who2's right neighbour is positioned
	int who2LeftArrSize = participants[who2LeftIdx].GetSize();

	// calculating the array's index in the table, where who2's left neighbour is positioned
	int who2RightIdx = HashFunction(w2->rightNeighbourName, tableSize);
	
	// getting the size of the array, where who2's right neighbour is positioned
	int who2RightArrSize = participants[who2RightIdx].GetSize();

	// finding who2's left neighbour and "pointing" at him/her
	for (int i = 0; i < who2LeftArrSize; ++i)
	{
		if (strcmp(participants[who2LeftIdx][i].name, w2->leftNeighbourName) == 0)
		{
			w2L = &participants[who2LeftIdx][i];
			break;
		}
	}

	// finding who2's right neighbour and "pointing" at him/her
	for (int i = 0; i < who2RightArrSize; ++i)
	{
		if (strcmp(participants[who2RightIdx][i].name, w2->rightNeighbourName) == 0)
		{
			w2R = &participants[who2RightIdx][i];
			break;
		}
	}


	if (strcmp(w1->leftNeighbourName, who2) && strcmp(w1->rightNeighbourName, who2))
	{
		std::cerr << who1 << " and " << who2 << " are not neighbours!\n";
		return;
	}
	else if(strcmp(w1->leftNeighbourName, who2) == 0)
	{
		// if "who1" and "who2" are not held and if they are not holding 
		if (w1R->holdingLeft || w1->holdingRight || w2L->holdingRight || w2->holdingLeft)
		{
			std::cerr << who1 << " and " << who2 << " are held by neighbours!\n";
			return;
		}
		else
		{
			// rearranging the neighbours so that the swap is valid
			strcpy_s(w1R->leftNeighbourName, strlen(w2->name) + 1, w2->name);
			strcpy_s(w1->leftNeighbourName, strlen(w2L->name) + 1, w2L->name);
			strcpy_s(w1->rightNeighbourName, strlen(w2->name) + 1, w2->name);
			strcpy_s(w2->leftNeighbourName, strlen(w1->name) + 1, w1->name);
			strcpy_s(w2->rightNeighbourName, strlen(w1R->name) + 1, w1R->name);
			strcpy_s(w2L->rightNeighbourName, strlen(w1->name) + 1, w1->name);
		}
	}
	else if (strcmp(w1->rightNeighbourName, who2) == 0)
	{
		// if "who1" and "who2" are not held and if they are not holding 
		if (w1L->holdingRight || w1->holdingLeft || w2R->holdingLeft || w2->holdingRight)
		{
			std::cerr << who1 << " and " << who2 << " are held by neighbours!\n";
			return;
		}
		else
		{
			// rearranging the neighbours so that the swap is valid
			strcpy_s(w1->rightNeighbourName, strlen(w2R->name) + 1, w2R->name);
			strcpy_s(w1->leftNeighbourName, strlen(w2->name) + 1, w2->name);
			strcpy_s(w1L->rightNeighbourName, strlen(w2->name) + 1, w2->name);
			strcpy_s(w2R->leftNeighbourName, strlen(w1->name) + 1, w1->name);
			strcpy_s(w2->leftNeighbourName, strlen(w1L->name) + 1, w1L->name);
			strcpy_s(w2->rightNeighbourName, strlen(w1->name) + 1, w1->name);
		}
	}
}

void Hash::PrintHash() const
{
	// calculating the array's index in the table, where the first person is
	int frstIdx = HashFunction(firstParticipant, tableSize);
	// getting the array's size, where the first person is
	int frstArrSize = participants[frstIdx].GetSize();
	char tmp[32];
	
	int k;

	// finding the first participants
	for (k = 0; k < frstArrSize; ++k)
	{
		if (strcmp(participants[frstIdx][k].name, firstParticipant) == 0)
		{	
			break;
		}
	}
	// getting the first participant's left neighbour name
	strcpy_s(tmp, strlen(participants[frstIdx][k].leftNeighbourName) + 1,
		participants[frstIdx][k].leftNeighbourName);

	std::cout << firstParticipant << std::endl;
	
	// while we don't reach the first participant
	while (strcmp(tmp, firstParticipant))
	{
		std::cout << tmp << std::endl;

		// calculating the array's index in the table, where the left neighbour is
		int idx = HashFunction(tmp, tableSize);
		// getting the array's size
		int idxArrSize = participants[idx].GetSize();

		// finding the left neighbour and getting his left neighbour
		for (int i = 0; i < idxArrSize; ++i)
		{
			if (strcmp(participants[idx][i].name, tmp) == 0)
			{
				strcpy_s(tmp,
					strlen(participants[idx][i].leftNeighbourName) + 1,
					participants[idx][i].leftNeighbourName);
				
				break;
			}
		}
	}
}

double Hash::GetLoadFactor() const
{
	return double(count) / double(tableSize);
}

void Hash::ReHash(int newSize)
{
	// allocating new array with the new size
	DynamicArray <DynamicArray<Participant>> newOne(newSize);
	// calculating the array's index in the table, where the first participant is
	int frstIdx = HashFunction(firstParticipant, tableSize);
	// getting the array's size, where the first one is
	int frstArrSize = participants[frstIdx].GetSize();

	Participant copy;
	char tmp[32];

	// finding the first participant
	for (int i = 0; i < frstArrSize; ++i)
	{
		if (strcmp(participants[frstIdx][i].name, firstParticipant) == 0)
		{	
			copy = participants[frstIdx][i];
			break;
		}
	}

	int oldTableSizeIdx;
	// calculating the firts participant's array index in the new table
	int newTableSizeIdx = HashFunction(copy.name, newSize);
	// "pushing" the first participant in the hash table
	newOne[newTableSizeIdx].PushBack(copy);
	// getting the first participant's left neighbour
	strcpy_s(tmp, strlen(copy.leftNeighbourName) + 1, copy.leftNeighbourName);
	
	int arrSize;

	while (strcmp(tmp, firstParticipant))
	{
		// calculating the left neighbour's array index in the old table
		oldTableSizeIdx = HashFunction(tmp, tableSize);
		// getting the array's size, where the left neighbour is located
		arrSize = participants[oldTableSizeIdx].GetSize();
		// calculating the left neighbour's index in the new table
		newTableSizeIdx = HashFunction(tmp, newSize);

		// finding the left neighbour from the old table
		for (int i = 0; i < arrSize; ++i)
		{
			if (strcmp(participants[oldTableSizeIdx][i].name, tmp) == 0)
			{
				copy = participants[oldTableSizeIdx][i];
				break;
			}
		}
		// tmp is the new left neighbour of the old left neigbour
		strcpy_s(tmp, strlen(copy.leftNeighbourName) + 1, copy.leftNeighbourName);
		// pushing the old left neighour in the new table
		newOne[newTableSizeIdx].PushBack(copy);
	}

	// deleting the old data
	participants.FreeDynamicArray();
	// reallocating new table with the new size
	participants.ResizeDynamicArray(newSize);

	//operator = for the arrays
	for (int i = 0; i < newSize; ++i)
	{
		participants[i] = newOne[i];
	}

	// assigning the new table size
	tableSize = newSize;
}

void Hash::FreeHash()
{
	count = 0;
	strcpy_s(firstParticipant, strlen("") + 1, "");
}

void Hash::InsertNewParticipant(const char* who, const char* lN, const char* rN)
{
	Participant newPar; // making new object

	// calculating the index, where the array, in which the new participant
	// is going to be placed, is at
	int idx = HashFunction(who, tableSize);
	
	// setting the new object
	strcpy_s(newPar.name, strlen(who) + 1, who);
	strcpy_s(newPar.leftNeighbourName, strlen(lN) + 1, lN);
	strcpy_s(newPar.rightNeighbourName, strlen(rN) + 1, rN);
	newPar.holdingLeft = true;
	newPar.holdingRight = true;

	// pushing him/her in the array
	participants[idx].PushBack(newPar);
	
	// calculating his/her left and right neigbours and their array's sizes 
	int leftIdx = HashFunction(lN, tableSize);
	int leftSize = participants[leftIdx].GetSize();
	int rightIdx = HashFunction(rN, tableSize);
	int rightSize = participants[rightIdx].GetSize();

	// finding the left neighbour and setting his new right neighbour
	for (int i = 0; i < leftSize; ++i)
	{
		if (strcmp(participants[leftIdx][i].name, lN) == 0)
		{
			strcpy_s(participants[leftIdx][i].rightNeighbourName, strlen(who) + 1, who);
			participants[leftIdx][i].holdingRight = true;
			break;
		}
	}

	// finding the right neighbour and setting his new left neighbour
	for (int i = 0; i < rightSize; ++i)
	{
		if (strcmp(participants[rightIdx][i].name, rN) == 0)
		{
			strcpy_s(participants[rightIdx][i].leftNeighbourName, strlen(who) + 1, who);
			participants[rightIdx][i].holdingLeft = true;
			break;
		}
	}

	++count;
}

int Hash::HashFunction(const char* key, int size) const
{
	int keyLen = strlen(key);		// the length of the key

	// we will split the key of parts that contain 4 bytes
	int intLength = keyLen / 4;
	int sum = 0;

	// here we will store the parts
	char buff[5];
	int buffLen;
	int mult;

	for (int i = 0; i < intLength; i++)
	{
		buff[0] = '\0';
		int buffIdx = 0;
		for (int k = i * 4; k < i * 4 + 4; ++k) // copying 4 bytes
		{
			buff[buffIdx] = key[k];
			++buffIdx;
		}

		buff[buffIdx] = '\0';

		mult = 1;
		buffLen = strlen(buff);

		for (int j = 0; j < buffLen; ++j)	// sum of the 4 bytes' ascii values we copied,
		{									// multiplying them by 256^k, where k = 0, 1, 2, 3
			sum += buff[j] * mult;
			mult *= 256;
		}
	}

	buff[0] = '\0';
	int i = 0;
	while (key[intLength * 4 + i] != '\0') // copying the rest bytes of the key
	{
		buff[i] = key[intLength * 4 + i];
		++i;
	}
	buff[i] = '\0';
	buffLen = strlen(buff);

	mult = 1;
	for (int k = 0; k < buffLen; k++)	// adding them to the sum after multiplying them
	{									// by 256^i, i = 0, 1, 2
		sum += buff[k] * mult;
		mult *= 256;
	}

	return abs(sum % size);
}

void Hash::ReadNamesFromFile(const char* fileName)
{
	std::ifstream inFile(fileName, std::ios::in);
	if (!inFile)
	{
		std::cerr << "Could not open " << fileName << "!\n";
		exit(1);
	}

	// we will read the file using two buffers, so that we can arrange their connections
	char buffer[32] = "";
	char buffer1[32] = "";
	int rowCnt = 0;
	Participant p1;
	Participant p2;

	while (!inFile.eof())
	{
		inFile.getline(buffer, 32, '\n');

		if (strcmp(buffer, "") == 0) // This means the file has even number of rows and we have
		{							 // reached the end. All the participants are in the hash.
			break;
		}

		++rowCnt;
		
		if (strcmp(buffer1, "")) // if buffer1 not empty => buffer contains the previous row
		{
			// calculating the index where we have to pushBack this participant
			int h = HashFunction(buffer1, tableSize);

			// adding the left neighbour name
			strcpy_s(participants[h][ (participants[h].GetSize() - 1 ) ].leftNeighbourName,
					 strlen(buffer) + 1, buffer);
			
			participants[h][participants[h].GetSize() - 1].holdingLeft = true;
			
			// adding the right neighbour name
			strcpy_s(p1.rightNeighbourName, strlen(buffer1) + 1, buffer1);
			p1.holdingRight = true;
		}

		inFile.getline(buffer1, 32, '\n'); 

		if (strcmp(buffer1, "") == 0) // This means we have odd number of rows 
		{							  // in the file and we have reached the end,
			// setting the name		  // so we have to add the last one to the hash.
			strcpy_s(p1.name, strlen(buffer) + 1, buffer);
			// calculating the index
			int idx = HashFunction(buffer, tableSize);
			// pushing the participant
			participants[idx].PushBack(p1);
			++count;
			break;
		}

		++rowCnt;
		
		// saving the first name for the last operation and for the Print method
		if (rowCnt == 2)
		{
			strcpy_s(firstParticipant, strlen(buffer) + 1, buffer);
		}

		// arranging names of the people and their neighbours
		strcpy_s(p1.name, strlen(buffer) + 1, buffer);
		strcpy_s(p2.name, strlen(buffer1) + 1, buffer1);
		
		// setting left neighbour name and arranging the connection
		strcpy_s(p1.leftNeighbourName, strlen(buffer1) + 1, buffer1);
		p1.holdingLeft = true;
		
		// setting right neighbour name and arranging the connection
		strcpy_s(p2.rightNeighbourName, strlen(buffer) + 1, buffer);
		p2.holdingRight = true;
		
		// calculating where we have to pushBack the new participants 
		int idx1 = HashFunction(buffer, tableSize);
		int idx2 = HashFunction(buffer1, tableSize);
		
		// adding the new participants to the hash table
		participants[idx1].PushBack(p1);
		++count;
	
		participants[idx2].PushBack(p2);
		++count;
	}
	
	int lastId;

	// calculating the last participant's array index, where he/she is located
	// if we have odd rows in the file => the last participant is in buffer
	if (rowCnt % 2 == 1) 
	{
		lastId = HashFunction(buffer, tableSize);
	}
	else // else - we have even number of rows => the last participant is in buffer1
	{
		lastId = HashFunction(buffer1, tableSize);
	}

	// the last participant's left neighbour is the first participant
	strcpy_s(participants[lastId][(participants[lastId].GetSize() - 1)].leftNeighbourName,
		strlen(firstParticipant) + 1, firstParticipant);

	participants[lastId][(participants[lastId].GetSize() - 1)].holdingLeft = true;

	// calculating the first participant's array index, where he/she is located
	int firstId = HashFunction(firstParticipant, tableSize);

	// setting the first participant's right neighbour
	// the first participant's right neighbour is the last participant
	if (rowCnt % 2 == 1) // if we have odd number of rows => the last row is in buffer
	{
		strcpy_s(participants[firstId][0].rightNeighbourName, strlen(buffer) + 1, buffer);
	}
	else // else - we have have even number of rows => the last row is in buffer1
	{
		strcpy_s(participants[firstId][0].rightNeighbourName, strlen(buffer1) + 1, buffer1);
	}
	participants[firstId][0].holdingRight = true;

	inFile.close();
}