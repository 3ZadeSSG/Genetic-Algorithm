#include<iostream>
#include<vector>
#include<algorithm>
#include<stdint.h>
#include<string.h>
#include<random>
#define random (rand()/(double)RAND_MAX)
#define ll int64_t
#define Add push_back
using namespace std;
//=======================================================================//
//=====================Class Definition of Graph=========================//
//=======================================================================//
class Graph{
	ll TotalVertices;
	vector<pair<ll,ll> >Vertices; //pair of vertices between which path exists
	vector<pair<ll,ll> >Edges; //first one is weight and second one is index corresponding to the vertex pair
	vector<ll>OptimalPath;		//this will store the path for which TotalCost variable's value will be set
	ll TotalCost;
public:
	Graph(ll TotalVertices){
		TotalCost=0;
		this->TotalVertices=TotalVertices;	
	}
	Graph(){				//Add entries for the vertice and edges, addEdge(Vertex1,Vertex2,WeightOfEdge)
		this->TotalVertices=5;
		addEdge(1,2,7);
		addEdge(1,3,50);
		addEdge(1,4,6);
		addEdge(1,5,4);
		addEdge(2,3,11);
		addEdge(2,4,5);
		addEdge(2,5,15);
		addEdge(3,4,8);
		addEdge(3,5,2);
		addEdge(4,5,27);
	}
	void addEdge(ll v1,ll v2,ll weight);	//given verices and edge weight betwene them, add a new entry into graph
	ll getCost(vector<ll>Path);				//give a path funciton will reutrn the travel cost or 0 in case path is not valid
	ll getCost();							//in case the optimal path is set then return the cost of that path
	ll getTotalVertices();					//return total number of vertices in the Graph
	bool isPathValid(vector<ll>Path);		//for checking is path is valid
	void setPath(vector<ll>OptimalPath);	//set the current optimal path as received from the calling function
	ll getDecesionVariables();				//return the number of decesion variables, TotalVertices in this case;
};
//===========================================================//
//===================Class definition of Individual==========//
//===========================================================//
class Individual{
	vector<ll>Gene;							//Gene will hold the path 
	ll DecesionVariables;					//Total number of vertices in this case
	Graph Properties;						//An ofject of Graph type to calculate the path cost and find validity of an randomly choosen path
public:
	Individual(){
		Properties=Graph();					//Create a Graph type object
		DecesionVariables=Properties.getDecesionVariables();
		Gene.resize(DecesionVariables,0);	//Resize the Gene array to the size of TotalVertices and initialize them with 0
		do{
			Gene.clear();					
			Gene.resize(DecesionVariables,0);	
			for(ll i=0;i<Gene.size();i++){
				ll vertex=(random*DecesionVariables)+1;
				while(find(Gene.begin(),Gene.end(),vertex)!=Gene.end()){  //if randomly created vertex is already added in Gene then create a new one
					vertex=(random*DecesionVariables)+1;
				}
				Gene[i]=vertex;			//add created valid vertex into the Gene
			}							 
		}while(!Properties.isPathValid(Gene));	//check if the created path is valid
		Properties.setPath(Gene);				//if path(Gene) is valid then set this path as the optimal path for the Graph object
		this->Fitness=Properties.getCost();		//get the cost of path form the Graph object corresponding to this individual's Gene
	}
	ll Fitness;									
	ll getFitness();							
	vector<ll>getGene();						//return the Gene array 
	bool isGeneValid(vector<ll>Array);			//given an array of path check if it is valid path or not
	void setGene(vector<ll>Gene);				//set the Gene of this individual equal to the received path array
	void displayIndividual();
	ll getDecesionVariables();
};
bool compare(const Individual & l,const Individual & r){	//this compartor function is required for 'sort()' function defined in <algorithm>
    return l.Fitness < r.Fitness;							//for sorting objects according to one of their data members (Cost/Fitness in this case)
}
//===========================================================//
//===================Class definition of SimulateGA==========//
//===========================================================//
class SimulateGA{
	vector<Individual>Population;				//List of population
	ll PopulationSize;							//Initial population size (as for now in this program this must be even)
public:
	SimulateGA(int PopulationSize){
		this->PopulationSize=PopulationSize;	//Initialize the population with given size 
		Population.resize(PopulationSize);		//Resize the population array to the Population size
	}
	void doCrossover();							
	vector<ll> doMutation(vector<ll>Array);		//Mutation on the passed array of genes
	void displayPopulation();					
	void displayFittest();
};
//===========================================================//
//==============Member functions for SimulateGA Class========//
//===========================================================//
void SimulateGA::displayPopulation(){
	for(int i=0;i<Population.size();i++){
		cout<<endl;
		Population[i].displayIndividual();
	}
}
void SimulateGA::displayFittest(){
	cout<<"\nFittest Individual: \n";
	(*min_element(Population.begin(),Population.end(),compare)).displayIndividual();//display the best cost(Min fitness value in this case) individual
}
void SimulateGA::doCrossover(){
	vector<Individual>NewPopulation;
	ll range=Population[0].getDecesionVariables();
	vector<pair<ll,ll> >MatingPair;
	for(ll i=0;i<PopulationSize/2;i++){	//create the mating pair from while population by randomly selecting them
		ll p1=random*PopulationSize;
		ll p2=random*PopulationSize;
		MatingPair.Add(make_pair(p1,p2));
	}
	for(ll index=0;index<MatingPair.size();index++){
		Individual Parent1=Population[MatingPair[index].first];		//Select 1st parent from population as per mating pair 
		Individual Parent2=Population[MatingPair[index].second];	//Select 2nd parent from population as per mating pair 
		vector<ll>Gene1=Parent1.getGene();							//Obtain genes of first parent
		vector<ll>Gene2=Parent2.getGene();							//Obtain genes of the second parent
		vector<ll>Points;											
		Points.Add(random*Gene1.size());							//select two random points
		Points.Add(random*Gene2.size());
		sort(Points.begin(),Points.end());							//sort the random points so that they can be used as a range for swapping
		vector<ll>ChildGene1(range);
		vector<ll>ChildGene2(range);
		for(ll i=Points[0];i<Points[1];i++){						//add the genes of parents within the selected range
			ChildGene1[i]=Gene1[i];
			ChildGene2[i]=Gene2[i];
		}
		for(ll i=0;i<range;i++){									//add remaining genes in such a way that no two verties are repeated
			if(ChildGene1[i]==0){
				ll id=0;
				ll flag=0;
				do{
					if(find(ChildGene1.begin(),ChildGene1.end(),Gene2[id])==ChildGene1.end()){
						ChildGene1[i]=Gene1[id];
						flag=1;
					}
					id++;
				}while(flag==0);
			}
			if(ChildGene2[i]==0){
				ll id=0;
				ll flag=0;
				do{
					if(find(ChildGene2.begin(),ChildGene2.end(),Gene1[id])==ChildGene2.end()){
						ChildGene1[i]=Gene1[id];
						flag=1;
					}
					id++;
				}while(flag==0);
			}
		}
		ChildGene1=doMutation(ChildGene1);				//do mutaiton on this gene array of both childs
		ChildGene2=doMutation(ChildGene2);
		vector<Individual>Family;
		Family.Add(Parent1);						//add both parnets to family list 
		Family.Add(Parent2);
		Individual Child1;
		Individual Child2;
		if(Child1.isGeneValid(ChildGene1)){			//add children if they have valid gene
			Child1.setGene(ChildGene1);
			Family.Add(Child1);
		}
		if(Child1.isGeneValid(ChildGene2)){
			Child1.setGene(ChildGene2);
			Family.Add(Child2);
		}
		sort(Family.begin(),Family.end(),compare);	//sort the family based on their fitness (lower fitness here in this case because of minimization problem)
		NewPopulation.Add(Family[0]);		//select top 2 individual whether its parent or child and add them into new list of population
		NewPopulation.Add(Family[1]);
	}
	Population.clear();
	Population=NewPopulation;		//replace the current population with new population 
}
vector<ll> SimulateGA::doMutation(vector<ll>Array){
	Individual Person;
	ll r1=random*Array.size();
	ll r2=random*Array.size();
	vector<ll>Temp=Array;
	Temp=Array;
	ll t=Temp[r1];
	Temp[r1]=Temp[r2];
	Temp[r2]=t;
	return Temp;
}
//===========================================================//
//==============Member functions for Individual Class========//
//===========================================================//
ll Individual::getFitness(){
	return Fitness;
}
vector<ll> Individual::getGene(){
	return Gene;
}
bool Individual::isGeneValid(vector<ll>Array){
	if(Properties.isPathValid(Array)){
		return true;
	}
	return false;
}
void Individual::setGene(vector<ll>Gene){
	this->Gene=Gene;
	Properties.setPath(Gene);
	this->Fitness=Properties.getCost();
}
void Individual::displayIndividual(){
	cout<<"Path: ";
	for(int i=0;i<Gene.size();i++){
		cout<<Gene[i]<<" ";
	}
	cout<<Gene[0]<<" "; //since last and first vertex has to be same
	cout<<"Fitness: "<<Fitness;
}
ll Individual::getDecesionVariables(){
	return this->DecesionVariables;
}
//===========================================================//
//================Member functions for Graph Class===========//
//===========================================================//
bool Graph::isPathValid(vector<ll>Path){
	if(getCost(Path)==0){
		return false;
	}
	return true;
}
void Graph::addEdge(ll v1,ll v2,ll weight){
	Vertices.Add(make_pair(v1,v2));
	Edges.Add(make_pair(weight,Vertices.size()-1));
}
ll Graph::getCost(vector<ll>Path){
	ll cost=0;
	vector<ll>visited;		//a list of all visited vertices
	for(ll i=0;i<Path.size()-1;i++){
		ll v1=Path[i];
		ll v2=Path[i+1];	
		if(find(visited.begin(),visited.end(),v1)==visited.end()){	//select the two vertices from path and check if first one is already visited
			visited.Add(v1);		//if not visited then add the first vertex to visited list
		}
		else{
			return 0;	//if vertex is visited more than onece then return 0 as cost indicating invalid path
		}
		if(i==Path.size()-2){
			if(find(visited.begin(),visited.end(),v2)==visited.end()){
			visited.Add(v2);	//if last vertex is not repeated then add it to visited list 
			}
			else{
				return 0; 	//if last vertex was visited once then return 0 as indicator of invalid cost
			}
		}
		if(find(Vertices.begin(),Vertices.end(),make_pair(v1,v2))<Vertices.end()){	
			cost+=Edges[(find(Vertices.begin(),Vertices.end(),make_pair(v1,v2))-Vertices.begin())].first;
		}
		else if(find(Vertices.begin(),Vertices.end(),make_pair(v2,v1))<Vertices.end()){
			cost+=Edges[(find(Vertices.begin(),Vertices.end(),make_pair(v2,v1))-Vertices.begin())].first;
		}
		else{
			//cout<<"\nNo path found! ";
			return 0;
		}
	}
	//find cost of edge between last and first vertex because in TSP we need to return back from where we started
	if(find(Vertices.begin(),Vertices.end(),make_pair(Path[Path.size()-1],Path[0]))<Vertices.end()){
			cost+=Edges[(find(Vertices.begin(),Vertices.end(),make_pair(Path[Path.size()-1],Path[0]))-Vertices.begin())].first;
	}
	else if(find(Vertices.begin(),Vertices.end(),make_pair(Path[0],Path[Path.size()-1]))<Vertices.end()){
			cost+=Edges[(find(Vertices.begin(),Vertices.end(),make_pair(Path[0],Path[Path.size()-1]))-Vertices.begin())].first;
	}
	else{
		return 0;
	}
	return cost;
}
ll Graph::getTotalVertices(){
	return TotalVertices;	
}
void Graph::setPath(vector<ll>OptimalPath){
	this->OptimalPath=OptimalPath;
	TotalCost=getCost(OptimalPath);
}
ll Graph::getCost(){
	return TotalCost;
}
ll Graph::getDecesionVariables(){
	return TotalVertices;
}
int main(){
	SimulateGA g=SimulateGA(100);
	cout<<"\n==================Initial Population==============\n";
	//g.displayPopulation();
	g.displayFittest();
	for(int i=0;i<10;i++){
	cout<<"\n==================Generation "<<i+1<<"==============\n";
	g.doCrossover();
	g.displayFittest();
	}
	//g.displayPopulation();
	g.displayFittest();
	return 0;
}
