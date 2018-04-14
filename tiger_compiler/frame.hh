#include <deque>
#include <vector>
#include <utility>
#include <string>
#pragma once
using namemap = std::vector<std::pair<std::string,int> >; //maps names to sp (was originally fp offsets)


class Frame {
public:
	Frame();
	~Frame()=default;
	int fp; //Frame pointer
	int sp; //stack pointer
	std::deque<namemap> tempmap; //stores previous Frame's temps
	std::deque<std::vector<std::string> > labelmap;
	std::deque<namemap> localsmap; //stores previous Frame's locals
	std::deque<namemap> argsmap; //stores previous Frame's arguments
	std::deque<int> stack; //the representation of the actual stack
	namemap current [3]; //current[0] = current tempmap, current[1] = current localsmap, current[2] = current argsmap
	std::vector<std::string> currentlabel;
	std::deque<int> temp1addr; //makes sure to keep track of the first temp address for each stack.

	int pushFrame(std::vector<std::string> arguments_passed, std::vector<std::string> local_variables);//pushes new Frame
	int popFrame(); //pops said newest Frame
	int addtemp(std::string name); //adds temporary to current Frame
	int poptemp(); //pops last temporary from Frame
	int addlabel(std::string);
	int lookuptemp(std::string name); //returns the temporary's sp offset
	int assignvar(int i, int offset); //assigns any value on the stack according to the sp offset.
	int lookupvar(std::string name); //returns the sp offset of the local or argument


private:
	//stack int temp1addr -> temp1addr first temp address
	//int fp -> Frame pointer of current stack
	//int sp -> easy to keep track of this this way
	//stack map tempmap -> stack of temp class(: maps temp names to offsets from temp1addr )
	//stack map labelmap -> stack of labels
	//stack map localsmap -> stack mapping (same template as tempmap) for locals
	//stack map argsmap -> u get the drift, may not be needed?
	//stack int stack -> the actual stack :0 (of ints??)
	//vector map current -> holds the maps that define the scope we're currently in. [temp,label,local,args]


	//init() -> returns Frame with self.stack empty, self.temp1addr empty, self.fp=0, self.*map empty
	//pushFrame(vector arguments_passed, vector local_variables)
	//	-> lastsp=self.sp
	//	-> push new *map, push arguments in order, adding them to argsmap
	//	-> push current fp as a value
	//	-> add locals in order to stack and localsmap
	//	-> push return address <= lastsp?
	//	-> push self.temp1addr <= self.sp
	//popFrame()
	//	-> takes fp and pops everything before on the stack,
	//	-> pop *map, pop temp1addr,
	//	-> fp = return address (temp1addr-1),
	//	-> sp = fp
	//lookupvar() looks through symbol table for previous variable scope
	};













