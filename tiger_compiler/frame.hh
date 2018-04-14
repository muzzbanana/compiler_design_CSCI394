#include <deque>
#include <vector>
#include <utility>
#include <string>
#pragma once
using map = std::vector<std::pair<std::string,int> >; //maps names to sp (was originally fp offsets)


class frame {
public:
	frame();
	~frame()=default;
	int fp; //frame pointer
	int sp; //stack pointer
	std::deque<map> tempmap; //stores previous frame's temps
	std::deque<std::vector<std::string> > labelmap; 
	std::deque<map> localsmap; //stores previous frame's locals
	std::deque<map> argsmap; //stores previous frame's arguments
	std::deque<int> stack; //the representation of the actual stack 
	map current [3]; //current[0] = current tempmap, current[1] = current localsmap, current[2] = current argsmap
	std::vector<std::string> currentlabel; 
	std::deque<int> temp1addr; //makes sure to keep track of the first temp address for each stack.
	
	int pushframe(map arguments_passed, map local_variables);//pushes new frame
	int popframe(); //pops said newest frame
	int addtemp(std::string name,int value); //adds temporary to current frame
	int poptemp(); //pops last temporary from frame
	int addlabel(std::string); 
	int lookuptemp(std::string name); //returns the temporary's current value
	int lookupvar(std::string name); //returns the value of the local or argument


private:
	//stack int temp1addr -> temp1addr first temp address 
	//int fp -> frame pointer of current stack
	//int sp -> easy to keep track of this this way
	//stack map tempmap -> stack of temp class(: maps temp names to offsets from temp1addr )
	//stack map labelmap -> stack of labels
	//stack map localsmap -> stack mapping (same template as tempmap) for locals
	//stack map argsmap -> u get the drift, may not be needed?
	//stack int stack -> the actual stack :0 (of ints??)
	//vector map current -> holds the maps that define the scope we're currently in. [temp,label,local,args]


	//init() -> returns frame with self.stack empty, self.temp1addr empty, self.fp=0, self.*map empty
	//pushframe(vector arguments_passed, vector local_variables)
	//	-> lastsp=self.sp
	//	-> push new *map, push arguments in order, adding them to argsmap
	//	-> push current fp as a value 
	//	-> add locals in order to stack and localsmap
	//	-> push return address <= lastsp?
	//	-> push self.temp1addr <= self.sp
	//popframe() 
	//	-> takes fp and pops everything before on the stack, 
	//	-> pop *map, pop temp1addr, 
	//	-> fp = return address (temp1addr-1), 
	//	-> sp = fp
	//lookupvar() looks through symbol table for previous variable scope
	};













