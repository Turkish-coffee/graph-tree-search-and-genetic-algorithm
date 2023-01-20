#include <iostream>
#include <iomanip>
#include <random>
#include <map>
#include <array>
#include <math.h>
#include <algorithm>
#include <vector>


// defining the constants of the problem: -------------------------------------
const int CITY_NUMBER = 5;
const int ITEM_NUMBER = 5;
const int GENERATION_SIZE = 10000;
const int GENERATION_NUMBER = 100000;
std::array<int, ITEM_NUMBER> stock = {30,40,20,40,20};
std::array<std::array<int, ITEM_NUMBER>, CITY_NUMBER> prices {{{1,4,6,4,4},
                                                               {3,8,2,5,15},
                                                               {3,12,3,5,5},
                                                               {2,6,10,2,4},
                                                               {10,5,12,6,3}}};

//-----------------------------------------------------------------------------



// helper function to create properly the parents without having to parse them 
// in order to fit to the hard constraints 
void Normalize(std::array<int,5> (&array), int stockNumber) {
    
    for (int i = 0; i < 5; i++)
    {  
        array[i] = rand() % (stockNumber);
    }
    std::sort(array.begin(), array.end());
    array[4] = stockNumber - array[3];
    array[3] = array[3] - array[2];
    array[2] = array[2] - array[1];
    array[1] = array[1] - array[0];
}


class Solution {

    public:


    std::array<std::array<int, ITEM_NUMBER>, CITY_NUMBER> params;
    double FitnessValue;



    //we have to normalize the Solutions at the beginning to avoid the hard constraints 
    Solution() {
        
        for (int i =0; i<5; i++)
        {
            Normalize(this->params[i], stock[i]);
        }
        
    }

    // function which will evaluate the compatibility of the solution to the problem
    void fitnessFunction() {

        double score = 0.0;
        
        int F1 = 100;
        double F2 = 0.0;
        double F3 = 0.0;

        
        for (int i = 0; i < 5; i++)
        {
            int sumOfItemCost = 0;
            for (int j = 0; j < 5; j++)
            {   
                sumOfItemCost += this->params[i][j] * prices[i][j];;
            }
            //calculating fbase  
            score += sumOfItemCost;

            
            int max = *std::max_element(this->params[i].begin(),this->params[i].end());
            int min = *std::min_element(this->params[i].begin(),this->params[i].end());
            F2 += (double)(std::max((20 - (max - min)), 0)*sumOfItemCost)/100;
            //std::cout << "score assigned to F2 : "<< (double)(std::max((20 - (max - min)), 0)*sumOfItemCost)/100 << std::endl;


            int sumOfCityCost = 0;
            int cityCostMax = 0;
            int cityCostMin = 30;
            for (int j = 0; j < 5; j++)
            {
                sumOfCityCost += this->params[j][i] * prices[j][i];
                if (this->params[j][i] > cityCostMax)
                {
                    cityCostMax = this->params[j][i];
                }
                else if (this->params[j][i] < cityCostMin)
                {
                    cityCostMin = this->params[j][i];
                }
 
            }
            // sumOfCity == 0 means nothing have been selt in that city,  
            // in other words, has never visited it.
            if (sumOfCityCost == 0)
            {
                F1 = 0;
            }

            //change score to sumOfCostCity if wanted 
            F3 += (double)(std::max((20 - (cityCostMax -cityCostMin)), 0)*score)/100;
            //std::cout << "score assigned to F3 : "<< (double)(std::max((20 - (cityCostMax -cityCostMin)), 0)*score)/100 << std::endl;  


        } 
        //---------------------------------------------------------- 
        //std::cout << "base score : "<< score << std::endl; 
        //std::cout << "F1: " << F1 <<std::endl;
        //std::cout << "F2: " << F2 <<std::endl;
        //std::cout << "F3: " << F3 <<std::endl;
        score += F1;
        score += F2;
        score += F3;
        //std::cout << " total score: "<< score << std::endl;
        this->FitnessValue = score;
        return;
    }

//------------------------------------------------------------------------------------------------------------------------------------------------------
    //function which will generate the first generation of Solution randomly
    static void generateSolutions(std::array<Solution*, GENERATION_SIZE> &array) {    

        for (int i = 0; i<GENERATION_SIZE;i++)
        {
            array[i] = new Solution();
            array[i]->fitnessFunction();
            
        }
                
        return;
    }

    //for the cross-over, I randomly choose a row of both sol
    static void solutionCrossOver(const std::array<Solution*, GENERATION_SIZE> &array1, const std::array<Solution*, GENERATION_SIZE> &array2, int solution1, int solution2) {

        int modifiedItem = rand() % 5;
        for (int i = 0; i < 5; i++) {
            int tmp = array1[solution1]->params[modifiedItem][i];          
            array1[solution1]->params[modifiedItem][i] = array2[solution2]->params[modifiedItem][i];
            array2[solution2]->params[modifiedItem][i] = tmp;
            
        }
        return; 
    }
//------------------------------------------------------------------------------------------------------------------------------------------------------
    void solutionMutate() {

            std::array<std::array<int, ITEM_NUMBER>, CITY_NUMBER> tmp;
            std::copy(std::begin(this->params), std::end(this->params), std::begin(tmp));
            this->fitnessFunction(); 
            int tmpFitness = this->FitnessValue;

            //whcich row will be muted 
            int alleleMutation = rand() % 5;
            int nucleotid = rand() % 5;
            // the row which will mutate will know a mutation between the itembounds 
            int mutation = rand() % stock[alleleMutation];
            int difference = this->params[alleleMutation][nucleotid] - mutation;
            this->params[alleleMutation][nucleotid] = mutation; 
            int a = 0;
            while (difference != 0) {
                
                int someOtherNucleotid = rand() % 5;        
                if ( someOtherNucleotid==nucleotid || (this->params[alleleMutation][someOtherNucleotid] == 0 && difference < 0))
                {   
                    
                    continue;
                }
                
                
                if (difference > 0)
                {
                    this->params[alleleMutation][someOtherNucleotid] ++;
                    difference--; 
                }
                else 
                {
                    this->params[alleleMutation][someOtherNucleotid] --;
                    difference++;      
                }
         
            }

            this->fitnessFunction();
            if (this->FitnessValue < tmpFitness)
            {
                std::copy(std::begin(tmp), std::end(tmp), std::begin(this->params));
                this->fitnessFunction();
            }    
            
        return; 
    }


    void printSolution() {
        std::cout << "Fitness value: " << this->FitnessValue << "\n" << std::endl;

        for (int i = 0; i < ITEM_NUMBER; i++)
        {
            for (int j = 0; j < CITY_NUMBER; j++)
            {
                std::cout << this->params[i][j] << " ";
            }
            std::cout <<""<< std::endl; 
        }
    }

};



int main(void) {

    // i will use a boltzmann distribution to determine the crossover frequences: 
    // in a boltzman distribution, the number which are the smallest are the more likly to be produced 
    // and since my array is sorted in a desc way following the fitness score, the smallest indexes   
    // (elements which have the best fitness score) are the more likly to be chosen.

    int progressionCounter = 1;

    double const exp_dist_mean   = 7;
    double const exp_dist_lambda = 1 / exp_dist_mean;

    std::random_device rd; 

    std::exponential_distribution<> rng (exp_dist_lambda);
    std::mt19937 rnd_gen (rd ());

    std::array<Solution*, GENERATION_SIZE> *array = new(std::array<Solution*, GENERATION_SIZE>);
    
    Solution::generateSolutions(*array);
    
    for (int i = 0; i < 3; i++)
    {
        (*array)[i]->printSolution();
    }
    printf("\n");
    

    for (int i = 0; i < GENERATION_NUMBER; i++) {
        
        auto x = (int)(rng(rnd_gen) );
        auto y = (int)(rng(rnd_gen) );
        

        if (x >= GENERATION_SIZE)
        {
            x = GENERATION_SIZE -1;
        }
        else if (y >= GENERATION_SIZE)
        {
            y = GENERATION_SIZE -1;
        }
        
        Solution::solutionCrossOver(*array, *array , x, y);
        (*array)[x]->fitnessFunction();
        (*array)[y]->fitnessFunction();

        //sort desc the parent array according to the fitnessValue, to pick them 
        //according the boltzmann distribution 
        std::sort(array->begin(), array->end(),[ ](const auto& lhs, const auto& rhs ) {
            return (lhs->FitnessValue > rhs->FitnessValue);
        });
        
        //perform mutation  
        (*array)[x]->solutionMutate();
        (*array)[y]->solutionMutate();

        int progression = GENERATION_NUMBER / 10;
        
        if (i % progression == 0)
        {
            std::cout << progressionCounter << "/10 ..." << std::endl;
            progressionCounter++;
        }    
          
    }

    std::cout << "-------------------RESULTS-------------" << std::endl;
        for (int i = 0; i < 3; i++)
    {
        (*array)[i]->printSolution();
    }

    
    return 0;
};






