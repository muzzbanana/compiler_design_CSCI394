#include <iostream>
#include <iomanip>  // for setprecision

using namespace std;

float euros_to_dollars(float amount){
	if (amount < 0){
		cout << "I am going to assume you did not want to add the negative. \n";
		amount = amount * -1;
	}
	return amount * 1.24;	// this is the conversion rate acccording to google
}

int round_down(float num){
	return static_cast<int>(num);
}

void count_up_down(int num){	// assumes argument could be non-positive integer
	if (num <= 0){
		if (num == 1) {
			cout << num << "\n";
			return;
		}
		cout << num << "\n";
		count_up_down(num+1);
		cout << num << "\n";
		}
	else {
		if (num == 1) {
			cout << num << "\n";
			return;
		}
		cout << num << "\n";
		count_up_down(num-1);
		cout << num << "\n";
	}
}


int sum_till_n(int num){	// assumes argument could be non-positive interger
	static int total = 0;
	if (num == 0){
		int result = total;
		total = 0;		// resets total to be 0
		return result;
	}
	total = total + num;
	cout << total << "\n";
	if (num < 0){
		return sum_till_n(num+1);
	}
	return sum_till_n(num-1);
}

int main(){
	float dollars = euros_to_dollars(10.00);
	float dollars2 = euros_to_dollars(-5.00);
	cout << fixed << setprecision(2) << "$" << dollars << "\n";
	cout << fixed << setprecision(2) << "$" << dollars2 << "\n";


	int val_1 = round_down(10.7);
	int val_2 = round_down(-1.6);
	cout << "10.7 is now " << val_1 << "\n";
	cout << "-1.6 is now " << val_2 << "\n";

	int num = 4;
	count_up_down(num);
	cout << "\n";
	int num1 = 0;
	count_up_down(num1);
	cout << "\n";
	int num2 = -8;
	count_up_down(num2);
	cout << "\n";
	int num3 = 1;
	count_up_down(num3);
	cout << "\n";


	int summ = sum_till_n(4);
	cout << "The sum for 0 to 4 is " << summ << "\n";
	int summ1 = sum_till_n(-3);
	cout << "The sum for -3 to 0 is " << summ1 << "\n";
	int summ2 = sum_till_n(8);
	cout << "The sum for 0 to 8 is " << summ2 << "\n";

}