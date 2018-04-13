#include "frame.hh"
#include <deque>
#include <vector>
#include <utility>
#include <string>
#include <iostream>


frame::frame()
	:fp(0),sp(0),tempmap(),labelmap(),localsmap(),argsmap(),stack(),current(),currentlabel(),temp1addr()
	{
	}

int frame::pushframe(map arguments_passed, map local_variables){
	//pushframe(vector arguments_passed, vector local_variables)

	//assumed vector of pairs in order (name,value)
	//	-> lastsp=self.sp
	int lastsp = sp;
	//	-> push new *map, push arguments in order, adding them to argsmap
	argsmap.push_back(current[2]);
	localsmap.push_back(current[1]);
	tempmap.push_back(current[0]);
	labelmap.push_back(currentlabel);
	map current [4]; //reinitialize current
	int i = 0-arguments_passed.size();
	for (int j = 0;  j< arguments_passed.size(); j++) {
		std::pair<std::string,int> arg = arguments_passed[j];
		std::pair<std::string,int> argum = std::make_pair(std::get<0>(arg),i);//name and i
		current[2].push_back(argum);
		stack.push_back(std::get<1>(arg)); //value
		sp += 1;
		i+=1;
	}
	//	-> push current fp as a value 
	stack.push_back(fp);
	sp += 1;
	fp = sp;
	//	-> add locals in order to stack and localsmap
	for (int h = 0; h < local_variables.size(); h++) {
		std::pair<std::string,int> local = local_variables[h]; 
		auto loc = std::make_pair(std::get<0>(local),h);//name and i
		current[1].push_back(loc);
		stack.push_back(std::get<1>(local)); //value
		sp += 1;
	}
	//	-> push return address <= lastsp?
	stack.push_back(lastsp);
	sp += 1;
	//	-> push self.temp1addr <= self.sp
	temp1addr.push_back(sp);
	return 0;
}

int frame::popframe(){
	//popframe() 
	//	-> takes fp and pops everything before on the stack, 
	while (stack.size()>fp) {stack.pop_back();}
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

int frame::addtemp(std::string name,int value){
	auto templist = current[0];
	std::pair<std::string,int> newpair (name,value);
	templist.push_back(newpair);
	return 0;
}

int frame::addlabel(std::string name){
	auto labellist = currentlabel;
	labellist.push_back(name);
	return 0;
}



int frame::lookuptemp(std::string name){
	auto templist = current[0];
	for (auto iter = templist.begin(); iter != templist.end(); ++iter){
		if (iter->first == name){
			return iter->second;
		};
	};
	return (-1); // no such name in temp ---> should also look up previous temps 
}

int frame::lookupvar(std::string name){
	auto localslist = current[1];
	auto argslist = current[2];
	for (auto iter = localslist.begin(); iter != localslist.end(); ++iter){
		if (iter->first == name){
			return iter->second;
		};
	};
	for (auto iter = argslist.begin(); iter != argslist.end(); ++iter){
		if (iter->first == name){
			return iter->second;
		};
	};
	return (-1); //should iterate through the stacks of previous scopes too
}


int main() {
	auto f = frame();
	std::cout<<"initialized frame!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111\n\n";
	std::string str = "var1";
	auto a = make_pair(str,2);
	std::string stg = "var2";
	auto b = make_pair(stg,18);
	map vect;
	vect.push_back(a);
	vect.push_back(b);
	std::cout<<"map made!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111\n\n";
	f.pushframe(vect,vect);
	std::cout << f.argsmap << "<f.argsmap \n" <<f.localsmap << "<f.localsmap\n";
	f.popframe();
	std::cout << "did not technically abort!!!!!!1\n\n";
	return 0;
}