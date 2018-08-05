import java.util.Random;
class SYSTEM{
	int DecisionVariables;
	double Budget;
	double Cost;
	double NoOfAds[];  //audience targeting after chromosome creation
	double CostOfAdvt[]; //cost of advt. in each medium as mentioned in problem statement
	double NoOfAudiencePerAds[]; //target audience per advt. as mentioned in problem statement
	double L[];
	double U[];
	SYSTEM(){
		DecisionVariables=3;
		NoOfAds=new double[DecisionVariables];
		CostOfAdvt=new double[DecisionVariables];
		NoOfAudiencePerAds=new double[DecisionVariables];
		Budget=18200;
		CostOfAdvt[0]=2000;
		CostOfAdvt[1]=600;
		CostOfAdvt[2]=300;
		NoOfAudiencePerAds[0]=100000;
		NoOfAudiencePerAds[1]=40000;
		NoOfAudiencePerAds[2]=18000;
		L=new double[DecisionVariables];
		U=new double[DecisionVariables];
		L[0]=0;
		U[0]=20;
		L[1]=0;
		U[1]=20;
		L[2]=0;
		U[2]=20;
	}
	public void setNoOfAds(double NoOfAds[]) {  
		this.NoOfAds=NoOfAds;
	}
	public boolean isValidGene() {
		Cost=0;
		double Sum=0;
		for(int i=0;i<NoOfAds.length;i++) {
			Cost+=NoOfAds[i]*CostOfAdvt[i];
			Sum+=NoOfAds[i];
		}
		if(Cost>Budget) {
			return false;
		}
		if(NoOfAds[0]<Sum*0.1) {
			return false;
		}
		if(NoOfAds[1]>10) {
			return false;
		}
		if(NoOfAds[2]>Sum*0.5) {
			return false;
		}
		return true;
	}
	public boolean isValidGene(double tempNoOfAds[]) {
		double tempCost=0;
		double tempSum=0;
		for(int i=0;i<NoOfAds.length;i++) {
			tempCost+=tempNoOfAds[i]*CostOfAdvt[i];
			tempSum+=NoOfAds[i];
		}
		if(tempCost>Budget) {
			return false;
		}
		if(tempNoOfAds[0]<tempSum*0.1) {
			return false;
		}
		if(tempNoOfAds[1]>10) {
			return false;
		}
		if(tempNoOfAds[2]>tempSum*0.5) {
			return false;
		}
		return true;
	}
	public double getCost() {  //return total cost of the advt based on the current NoOfAds count
		return Cost;
	}
	public int getDecisionVariables() { 
		return DecisionVariables;
	}
	public double getLowerBound(int i) {
		return L[i];
	}
	public double getUpperBound(int i) {
		return U[i];
	}
	public double getAudience() { //return total number of audience the current advt count can target
		double Sum=0;
		for(int i=0;i<DecisionVariables;i++) {
			Sum+=NoOfAudiencePerAds[i]*NoOfAds[i];
		}
		return Sum;
	}
}
class CHROMOSOME{
	int NoOfGenes; //NoOfGenes is equals to the Decision variables
	double Genes[];
	SYSTEM sys; 
	Random r=new Random();
	CHROMOSOME(SYSTEM sys){
		this.sys=sys;
		NoOfGenes=sys.getDecisionVariables(); 
		Genes=new double[NoOfGenes]; 
		do {
			for(int i=0;i<NoOfGenes;i++) {
				Genes[i]=sys.getLowerBound(i)+(sys.getUpperBound(i)-sys.getLowerBound(i))*r.nextDouble();
				sys.setNoOfAds(Genes); //here in problem genes actually simulating the number of ads, so set the number of ads at SYSTEM to be later used with isValid function
			}
		}while(!sys.isValidGene()); //if gene is not valid then repeat till it becomes valid
	}
	public void displayGenes() {
		for(int i=0;i<Genes.length;i++) {
			System.out.print(" "+(int)Genes[i]);
		}
	}
	public SYSTEM getSys() {
		return sys;
	}
	public boolean setGenes(double genes[]) {
		if(sys.isValidGene(genes)) {
			sys.setNoOfAds(genes);
			Genes=genes;
			return true;
		}
		return false;
	}
	
}
class INDIVIDUAL{
	double TotalCost;  
	double TotalAudience;
	SYSTEM sys;
	CHROMOSOME Chromosome;
	INDIVIDUAL(SYSTEM sys){
		Chromosome=new CHROMOSOME(sys); //create a valid chromosome
		this.sys=Chromosome.sys;  
		TotalCost=sys.getCost(); //retrieve total cost this chromosome requires 
		TotalAudience=sys.getAudience(); //retrieve the total number of Audience this chromosome can target
	}
	public double getTotalCost() {
		return TotalCost;
	}
	public double getTotalAudience() {
		return TotalAudience;
	}
	public void Update() {
		TotalCost=sys.getCost(); //retrieve total cost this chromosome requires 
		TotalAudience=sys.getAudience(); //retrieve the total number of Audience this chromosome can target
	}
	
}
class GASimulation{
	INDIVIDUAL Population[];
	//ArrayList<INDIVIDUAL>Population;
	int PopulationSize;
	double MutationRate;
	double CrossoverRate;
	double MutationValue;
	int NumberOfCrossoverPoints;
	SYSTEM sys;
	Random r=new Random();
	GASimulation(SYSTEM sys){
		this.sys=sys;
		PopulationSize=100;
		MutationRate=0.06;
		CrossoverRate=0.8;
		MutationValue=1.023;
		NumberOfCrossoverPoints=1;
		Population=new INDIVIDUAL[PopulationSize];
		//Population=new ArrayList<INDIVIDUAL>(PopulationSize);
		for(int i=0;i<PopulationSize;i++) {
			Population[i]=new INDIVIDUAL(sys);
		}
	}
	public void DisplayPopulation() {
		System.out.print("\nPopulation:- \n");
		for(int i=0;i<PopulationSize;i++) {
			System.out.print("INDEX: "+i+"\t");
			Population[i].Chromosome.displayGenes();
			System.out.print("\t Cost: "+(int)Population[i].getTotalCost()+"\tAudience: "+(int)Population[i].getTotalAudience()+"\n");
		}
	}
	public double getMaxPopulation() {
		double max=0;
		for(int i=0;i<PopulationSize;i++) {
			if(Population[i].getTotalAudience()>max) {
				max=Population[i].getTotalAudience();
			}
		}
		return max;
	}
	public double getAveragePopulation() {
		double avg=0;
		for(int i=0;i<PopulationSize;i++) {
				avg=+Population[i].getTotalAudience();
		}
		return avg/PopulationSize;
	}
	public void doCrossover() {
		int NoOfGenes=Population[0].Chromosome.NoOfGenes;
		double temp1[]=new double[NoOfGenes];
		double temp2[]=new double[NoOfGenes];
		int count=0;
		int p1,p2;
		while(count!=500) {
		do {
		p1=r.nextInt(PopulationSize);
		p2=r.nextInt(PopulationSize);
		}while(p1>=PopulationSize && p2>PopulationSize);
		int point=r.nextInt(NoOfGenes+1); //select crossover point
		//point--;
		for(int i=0;i<NoOfGenes;i++) {
			if(i!=point) {
				temp1[i]=Population[p1].Chromosome.Genes[i];
				temp2[i]=Population[p2].Chromosome.Genes[i];
			}
			else {
				temp2[i]=Population[p1].Chromosome.Genes[i];
				temp1[i]=Population[p2].Chromosome.Genes[i];
			}
		}
		INDIVIDUAL child1=new INDIVIDUAL(new SYSTEM());
		INDIVIDUAL child2=new INDIVIDUAL(new SYSTEM());
		temp1=doMutation(temp1);
		temp2=doMutation(temp2);
		if(child1.Chromosome.setGenes(temp1)) {
			child1.Update();
		}
		if(child2.Chromosome.setGenes(temp2)) {
			child2.Update();
		}
		if(child1.TotalAudience>Population[p1].TotalAudience) {
			Population[p1]=child1;
			if(child2.TotalAudience>Population[p2].TotalAudience) {
				Population[p2]=child2;	
			}
		}
		else if(child1.TotalAudience>Population[p2].TotalAudience) {
			Population[p2]=child1;
			if(child2.TotalAudience>Population[p1].TotalAudience) {
				Population[p1]=child2;
			}
		}
		count++;
		}
	}
	public double[] doMutation(double Array[]) {
		for(int i=0;i<Array.length;i++) {
			double num=r.nextDouble();
			if(num<MutationRate) {
				Array[i]=(Array[i]+MutationValue);
			}
		}
		return Array;
	}
}
public class GeneticAlgorithm {
	public static void main(String[] args) {
		GASimulation g=new GASimulation(new SYSTEM());
		g.DisplayPopulation();
		System.out.print("\nMax Targetted Audience: "+(int)g.getMaxPopulation());
		g.doCrossover();
		System.out.print("\nMax Targetted Audience after crossover: "+(int)g.getMaxPopulation());
		g.doCrossover();
		System.out.print("\nMax Targetted Audience after corssover: "+(int)g.getMaxPopulation());
		g.doCrossover();
		for(int i=0;i<1000;i++) {
			g.doCrossover();
		}
		g.DisplayPopulation();
		System.out.print("\nMax Targetted Audience after 100 generations : "+(int)g.getMaxPopulation());

	}
}