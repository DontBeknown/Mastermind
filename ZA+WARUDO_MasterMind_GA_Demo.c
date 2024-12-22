#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ColorsNum 8
#define CodeLength 5

#define MaxPopSize 60
#define MaxGen 100

#define CrossOverChance 100

#define SwapMutateChance 25
#define ResetMutateChance 75
#define InverseMutateChance 25

//ColorsNum^CodeLength
//512 if 8 3
//1296 if 6 4
#define AllPosibleCase 512

#define MutateMethod 3
#define AllPosibleMethod 8

#define TournamentselectionSize 5

typedef struct {
    int code[CodeLength];
    int fitness ;
    int black,white ;
}CODE;

void randNum(int A[]) {
    for (int i = 0; i < CodeLength; i++) {
        A[i] = (rand() % ColorsNum)+1;
    }
}

int whitePeg,blackPeg;
int foundedPosition[CodeLength];
void checkCode(int guess[],int code[]){

    whitePeg=0,blackPeg=0;
    memset(foundedPosition, 0, (CodeLength) * sizeof(int));

    for(int i=0;i<CodeLength;i++){
        for(int j=0;j<CodeLength;j++){
            if(guess[i]==code[j] && foundedPosition[j]!=1 ){
                foundedPosition[j]=1;
                whitePeg++;
                break;
            }
        }
    }

    for(int i=0;i<CodeLength;i++){
        if(guess[i]==code[i]){
            blackPeg++;
        }
    }
    whitePeg-=blackPeg;

}

void swap(int *a,int *b){
    int k =*a;
    *a=*b;
    *b=k;
}

void copyArr(int a[],int b[]){
    for(int i=0;i<CodeLength;i++){
        a[i]=b[i];
    }
}

// Random  resetting
int resetChannce = ResetMutateChance;
void randMutate(int code[]){
    int chance = rand() % 100;
    if(chance < resetChannce){
        int randPoint = rand() % CodeLength;
        int randNumber = (rand() % ColorsNum)+1;
        code[randPoint] = randNumber;
    }

}

int CrossMethod=1;
int crossChance = CrossOverChance;
void crossOver(int mommy[] ,int daddy[], int son[]){

    switch(CrossMethod){
        case 1:{
            //Single crossover
            int chance = rand() % 100;
            if(chance < crossChance){
                int point = rand() % CodeLength;
                for(int i=0;i<CodeLength;i++){
                    if(i < point){
                        son[i]=mommy[i];
                    }
                    else{
                        son[i]=daddy[i];
                    }
                }
            }
        }
        break;

        case 2:{
            //Two point crossover
            int chance = rand() % 100;
            if(chance < crossChance){
                int point1 = rand() % CodeLength;
                int point2 = rand() % CodeLength;
                if(point1 > point2){
                    swap(&point1,&point2);
                }
                for(int i=0;i<CodeLength;i++){
                    if(i < point1){
                        son[i]=mommy[i];
                    }
                    else if (i >= point1 && i < point2){
                        son[i]=daddy[i];
                    }
                    else{
                        son[i]=mommy[i];
                    }
                }
            }
        }
        break;

        case 3:{
            //Uniform crossover
            for(int i=0;i<CodeLength;i++){
                int chance = rand() % 100;
                if(chance < crossChance){
                    son[i]=mommy[i];
                }
                else{
                    son[i]=daddy[i];
                }
            }
        }
        break;

    }
}

int swapChannce = SwapMutateChance;
void swapMutate(int code[]){
    for(int i=0;i<CodeLength;i++){
        int chance = rand() % 100;
        if(chance < swapChannce){
            int x = rand() % CodeLength ,y = rand() % CodeLength;
            swap(&code[x],&code[y]);
        }
    }

}

int inverseChannce = InverseMutateChance;
void inversionMutate(int code[]) {
    int chance = rand() % 100;
    if(chance < inverseChannce){
        int start = rand() % CodeLength;
        int end = rand() % CodeLength;

        if (start > end) {
            swap(&start,&end);
        }

        while (start < end) {
            swap(&code[start], &code[end]);
            start++;
            end--;
        }
    }
}

void printArr(int arr[],int n){
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
}

void fitnessCal(CODE pop[],CODE trail[],int trailCnt){

    for(int i=0;i<MaxPopSize;i++){
        pop[i].fitness=0;
        for(int j=0;j<trailCnt;j++){
            checkCode(pop[i].code,trail[j].code);
            pop[i].fitness += abs(trail[j].black-blackPeg)+abs(trail[j].white-whitePeg);
        }
    }

}

int isDupe(int code[],CODE ref[],int cntRef){
    for(int i=0;i<cntRef;i++){
        int cnt=0;
        for(int j=0;j<CodeLength;j++){
            if(code[j] == ref[i].code[j])
                cnt++;
        }
        if(cnt==CodeLength){
            return i;
        }

    }
    return -1;
}

int selMutate[MutateMethod] = {1,1,1};
void genNewPop(CODE population[]) {
    for (int i = 0; i < MaxPopSize; i += 2) {

        crossOver(population[i].code, population[i + 1].code, population[i].code);
        crossOver(population[i + 1].code, population[i].code, population[i + 1].code);

        if(selMutate[0] == 1){
            randMutate(population[i].code);
            randMutate(population[i+1].code);
        }
        else if(selMutate[1] == 1){
            swapMutate(population[i].code);
            swapMutate(population[i + 1].code);
        }
        else if(selMutate[2] == 1){
            inversionMutate(population[i].code);
            inversionMutate(population[i + 1].code);
        }

        population[i].fitness = 0;
        population[i + 1].fitness = 0;
    }
}

int selPrimePop(CODE pop[],CODE prime[], CODE Eligibles[],int eCnt){
    int primeCnt=0;
    for(int i=0;i<MaxPopSize;i++){
        if(pop[i].fitness == 0){
            prime[primeCnt]=pop[i];
            primeCnt++;
        }
    }

    for(int i=0;i<primeCnt;i++){
        int dupeIndex = isDupe(prime[i].code,Eligibles,eCnt);
        if(dupeIndex != -1){
            randNum(Eligibles[dupeIndex].code);
        }
    }
    return primeCnt;
}

int addEligible(CODE Eligibles[],CODE pop[],int eCnt){
    for(int i=0;i<MaxPopSize;i++){
        if(eCnt == MaxPopSize)
            break;
        if(pop[i].fitness == 0 && isDupe(pop[i].code,Eligibles,eCnt) == -1){
            Eligibles[eCnt]=pop[i];
            eCnt++;
        }
    }
    return eCnt;
}

int tourSize = TournamentselectionSize;
void popSelectTournament(CODE pop[]){

    int cntSel=0,mark[MaxPopSize];
    CODE selected[MaxPopSize];

    while(cntSel < MaxPopSize){
        CODE competitor[tourSize];

        for(int i=0;i<tourSize;i++){
            int randSel = rand() % MaxPopSize;
            competitor[i] = pop[randSel];
        }
        CODE winner = competitor[0];

        for(int i=1;i<tourSize;i++){
            if(competitor[i].fitness < winner.fitness){
                winner = competitor[i];
            }
        }

        selected[cntSel++] = winner;
    }

    for(int i=0;i<MaxPopSize;i++){
        copyArr(pop[i].code,selected[i].code);
        pop[i].fitness=selected[i].fitness;
    }
}

void popSelectRanking(CODE pop[]) {

    int rank[MaxPopSize];
    int totalRank = 0;

    for (int i = 0; i < MaxPopSize; i++) {
        int count = 0;
        for (int j = 0; j < MaxPopSize; j++) {
            if (pop[j].fitness < pop[i].fitness) {
                count++;
            }
        }
        rank[i] = count + 1;
        totalRank += rank[i];
    }

    double probabilities[MaxPopSize];
    for (int i = 0; i < MaxPopSize; i++) {
        probabilities[i] = (double)rank[i] / totalRank;
    }

    CODE selected[MaxPopSize];
    int cntSel = 0;

    while (cntSel < MaxPopSize) {

        double r = ((double)rand() / RAND_MAX);
        double cumulativeProbability = 0.0;

        for (int i = 0; i < MaxPopSize; i++) {
            cumulativeProbability += probabilities[i];
            if (r <= cumulativeProbability) {
                selected[cntSel++] = pop[i];
                break;
            }
        }
    }

    for (int i = 0; i < MaxPopSize; i++) {
        copyArr(pop[i].code, selected[i].code);
        pop[i].fitness = selected[i].fitness;
    }
}

void popSelectRouletteWheel(CODE pop[]) {

    double totalFitness = 0.0;
    for (int i = 0; i < MaxPopSize; i++) {
        totalFitness += pop[i].fitness;
    }

    double probabilities[MaxPopSize];
    if (totalFitness == 0.0) {
        for (int i = 0; i < MaxPopSize; i++) {
            probabilities[i] = 1.0 / MaxPopSize;
        }
    } else {
        for (int i = 0; i < MaxPopSize; i++) {
            probabilities[i] = 1.0 - (pop[i].fitness / totalFitness);
        }
    }

    CODE selected[MaxPopSize];
    int cntSel = 0;

    while (cntSel < MaxPopSize) {
        double r = ((double)rand() / RAND_MAX);
        double cumulativeProbability = 0.0;

        for (int i = 0; i < MaxPopSize; i++) {
            cumulativeProbability += probabilities[i];
            if (r <= cumulativeProbability) {
                selected[cntSel++] = pop[i];
                break;
            }
        }
    }

    for (int i = 0; i < MaxPopSize; i++) {
        copyArr(pop[i].code, selected[i].code);
        pop[i].fitness = selected[i].fitness;
    }
}

void reNewPop(CODE primePop[],CODE pop[],int primeCnt){

    for(int i=0;i<primeCnt;i++){
        pop[i]=primePop[i];
    }

    for(int i=primeCnt-1;i<MaxPopSize;i++){
        randNum(pop[i].code);
    }

}

int selMethod = 3;
int generaticAlgo(CODE Eligibles[],CODE trail[],int trailCnt,int mode){

    CODE population[MaxPopSize];

    for(int i=0;i<MaxPopSize;i++){
        randNum(population[i].code);
    }

    int genCnt = 1,eCnt = 0,chEmpty = 0;
    while(genCnt <= MaxGen/mode && eCnt < MaxPopSize*mode){

        genNewPop(population);

        fitnessCal(population,trail,trailCnt);

        CODE primePopulation[MaxPopSize];
        int primeCnt=0;

        primeCnt = selPrimePop(population,primePopulation,Eligibles,eCnt);

        if(primeCnt == 0){
            genCnt++;
            continue;
        }

        eCnt = addEligible(Eligibles,population,eCnt);

        switch(selMethod){
            case 1: reNewPop(primePopulation,population,primeCnt);
            break;
            case 2: popSelectRanking(population);
            break;
            case 3: popSelectRouletteWheel(population);
            break;
            case 4: popSelectTournament(population);
            break;
        }

        genCnt++;
    }

    return eCnt;
}


int allCase[AllPosibleCase][CodeLength],per[CodeLength],caseNum;
void genAllCase(int now){
    if(now == CodeLength){
        for(int i=0;i<CodeLength;i++){
            allCase[caseNum][i]=per[i];
        }
        caseNum++;
        return;
    }

    for(int i=1;i<=ColorsNum;i++){
        per[now] = i;
        genAllCase(now+1);
    }
}

int selCase[AllPosibleMethod][MutateMethod],per2[MutateMethod],caseNum2;
void genSelection(int now){
    if(now == MutateMethod){
        for(int i=0;i<MutateMethod;i++){
            selCase[caseNum2][i]=per2[i];
        }
        caseNum2++;
        return;
    }

    for(int i=0;i<=1;i++){
        per2[now] = i;
        genSelection(now+1);
    }
}

int main(){

    srand(time(NULL));
    int mxGuess=-1,mnGuess=1e9;
    double cnt=0,cntAvg=0;;
    int k = 1;

    printf("How many times? : ");
    scanf("%d",&k);
    int dd=k;
    cnt=0;
    mxGuess=-1,mnGuess=1e9;
    while(k--){

            int code[CodeLength];
            CODE guessTrial[MaxPopSize];
            int guess[CodeLength]={1,1,2,3},guessCnt=0;

            randNum(code);
            //copyArr(code,allCase[i]);

            printf("Code : ");
            printArr(code,CodeLength);
            printf("\n");

            while(1){
                CODE E[MaxPopSize];
                int lastCode=0;

                checkCode(guess,code);

                printf("#%d Guess : ",guessCnt+1);
                printArr(guess,CodeLength);
                printf("\nBlack: %d White: %d\n\n",blackPeg,whitePeg);

                if(blackPeg == CodeLength){
                    printf("Found it!!! in %d times\n",guessCnt+1);
                    printf("------------------------------\n",guessCnt+1);
                    cntAvg+=guessCnt+1;
                    if(mxGuess < guessCnt+1) mxGuess = guessCnt+1;
                    if(mnGuess > guessCnt+1) mnGuess = guessCnt+1;

                    break;
                }

                copyArr(guessTrial[guessCnt].code,guess);
                guessTrial[guessCnt].black = blackPeg;
                guessTrial[guessCnt].white = whitePeg;

                guessCnt++;

                lastCode = generaticAlgo(E,guessTrial,guessCnt,1);

                while(lastCode == 0){
                    lastCode = generaticAlgo(E,guessTrial,guessCnt,2);
                }
                lastCode--;

                int dupeIdx = isDupe(E[lastCode-1].code,guessTrial,guessCnt);

                while(dupeIdx != -1 && lastCode != 0){
                    lastCode--;
                    dupeIdx = isDupe(E[lastCode].code,guessTrial,guessCnt);
                }

                copyArr(guess,E[lastCode].code);
            }

        //printf("\t Avg : %.4lf \t",cnt/AllPosibleCase);

    }
    printf("Avg %d : %.4lf",dd,cntAvg/dd);
    printf("\t Min :%d\t Max :%d\n",mnGuess,mxGuess);
    return 0;
}
