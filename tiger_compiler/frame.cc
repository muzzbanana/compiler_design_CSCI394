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
	current[0] = namemap(); //reinitialize current
	current[1] = namemap();
	current[2] = namemap();
	int i = 0-arguments_passed.size();
	// push arguments in order, adding them to argsmap
	for (int j = 0;  j< int(arguments_passed.size()); j++) {
		std::string arg = arguments_passed[j];
		std::pair<std::string,int> argum = std::make_pair(arg,i); //name and negative fp offset
		current[2].push_back(argum);
		stack.push_back(0); //value pushed onto the stack
		sp += 1;
		i+=1;
	};
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
	};
	//	-> push return address <= lastsp?
	stack.push_back(lastsp);
	sp += 1;
	//	-> push self.temp1addr <= self.sp
	temp1addr.push_back(fp);
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

int Frame::addtemp(std::string name){ //adds new temporary to the stack and maps the name to the stack address
	auto templist = current[0];
	std::pair<std::string,int> newpair (name,sp-fp);
	templist.push_back(newpair);
	stack.push_back(0);
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
		sp -= 1;
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
			return iter->second;
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
	for (auto iter = localslist.rbegin(); iter != localslist.rend(); ++iter){
		if (iter->first == name){
			return iter->second;
		};
	};
	for (auto iter = argslist.rbegin(); iter != argslist.rend(); ++iter){
		if (iter->first == name){
			return iter->second;
		};
	}; //starts iterating through previous Frames to find the last time that variable was used.
	int num = 1;
	for (auto i = localsmap.rbegin(), j = argsmap.rbegin(); i != localsmap.rend(); i++, j++) {
		localslist = *i;
		argslist = *j;
		for (auto iter = localslist.begin(); iter != localslist.end(); ++iter){
			if (iter->first == name){
				return iter->second - (temp1addr.back() - temp1addr[temp1addr.size()-num]); //return fp offset
			};
		};
		for (auto iter = argslist.begin(); iter != argslist.end(); ++iter){
			if (iter->first == name){
				return iter->second - (temp1addr.back() - temp1addr[temp1addr.size()-num]); //return fp offset
			};
		};
		num++;
	} ;//if no return here, the variable isn't in any map
	return 0;
}

/* int main() {
	std::cout<<"start!\n"<<std::endl;
	auto f = Frame();
	std::cout<<"initialized Frame!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111\n\n"<<std::endl;
	std::string str = "var1";
	std::string stg = "var2";
	auto vect = std::vector<std::string>();
	vect.push_back(str);
	vect.push_back(stg);
	std::cout<<"map made!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111\n\n"<<std::endl;
	f.pushFrame(vect,std::vector<std::string>());
	std::cout << f.lookupvar(stg) << f.lookupvar(str) << std::endl;
	f.popFrame();
	std::cout << "did not technically abort!!!!!!1\n\n"<<std::endl;
	return 0;
} */
