#ifndef FAULTS_H
#define FAULTS_H

	//-------------------------------------------------------------------
	// Class: Counter
	//
	// Stores counts of varrious things, flags and temp numbers
	//-------------------------------------------------------------------
	class Counter {
	private:
		int count;
		int temp;
		int flag;

	public:
		Counter();
		int Count();
		void Set(int num);
		void Reset();
		
		void Add();
		void Sub();
		void Sub(int num);  //overloaded sub
		
		int Temp();
		void SetTemp(int num);
		
		int Flag();
		// 0 = off / 1 = false / 2 = true
		void SetFlag(int token);

	}; //end Counter


//---------------------------------------------

// Default Initializer 
Counter::Counter()
{
	count = 0;
	temp = 0;
	flag = 0;
}

// Get Count
int Counter::Count()
{
	return count;
}

// Increase Counter by a number
void Counter::Set(int num)
{
	count = num;
}

// Reset everything to zero
void Counter::Reset()
{
	count = 0;
	temp = 0;
	flag = 0;
}

// Add one to count
void Counter::Add()
{
	count++;
}

// Sub one to count
void Counter::Sub()
{
	count--;
}

// Reduce Counter by a number
void Counter::Sub(int num)
{
    count -= num;
}

// Get temp number
int Counter::Temp()
{
	return temp;
}

// Set temp number
void Counter::SetTemp(int num)
{
	temp = num;	
}

// Get Flag
int Counter::Flag()
{
	return flag;
}

// Set Flag
void Counter::SetFlag(int token)
{
	flag = token;
}

#endif