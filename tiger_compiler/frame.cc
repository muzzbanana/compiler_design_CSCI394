#include "frame.hh"
#include <deque>
#include <vector>
#include <utility>
#include <string>
#include <iostream>


Frame::Frame()
	:fp(0),sp(0),tempmap(),labelmap(),localsmap(),argsmap(),stack(),current(),currentlabel(),temp1addr()
	{
	}

int Frame::pushFrame(std::vector<std::string> arguments_passed, std::vector<std::string> local_variables){
	//pushFrame(vector arguments_passed, vector local_variables)

	//assumed vector of pairs in order (name,value)
	//	-> lastsp=self.sp
	int lastsp = sp;
	//	-> push new *maps,
	argsmap.push_back(current[2]);
	localsmap.push_back(current[1]);
	tempmap.push_back(current[0]);
	labelmap.push_back(currentlabel);
	namemap current [4]; //reinitialize current
	int i = 0-arguments_passed.size();
	// push arguments in order, adding them to argsmap
	for (int j = 0;  j< int(arguments_passed.size()); j++) {
		std::string arg = arguments_passed[j];
		std::pair<std::string,int> argum = std::make_pair(arg,i); //name and negative fp offset
		current[2].push_back(argum);
		stack.push_back(0); //value pushed onto the stack
		sp += 1;
		i+=1;
	}
	//	-> push current fp as a value 
	stack.push_back(fp);
	sp += 1;
	fp = sp;
	//	-> add locals in order to stack and localsmap
	for (int h = 0; h < int(local_variables.size()); h++) {
		std::string local = local_variables[h]; 
		auto loc = std::make_pair(local,h);//name and offset from fp
		current[1].push_back(loc);
		stack.push_back(0); //value pushed onto the stack
		sp += 1;
	}
	//	-> push return address <= lastsp?
	stack.push_back(lastsp);
	sp += 1;
	//	-> push self.temp1addr <= self.sp
	temp1addr.push_back(sp);
	return 0;
}

int Frame::popFrame(){
	//popFrame() 
	//	-> takes fp and pops everything before on the stack, 
	while (int(stack.size())>fp) {stack.pop_back();}
	//	-> pop *map, pop temp1addr, 
	current[0] = tempmap.back();
	tempmap.pop_back();
	currentlabel = labelmap.back();
	labelmap.pop_back();
	current[1] = localsmap.back();
	localsmap.pop_back();
	current[2] = argsmap.back();
	argsmap.pop_back();
	temp1addr.pop_back();
	//	-> fp = return address (temp1addr-1), 
	fp = stack[fp-1];
	//	-> sp = fp
	sp = fp;
	return 0;

}

int Frame::addtemp(std::string name,int value){ //adds new temporary to the stack and maps the name to the stack address
	auto templist = current[0];
	std::pair<std::string,int> newpair (name,templist.size());
	templist.push_back(newpair);
	stack.push_back(value);
	sp +=1;
	return 0;
}

int Frame::poptemp() { //pops last temp and returns its value
	auto templist = current[0];
	if (templist.empty()==1) {
		return (-1);
	} else {
		templist.pop_back();
		int ret = stack.back();
		stack.pop_back();
		return ret;
	}
}

int Frame::addlabel(std::string name){ //probably useless, but here if we need it again
	auto labellist = currentlabel;
	labellist.push_back(name);
	return 0;
} //hello appendix, we love you too



int Frame::lookuptemp(std::string name){ //takes temp name and returns the value
	auto templist = current[0];
	for (auto iter = templist.begin(); iter != templist.end(); ++iter){
		if (iter->first == name){
			return iter->second+temp1addr.back();
		};
	};
	return (-1); 
}
int Frame::assignvar(int i, int offset) {
	stack[offset] = i;
	return 0;
}

int Frame::lookupvar(std::string name){ //takes a local or argument name and returns the fp offset
	auto localslist = current[1];
	auto argslist = current[2];
	for (auto iter = localslist.begin(); iter != localslist.end(); ++iter){
		if (iter->first == name){
			return iter->second+temp1addr.back();
		};
	};
	for (auto iter = argslist.begin(); iter != argslist.end(); ++iter){
		if (iter->first == name){
			return iter->second+temp1addr.back();
		};
	}; //starts iterating through previous Frames to find the last time that variable was used.
	auto cleanup = std::deque<namemap>(); //to store popped maps until they can be pushed back on in order.
	auto cleanuptemp1addr = std::deque<int>();
	while (argsmap.empty() == 0) {
		cleanup.push_back(localslist); //clears the previous Frames lists
		cleanup.push_back(argslist);
		cleanuptemp1addr.push_back(temp1addr.back());
		temp1addr.pop_back();
		localslist = localsmap.back(); //pops on the next set of Frame maps
		localsmap.pop_back();
		argslist = argsmap.back();
		argsmap.pop_back(); 
		for (auto iter = localslist.begin(); iter != localslist.end(); ++iter){
			if (iter->first == name){
				//flush cleanup
				localsmap.push_back(localslist);
				argsmap.push_back(argslist);
				while (cleanup.empty() == 0) {
					argsmap.push_back(cleanup.back());
					cleanup.pop_back();
					localsmap.push_back(cleanup.back());
					cleanup.pop_back();
				}
				while(cleanuptemp1addr.empty()==0) { //clean up temp1address
					temp1addr.push_back(cleanuptemp1addr.back());
					cleanuptemp1addr.pop_back();
				}
				return iter->second; //return value
			};
		};
		for (auto iter = argslist.begin(); iter != argslist.end(); ++iter){
			if (iter->first == name){
				//flush cleanup
				localsmap.push_back(localslist);
				argsmap.push_back(argslist);
				while (cleanup.empty() == 0) {
					argsmap.push_back(cleanup.back());
					cleanup.pop_back();
					localsmap.push_back(cleanup.back());
					cleanup.pop_back();
				}
				while(cleanuptemp1addr.empty()==0) { //clean up temp1address
					temp1addr.push_back(cleanuptemp1addr.back());
					cleanuptemp1addr.pop_back();
				}
				return iter->second; //return value
			};
		};

	} //if no return here, the variable isn't in any map
	localsmap.push_back(localslist);
	argsmap.push_back(argslist);
	while (cleanup.empty() == 0) {
		argsmap.push_back(cleanup.back());
		cleanup.pop_back();
		localsmap.push_back(cleanup.back());
		cleanup.pop_back();
	}
	while(cleanuptemp1addr.empty()==0) { //clean up temp1address
		temp1addr.push_back(cleanuptemp1addr.back());
		cleanuptemp1addr.pop_back();
	}
	return 0; 
}

//int main() {
//	std::cout<<"start!\n";
//	auto f = Frame();
//	std::cout<<"initialized Frame!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111\n\n";
//	std::string str = "var1";
//	auto a = make_pair(str,2);
//	std::string stg = "var2";
//	auto b = make_pair(stg,18);
//	map vect;
//	vect.push_back(a);
//	vect.push_back(b);
//	std::cout<<"map made!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111\n\n";
//	f.pushFrame(vect,vect);
//	f.popFrame();
//	std::cout << "did not technically abort!!!!!!1\n\n";
//	return 0;
//}
