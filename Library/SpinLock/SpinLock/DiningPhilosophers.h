#pragma once

class DiningPhilosophers
{
public:
	const int		nNum = 5;
	Philosopher		*mPhilosophers;
	Fork			*mForks;
public:
	DiningPhilosophers();
	~DiningPhilosophers();

	void start(void);

	void join(void);
};
