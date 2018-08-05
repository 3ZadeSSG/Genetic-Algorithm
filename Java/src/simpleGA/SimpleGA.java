package simpleGA;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Random;
class Problem{
	private double CostPerAdvt[];
	private double AudiencePerAdvt[];
	private double NoOfAdvt[];
	private double L[];
	private double U[];
	private int DecesionVariables;
	private double TotalCost;
	private double TotalAudience;
	private double Fitness;
	private double Budget;
	public Problem() {
		DecesionVariables=3;
		CostPerAdvt=new double[DecesionVariables];
		AudiencePerAdvt=new double[DecesionVariables];
		NoOfAdvt=new double[DecesionVariables];
		L=new double[DecesionVariables];
		U=new double[DecesionVariables];
		CostPerAdvt[0]=2000;
		CostPerAdvt[1]=600;
		CostPerAdvt[2]=400;
		AudiencePerAdvt[0]=100000;
		AudiencePerAdvt[1]=40000;
		AudiencePerAdvt[2]=18000;
		L[0]=0;
		L[1]=0;
		L[2]=0;
		U[0]=20;
		U[1]=20;
		U[2]=20;
		TotalCost=Fitness=TotalAudience=0;
		Budget=18000;
	}
	public double getLowerBound(int i) {
		return L[i];
	}
	public double getUpperBound(int i) {
		return U[i];
	}
	public void setNoOfAdvt(double NoOfAdvt[]) {
		this.NoOfAdvt=NoOfAdvt;
		TotalCost=0;
		TotalAudience=0;
		for(int i=0;i<DecesionVariables;i++) {
			TotalCost+=NoOfAdvt[i]*CostPerAdvt[i];
			TotalAudience+=NoOfAdvt[i]*AudiencePerAdvt[i];
		}
		Fitness=TotalAudience;
	}
	public boolean isGeneValid(double Array[]) {
		double sumCost=0,sumAdvt=0;
		for(int i=0;i<DecesionVariables;i++) {
			sumCost+=Array[i]*CostPerAdvt[i];
			sumAdvt+=Array[i];
		}
		if(sumCost>Budget) {
			return false;
		}
		if(Array[0]<sumAdvt*0.1) {
			return false;
		}
		if(Array[1]>10) {
			return false;
		}
		if(Array[2]>sumAdvt*0.5) {
			return false;
		}
		return true;
	}
	public boolean isGeneValid() {
		double sumAdvt=0;
		if(TotalCost>Budget) {
			return false;
		}
		for(int i=0;i<NoOfAdvt.length;i++) {
			sumAdvt+=NoOfAdvt[i];
		}
		if(NoOfAdvt[0]<sumAdvt*0.1) {
			return false;
		}
		if(NoOfAdvt[1]>10) {
			return false;
		}
		if(NoOfAdvt[2]>sumAdvt*05) {
			return false;
		}
		return true;
	}
	public int getDecesionVariables() {
		return DecesionVariables;
	}
	public double getFitness() {
		return Fitness;
	}
	public double getTotalCost() {
		return TotalCost;
	}
	public double getTotalAudience() {
		return TotalAudience;
	}
}
class Individual{
	Random random;
	private double Gene[];
	int DecesionVariables;
	Problem properties;
	double Fitness;
	double TotalCost;
	double TotalAudience;
	public Individual() {
		properties=new Problem();
		DecesionVariables=properties.getDecesionVariables();
		random=new Random();
		Gene=new double[DecesionVariables];
		do {
			for(int i=0;i<DecesionVariables;i++) {
				Gene[i]=properties.getLowerBound(i)+(properties.getUpperBound(i)-properties.getLowerBound(i))*random.nextDouble();
			}
		}while(!properties.isGeneValid(Gene));
		properties.setNoOfAdvt(Gene);
		TotalCost=properties.getTotalCost();
		TotalAudience=properties.getTotalAudience();
		Fitness=properties.getFitness();
	}
	public void displayIndividual() {
		System.out.print("Cost: "+(int)TotalCost+"\tAudience: "+(int)TotalAudience);
		for(int i=0;i<DecesionVariables;i++) {
			System.out.print("\t "+(int)Gene[i]);
		}
	}
	public double getFitness() {
		return Fitness;
	}
	public boolean isGeneValid(double Array[]) {
		/*if(properties.isGeneValid(Array)) {
			return true;
		}
		return false;*/
		return properties.isGeneValid(Array);
	}
	public void setGenes(double Array[]) {
		Gene=Array;
		properties.setNoOfAdvt(Gene);
		TotalCost=properties.getTotalCost();
		TotalAudience=properties.getTotalAudience();
		Fitness=properties.getFitness();
	}
	public double[] getGene() {
		return Gene;
	}
}
class SimulateGA{
	Random random;
	ArrayList<Individual>Population;
	int PopulationSize;
	double CrossoverRate;
	double MutationRate;
	int CrossoverPoint;
	double MutationValue;
	public SimulateGA() {
		MutationValue=1.023;
		CrossoverRate=0.6;
		CrossoverPoint=1;
		MutationRate=0.08;
		random=new Random();
		PopulationSize=100;
		Population=new ArrayList<Individual>();
		for(int i=0;i<PopulationSize;i++) {
			Population.add(new Individual());
		}
	}
	public void displayPopulation() {
		System.out.print("\n===================Population==============\n");
		for(int i=0;i<PopulationSize;i++) {
			System.out.print("\nIndex: "+(i+1)+" ");
			Population.get(i).displayIndividual();
		}
	}
	public void displayFittest() {
		int index=0;
		for(int i=0;i<PopulationSize;i++) {
			if(Population.get(i).getFitness()>Population.get(index).getFitness()) {
				index=i;
			}
		}
		System.out.print("\nFittest Individual: ");
		Population.get(index).displayIndividual();
	}
	public void doCrossoverWithMutation() {
		ArrayList<Individual>NewPopulation=new ArrayList<Individual>();
		ArrayList<Integer>MatingPair1=new ArrayList<Integer>();
		ArrayList<Integer>MatingPair2=new ArrayList<Integer>();
		for(int i=0;i<PopulationSize/2;i++) {
			MatingPair1.add(random.nextInt(99-0));
			MatingPair2.add(random.nextInt(99-0));
		}
		for(int i=0;i<MatingPair1.size();i++) {
			Individual Parent1=Population.get(MatingPair1.get(i));
			Individual Parent2=Population.get(MatingPair2.get(i));
			double Gene1[]=Parent1.getGene();
			double Gene2[]=Parent2.getGene();
			int point=random.nextInt(Gene1.length+1);
			for(int index=point;index<Gene1.length;index++) {
				double temp=Gene1[index];
				Gene1[index]=Gene2[index];
				Gene2[index]=temp;
			}
			for(int id=0;id<Gene1.length;id++) {
				double num=random.nextDouble();
				if(num<MutationRate) {
					Gene1[id]=(Gene1[id]*MutationValue);
				}
				//System.out.print(" "+(int)Gene1[id]);
			}
			//System.out.println();
			for(int id=0;id<Gene2.length;id++) {
				double num=random.nextDouble();
				if(num<MutationRate) {
					Gene2[id]=(Gene2[id]*MutationValue);
				}
			}
			
			
			Individual Child1=new Individual();
			Individual Child2=new Individual();
			Child1.setGenes(Gene1);
			Child2.setGenes(Gene2);
			ArrayList<Individual>Family=new ArrayList<Individual>();
			Family.add(Parent1);
			Family.add(Parent2);
			if(Child1.isGeneValid(Gene1)) {
				Family.add(Child1);
			}
			if(Child2.isGeneValid(Gene2)) {
				Family.add(Child2);
			}
			Collections.sort(Family, new CustomComparator());	
			NewPopulation.add(Family.get(Family.size()-1));
			NewPopulation.add(Family.get(Family.size()-2));
		}
		Population=NewPopulation;
	}
	
	
	public void doCrossoverWithoutMutation() {
		ArrayList<Individual>NewPopulation=new ArrayList<Individual>();
		ArrayList<Integer>MatingPair1=new ArrayList<Integer>();
		ArrayList<Integer>MatingPair2=new ArrayList<Integer>();
		for(int i=0;i<PopulationSize/2;i++) {
			MatingPair1.add(random.nextInt(99-0));
			MatingPair2.add(random.nextInt(99-0));
		}
		for(int i=0;i<MatingPair1.size();i++) {
			Individual Parent1=Population.get(MatingPair1.get(i));
			Individual Parent2=Population.get(MatingPair2.get(i));
			double Gene1[]=Parent1.getGene();
			double Gene2[]=Parent2.getGene();
			int point=random.nextInt(Gene1.length+1);
			for(int index=point;index<Gene1.length;index++) {
				double temp=Gene1[index];
				Gene1[index]=Gene2[index];
				Gene2[index]=temp;
			}
			Individual Child1=new Individual();
			Individual Child2=new Individual();
			Child1.setGenes(Gene1);
			Child2.setGenes(Gene2);
			ArrayList<Individual>Family=new ArrayList<Individual>();
			Family.add(Parent1);
			Family.add(Parent2);
			if(Child1.isGeneValid(Gene1)) {
				Family.add(Child1);
			}
			if(Child2.isGeneValid(Gene2)) {
				Family.add(Child2);
			}
			Collections.sort(Family, new CustomComparator());	
			NewPopulation.add(Family.get(Family.size()-1));
			NewPopulation.add(Family.get(Family.size()-2));
		}
		Population=NewPopulation;
	}
	public double[] doMutation(double Array[]) {
		for(int i=0;i<Array.length;i++) {
			double num=random.nextDouble();
			if(num<MutationRate) {
				Array[i]=(Array[i]+MutationValue);
			}
		}
		return Array;
	}
	
}
class CustomComparator implements Comparator<Individual> {
    @Override
    public int compare(Individual o1,Individual o2) {
        if(o1.getFitness()>o2.getFitness()) {
        	return 1;
        }
        return -1;
    }
}
public class SimpleGA {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SimulateGA g=new SimulateGA();
		g.displayPopulation();
		g.displayFittest();
		/*g.doCrossover();
		g.displayPopulation();
		g.displayFittest();
		g.doCrossover();
		g.displayPopulation();
		g.displayFittest();
		g.doCrossover();
		g.displayPopulation();
		g.displayFittest();*/
		/*for(int i=0;i<100;i++) {
			g.doCrossoverWithoutMutation();
			g.displayFittest();
		}*/
		System.out.println("\n==========================\n");
		for(int i=0;i<100;i++) {
			g.doCrossoverWithoutMutation();
			//g.displayPopulation();
			//g.displayFittest();
		}
		g.displayFittest();
	}

}
