#include <deque>
#include <vector>
#include <utility>
#include <string>
using map = std::vector<std::pair<std::string,int> >;


class frame {
public:
	frame();
	~frame()=default;
	std::deque<int> temp1addr;
	int fp;
	int sp;
	std::deque<map> tempmap;
	std::deque<std::vector<std::string> > labelmap;
	std::deque<map> localsmap;
	std::deque<map> argsmap;
	std::deque<int> stack;
	map current [4];

	int pushframe(map arguments_passed, map local_variables);
	int popframe();
	int addtemp(std::string name,int value);
	int addlabel(std::string);
	int lookuptemp(std::string name);
	int lookupvar(std::string name);


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













