#include<bits/stdc++.h>
#define dd double
#define random (rand()/(double)RAND_MAX)
#define Add push_back
using namespace std;
class System{
	vector<dd>CostPerAdvt;
	vector<dd>AudiencePerAdvt;
	vector<dd>NoOfAdvt;
	vector<dd>L;
	vector<dd>U;
	int DecesionVariables;
	dd Budget;
	dd TotalAudience;
	dd TotalCost;
public:
	System(){
		DecesionVariables=3;
		CostPerAdvt.Add(2000);
		CostPerAdvt.Add(600);
		CostPerAdvt.Add(400);
		AudiencePerAdvt.Add(100000);
		AudiencePerAdvt.Add(40000);
		AudiencePerAdvt.Add(18000);
		L.Add(0);
    	L.Add(0);
    	L.Add(0);
    	U.Add(20);
    	U.Add(20);
    	U.Add(20);
		NoOfAdvt.resize(3);
		Budget=18200;
	}
	bool isValidGene(vector<dd>Array);	//check if passed array of advts is valid or not according to problem statement
	bool isValidGene(void);				//check if currently set NoOfAdvt is valid or not according to problem staement
	void setNoOfAds(vector<dd>Array);	//set the number of advt array to same values as the received one
	int getDecesionVariables(void);  	//return decesion variables count (required to determine the chromosome size)
	dd getLowerBound(int i); 			//given index of the gene return its corresponding lower bound (required to set the random value to gene)
	dd getUpperBound(int i);			// 
	dd getFitness(void);				//return fitness value, targetted audience in this case
	dd getTotalCost(void);				//return total cost as per the NoOfAdvt array vlaues
	dd getTotalAudience(void);			//return total number of targetted audience as per the NoOfAdvt array values
};
class Individual{
	vector<dd>Gene;
	System Properties;
	int DecesionVariables;
	dd TotalCost;
	dd TotalAudience;
public:
	dd Fitness;
	Individual(){  //default constructor to initialze every member function with a valid value;
		Properties=System();
		DecesionVariables=Properties.getDecesionVariables();
		Gene.resize(DecesionVariables);
		do{
			for(int i=0;i<DecesionVariables;i++){
				Gene[i]=(int)(Properties.getLowerBound(i)+(Properties.getUpperBound(i)-Properties.getLowerBound(i))*random);
			}
		}while(!Properties.isValidGene(Gene));
		Properties.setNoOfAds(Gene);
		Fitness=Properties.getFitness();
		TotalCost=Properties.getTotalCost();
		//cout<<"\t\t\t\fadsfadsfffffffff"<<TotalCost;
		TotalAudience=Properties.getTotalAudience();
	}
	dd getFitness(void);				// return the firness function according to the current gene value
	dd getTotalCost(void);				// return total cost
	dd getTotalAudience(void);			// return total audience 
	int getDecesionVariables(void);		// 
	vector<dd> getGenes(void);			// return the current gene vlaue held by Individual
	bool isValidGene(vector<dd>Array);	// 
	void displayIndividual(void);		//	display the cost,target audience and the gene contents of the current individual
	void setGenes(vector<dd>Array);		//
};
bool compare(const Individual & l, const Individual & r)
{
    return l.Fitness < r.Fitness;
}
class SimulateGA{
	vector<Individual>Population;
	int PopulationSize;
	dd MutationRate;
	dd CrossoverRate;
	int CrossoverPoint;					//As for now it doesn't serves any function, but can be used to scale a function for multi point crossover
public:
	SimulateGA(){
		PopulationSize=100;
		Population.resize(PopulationSize);
		MutationRate=0.06;
		CrossoverRate=0.8;
		CrossoverPoint=1;
	}
	void doCrossover();
	vector<dd> doMutation(vector<dd>Array);
	void displayPopulation(void);
	void displayFittest(void);
	void sortFittest();
};
//======================================================================================//
//====================Member function definitions for SimulateGA class==================//
//======================================================================================//
void SimulateGA::doCrossover(void){
	vector<Individual>NewPopulation; //new population which will replace the old one after the entire crossover process
	vector<pair<int,int> >MatingPair; //index of two persons choosen for crossover
	int DecesionVariables=Population[0].getDecesionVariables();
	/*Create the mating pair*/
	for(int i=0;i<PopulationSize/2;i++){
		int p1=random*PopulationSize;
		int p2=random*PopulationSize;
		MatingPair.Add(make_pair(p1,p2));
	}
	for(int index=0;index<MatingPair.size();index++){
		Individual Parent1=Population[MatingPair[index].first];
		Individual Parent2=Population[MatingPair[index].second];
		vector<dd>Gene1=Parent1.getGenes();
		vector<dd>Gene2=Parent2.getGenes();
		int Point=random*DecesionVariables;
		for(int i=Point;i<DecesionVariables;i++){
			dd temp=Gene1[i];
			Gene1[i]=Gene2[i];
			Gene2[i]=temp;
		}
		Individual Child1;
		Individual Child2;
		Gene1=doMutation(Gene1);	//mutate the current produced chlids
		Gene2=doMutation(Gene2);
		Child1.setGenes(Gene1);
		Child2.setGenes(Gene2);
		vector<Individual>Family;
		Family.Add(Parent1);
		Family.Add(Parent2);
		if(Child1.isValidGene(Gene1)){
			Family.Add(Child1);
		}
		if(Child2.isValidGene(Gene2)){
			Family.Add(Child2);
		}
		sort(Family.begin(),Family.end(),compare);
		reverse(Family.begin(),Family.end());	//sort the family into order of their fitness so that fittest ones whethr its parent or child will go to nextgeneration
		NewPopulation.Add(Family[0]);
		NewPopulation.Add(Family[1]);
	}
	Population=NewPopulation;
}
vector<dd> SimulateGA::doMutation(vector<dd>Array){
	for(int i=0;i<Array.size();i++){
		dd r=random;
		if(r<MutationRate){
			//cout<<"Old "<<Array[i];
			Array[i]+=(Array[i]*MutationRate);
			//cout<<"New  "<<Array[i];
		}
	}
	return Array;
}
void SimulateGA::displayPopulation(void){
	cout<<"\n=================Current Population================\n";
	cout<<setw(10)<<"Cost "<<setw(10)<<"Audience "<<setw(10)<<" Gene "<<endl;
	for(int i=0;i<PopulationSize;i++){
		Population[i].displayIndividual();
	}
}
void SimulateGA::sortFittest(){
	sort(Population.begin(),Population.end(),compare);
	reverse(Population.begin(),Population.end());
}
void SimulateGA::displayFittest(void){
	cout<<"\nFittest Person: ";
	Population[0].displayIndividual();
}
//======================================================================================//
//====================Member function definitions for System class======================//
//======================================================================================//
bool System::isValidGene(vector<dd>Array){
	int Cost=0;
	int AllAds=0;
	for(int i=0;i<DecesionVariables;i++){
		Cost+=Array[i]*CostPerAdvt[i];
		AllAds+=Array[i];
	}
	if(Cost>Budget){
		return false;
	}
	if(Array[0]<(AllAds*0.1)){
		return false;
	}
	if(Array[1]>10){
		return false;
	}
	if(Array[2]>(AllAds*0.5)){
		return false;
	}
	return true;
}
bool System::isValidGene(void){
	vector<dd>Array=NoOfAdvt;
	int Cost=0;
	int AllAds=0;
	for(int i=0;i<DecesionVariables;i++){
		Cost+=Array[i]*CostPerAdvt[i];
		AllAds+=Array[i];
	}
	if(Cost>Budget){
		return false;
	}
	if(Array[0]<(AllAds*0.1)){
		return false;
	}
	if(Array[1]>10){
		return false;
	}
	if(Array[2]>(AllAds*0.5)){
		return false;
	}
	return true;
}
dd System::getLowerBound(int i){
	return L[i];
}
dd System::getUpperBound(int i){
	return U[i];
}
int System::getDecesionVariables(void){
	return DecesionVariables;
}
void System::setNoOfAds(vector<dd>Array){
	this->NoOfAdvt=Array;
	dd sum1=0;
	dd sum2=0;
	for(int i=0;i<DecesionVariables;i++){
		sum1+=NoOfAdvt[i]*AudiencePerAdvt[i];
		sum2+=NoOfAdvt[i]*CostPerAdvt[i];
	}
	TotalAudience=sum1;
	TotalCost=sum2;
}
dd System::getTotalAudience(void){
	return TotalAudience;
}
dd System::getTotalCost(void){
	return TotalCost;
}
dd System::getFitness(void){
	return TotalCost;
}
//======================================================================================//
//====================Member function definitions for Individual class==================//
//======================================================================================//
dd Individual::getFitness(void){
	return Fitness;
}
dd Individual::getTotalCost(void){
	return TotalCost;
}
dd Individual::getTotalAudience(void){
	return TotalAudience;
}
int Individual::getDecesionVariables(void){
	return DecesionVariables;
}
bool Individual::isValidGene(vector<dd>Array){
	if(Properties.isValidGene(Array)){
		return true;
	}
	return false;
}
vector<dd> Individual::getGenes(void){
	return Gene;
}
void Individual::displayIndividual(void){
	cout<<setw(10)<<(int)TotalCost<<" "<<setw(10)<<(int)TotalAudience<<" ";
	for(int i=0;i<Gene.size();i++){
		cout<<setw(10)<<(int)Gene[i]<<" ";
	}
	cout<<endl;
}
void Individual::setGenes(vector<dd>Array){
	Gene=Array;
	Properties.setNoOfAds(Gene);
	Fitness=Properties.getFitness();
	TotalCost=Properties.getTotalCost();
	TotalAudience=Properties.getTotalAudience();
}
int main(){
	//ToDo implement the mutation function so that algorithm doesn't gets stuck at local optima
	srand(100);	//seed value for rand() in case C++11 compiler is not available otherwise #include<random> will do the job
	SimulateGA g;
	cout<<"\n=========================Generation 1========================\n";
	g.displayPopulation();
	g.displayFittest();
	for(int i=0;i<50;i++){
		cout<<"\n=========================Generation "<<i+1<<"========================\n";
		g.doCrossover();
	//	g.displayPopulation();
		g.displayFittest();
	}
	return 0;
}
