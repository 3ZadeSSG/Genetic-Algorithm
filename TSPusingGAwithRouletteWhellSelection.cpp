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
	vector<ll>OptimalPath;
	ll TotalCost;
public:
	Graph(ll TotalVertices){
		TotalCost=0;
		this->TotalVertices=TotalVertices;
	}
	Graph(){
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
	void addAll(vector<pair<ll,ll> >Vertices,vector<pair<ll,ll> >Edges);
	void addEdge(ll v1,ll v2,ll weight);	//given verices and edge weight betwene them, add a new entry into graph
	void display(void);						//display the current graph contents
	ll getCost(vector<ll>Path);				//give a path funciton will reutrn the travel cost or 0 in case path is not valid
	ll getCost();							//in case the optimal path is set then return the cost of that path
	ll getTotalVertices();					//return total number of vertices in the Graph
	bool isPathValid(vector<ll>Path);		//for checking is path is valid
	void setPath(vector<ll>OptimalPath);	//set the current optimal path as received from the calling function
	ll getDecesionVariables();
};
//===========================================================//
//===================Class definition of Individual==========//
//===========================================================//
class Individual{
	vector<ll>Gene;
	ll DecesionVariables;
	Graph Properties;
public:
	Individual(){
		Properties=Graph();
		DecesionVariables=Properties.getDecesionVariables();
		Gene.resize(DecesionVariables,0);
		do{
			Gene.clear();
			Gene.resize(DecesionVariables,0);
			for(ll i=0;i<Gene.size();i++){
				ll vertex=(random*DecesionVariables)+1;
				while(find(Gene.begin(),Gene.end(),vertex)!=Gene.end()){
					vertex=(random*DecesionVariables)+1;
				}
				Gene[i]=vertex;			}
		}while(!Properties.isPathValid(Gene));
		Properties.setPath(Gene);
		this->Fitness=Properties.getCost();
	}
	ll Fitness;
	ll getFitness();
	vector<ll>getGene();
	bool isGeneValid(vector<ll>Array);
	void setGene(vector<ll>Gene);
	void displayIndividual();
	ll getDecesionVariables();
};
bool compare(const Individual & l,const Individual & r)
{
    return l.Fitness < r.Fitness;
}
//===========================================================//
//===================Class definition of SimulateGA==========//
//===========================================================//
class SimulateGA{
	vector<Individual>Population;
	ll PopulationSize;
public:
	SimulateGA(int PopulationSize){
		this->PopulationSize=PopulationSize;
		Population.resize(PopulationSize);
	}
	void doCrossover();
	vector<ll> doMutation(vector<ll>Array);
	void displayPopulation();
	void displayFittest();
	vector<pair<ll,ll> > RouletteWheelSelection();
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
	(*min_element(Population.begin(),Population.end(),compare)).displayIndividual();
}
vector<pair<ll,ll> > SimulateGA::RouletteWheelSelection(){
	vector<pair<ll,ll> >MatingPair;
	ll SumFitness=0;
	for(ll i=0;i<PopulationSize;i++){
		SumFitness+=Population[i].getFitness();
	}
	for(ll i=0;i<PopulationSize/2;i++){
		ll r=random*SumFitness;
		ll Sum=0;
		ll j=0;
		while(Sum<r){
			Sum+=Population[j].getFitness();
			j++;
		}
		ll pair1=--j;
		r=random*SumFitness;
		Sum=0;
		j=0;
		while(Sum<r){
			Sum+=Population[j].getFitness();
			j++;
		}
		ll pair2=--j;
		MatingPair.Add(make_pair(pair1,pair2));
	}
	return MatingPair;
}
void SimulateGA::doCrossover(){
	vector<Individual>NewPopulation;
	ll range=Population[0].getDecesionVariables();
	vector<pair<ll,ll> >MatingPair;
	MatingPair=RouletteWheelSelection(); //call Roulette Wheel Selection function to select the mating pairs
	for(ll index=0;index<MatingPair.size();index++){
		Individual Parent1=Population[MatingPair[index].first];
		Individual Parent2=Population[MatingPair[index].second];
		vector<ll>Gene1=Parent1.getGene();
		vector<ll>Gene2=Parent2.getGene();
		vector<ll>Points;
		Points.Add(random*Gene1.size());
		Points.Add(random*Gene2.size());
		sort(Points.begin(),Points.end());
		vector<ll>ChildGene1(range);
		vector<ll>ChildGene2(range);
		for(ll i=Points[0];i<Points[1];i++){
			ChildGene1[i]=Gene1[i];
			ChildGene2[i]=Gene2[i];
		}
		for(ll i=0;i<range;i++){
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
		ChildGene1=doMutation(ChildGene1);
		ChildGene2=doMutation(ChildGene2);
		vector<Individual>Family;
		Family.Add(Parent1);
		Family.Add(Parent2);
		Individual Child1;
		Individual Child2;
		if(Child1.isGeneValid(ChildGene1)){
			Child1.setGene(ChildGene1);
			Family.Add(Child1);
		}
		if(Child1.isGeneValid(ChildGene2)){
			Child1.setGene(ChildGene2);
			Family.Add(Child2);
		}
		sort(Family.begin(),Family.end(),compare);
		NewPopulation.Add(Family[0]);
		NewPopulation.Add(Family[1]);
	}
	Population.clear();
	Population=NewPopulation;
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
void Graph::addAll(vector<pair<ll,ll> >Vertices,vector<pair<ll,ll> >Edges){
	this->Vertices=Vertices;
	this->Edges=Edges;
}
void Graph::addEdge(ll v1,ll v2,ll weight){
	Vertices.Add(make_pair(v1,v2));
	Edges.Add(make_pair(weight,Vertices.size()-1));
}
void Graph::display(void){
	cout<<"\nGraph Contents: \n";
	for(int i=0;i<Edges.size();i++){
		cout<<"Vertex: "<<Vertices[i].first<<" Vertex: "<<Vertices[i].second<<" Edge: "<<Edges[i].first<<endl;
	}
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
ll Graph::getCost(vector<ll>Path){
	ll cost=0;
	vector<ll>visited;
	for(ll i=0;i<Path.size()-1;i++){
		ll v1=Path[i];
		ll v2=Path[i+1];
		if(find(visited.begin(),visited.end(),v1)==visited.end()){
			visited.Add(v1);
		}
		else{
			return 0;
		}
		if(i==Path.size()-2){
			if(find(visited.begin(),visited.end(),v2)==visited.end()){
			visited.Add(v2);
			}
			else{
				return 0;
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
	return cost;
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
