#include "frame.hh"
#include <deque>
#include <vector>
#include <utility>
#include <string>
#pragma <once>

frame::frame()
	:fp(0),sp(0),tempmap(),labelmap(),argsmap(),localsmap(),stack(),current(),temp1addr()
	{
	}

void frame::pushframe(std::vector<std::pair<std::string,int>> arguments_passed, std::vector<std::pair<std::string,int>> local_variables){
	//pushframe(vector arguments_passed, vector local_variables)

	//assumed vector of pairs in order (name,value)
	//	-> lastsp=self.sp
	int lastsp = sp;
	//	-> push new *map, push arguments in order, adding them to argsmap
	argsmap.push_back(std::vector<std::pair<std::string,int>>);
	localsmap.push_back(std::vector<std::pair<std::string,int>>);
	tempmap.push_back(std::vector<std::pair<std::string,int>>);
	int i = 0-arguments_passed.size();
	for (std::pair<std::string,int> arg : argsmap) {
		auto argum = std::make_pair(std::get<0>(arg),i);//name and i
		argsmap.back.insert(argum);
		stack.push_back(std::get<1>(arg)); //value
		sp += 1;
		i+=1;
	}
	//	-> push current fp as a value 
	stack.push_back(fp);
	sp += 1;
	fp = sp;
	//	-> add locals in order to stack and localsmap
	i = 0;
	for (std::pair<std::string,int> local : local_variables) {
		auto loc = std::make_pair(std::get<0>(local),i);//name and i
		localsmap.back.insert(loc);
		stack.push_back(std::get<1>(local)); //value
		sp += 1;
		i+=1;
	}
	//	-> push return address <= lastsp?
	stack.push_back(lastsp);
	sp += 1;
	//	-> push self.temp1addr <= self.sp
	temp1addr.push_back(sp);
}

void frame::popframe(){
	//popframe() 
	//	-> takes fp and pops everything before on the stack, 
	//	-> pop *map, pop temp1addr, 
	//	-> fp = return address (temp1addr-1), 
	//	-> sp = fp


}

void frame::addtemp(std::string name,int value){
	auto templist = current[0];
	std::pair<std::string,int> newpair (name,value);
	templist.push_back(newpair);
}

void frame::addlabel(std::string name,int value){
	auto labellist = current[1];
	labellist.push_back(name);
}



int frame::lookuptemp(std::string name){
	auto templist = current[0];
	for (auto iter = templist.begin(); iter != templist.end(); ++i){
		if (*iter.first() == name){
			return *iter.second();
		};
	};
	return (-1) // no such name in temp
}

int frame::lookupvar(std::string name){
	return 0;
}


int main() {
	auto f = frame();
	std::string str = "var1";
	auto a = make_pair(str,2);
	std::string stg = "var2";
	auto b = make_pair(stg,18);
	std::vector<std::pair<std::string,int>> vect;
	vect.insert(a);
	vect.insert(b);
	f.pushframe(vect,vect);
	return 0;
}