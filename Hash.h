#ifndef HASH_HEADER_INCLUDED
#define HASH_HEADER_INCLUDED
#include <fstream>
#include "DynamicArray.hpp"

class Hash
{
	struct Participant
	{
		char name[32];
		char leftNeighbourName[32];
		char rightNeighbourName[32];
		bool holdingLeft;
		bool holdingRight;

		Participant()
			: name(""), leftNeighbourName(""),
			  rightNeighbourName(""), holdingLeft(false),
			  holdingRight(false) {}
	};

	Hash(const Hash& other) = delete;
	Hash& operator=(const Hash& rhs) = delete;

public:
	Hash();
	~Hash();

	void ReleaseParticipant(const char* who, const char* op);
	void GrabParticipant(const char* who, const char* op);
	void InfoAboutParticipant(const char* who);
	void AddParticipant(const char* who, const char* lN, const char* rN);
	void RemoveParticipant(const char* who);
	void SwapParticipants(const char* who1, const char* who2);
	void PrintHash() const;

	double GetLoadFactor() const;

private:
	void ReHash(int newSize);
	void FreeHash();
	void InsertNewParticipant(const char* who, const char* lN, const char* rN);
	int HashFunction(const char* c, int size) const;
	void ReadNamesFromFile(const char* fileName);;

private:
	DynamicArray<DynamicArray<Participant>> participants;
	int count;
	int tableSize;
	char firstParticipant[32];
};

#endif