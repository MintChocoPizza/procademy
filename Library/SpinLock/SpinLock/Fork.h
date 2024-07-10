#pragma once

class Fork
{
public:
	int mID;
	int mOuner;
	int mMutex;

public:
	Fork();
	Fork(int id);
	~Fork();

	void printGet(int who);
	void get(int who);
	bool tryGet(int who);
	void put(void);
};
