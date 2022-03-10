#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex pumpOneLock, pumpTwoLock;
int pumpOne = 0, pumpTwo = 0;

struct Car{
	int numFills = 0;
	int id;
} typedef car;

car carArr[10];

void pumpCar(car* c){
	//std::cout << "car " << c.id << " filled " << c.numFills << std::endl;
	bool pumped = false;
	if(pumpOneLock.try_lock()){
		//std::cout << "Car " << c.id << " pumping at pump 1" << std::endl;
		pumpOne++;
		c->numFills++;
		pumped = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		pumpOneLock.unlock();
	}
	if(pumpTwoLock.try_lock() && !pumped){
		//std::cout << "Car " << c.id << " pumping at pump 2" << std::endl;
		pumpTwo++;
		c->numFills++;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		pumpTwoLock.unlock();
	}
}

int main(){
	std::thread carThreads[10];
	for(int i = 0; i < 10; i++){
		carArr[i].id = i;
		carArr[i].numFills = 0;
	}
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while(std::chrono::system_clock::now() < start+std::chrono::seconds(30)){
		for(int i = 0; i < 10; i++){
			carThreads[i] = std::thread(pumpCar, &carArr[i]);
			carThreads[i].join();
		}
	}
	/*for(int i = 0; i < 10; i++){
		carThreads[i].join();
	}*/
	std::cout << "Pump 1 was used " << pumpOne << "times\nPump 2 was used " << pumpTwo << " times\n";
	for(int i = 0; i < 10; i ++){
		std::cout << "Car " << i+1 << " was filled " << carArr[i].numFills << " times" << std::endl;
	}
}
