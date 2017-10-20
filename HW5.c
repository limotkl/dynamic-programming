#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

  int n;
  int flag = 0; 
  int maxprofit = 0;

typedef struct
{
	int profit;//10-30
	int weight;//5-20
	int mark;
  int upperBound;
}item; 
typedef struct
{
  float value;
  int index;
}node; 

float KWF2(int i, int weight, int profit, node temp[], int W,item it[])
{
        int j;
        float bound;
        float x[9];
        bound  = (float)profit;
        for(j=0;j< n;j++)
        {
          x[j] = 0 ;  //initialize   variables   to 0
        }

        while  (weight < W &&  i < n)                //not “full”and more  items
        {
          if (weight  + it[temp[i].index].weight  <=  W )
          {
              x[i]=1;                                  
              weight = weight  + it[temp[i].index].weight; 
              bound = bound + it[temp[i].index].profit;
          }                               
         else
         {
              x[i]=(float)(W - weight)/(float)it[temp[i].index].weight;    //fraction  of    i added  to knapsack
              weight  = W; 
              bound  =  bound + (float)it[temp[i].index].profit * x[i];//p[i]*x[i]
         }
          i=i+1;                                                        //  next  item
        }
                                                      //  next  item
       return  bound;
}

int promising(int i,int W,node temp[],int profit, int weight,item it[])
{
      //Cannot  get a solution  by  expanding node
      if (weight >=  W )
        return 0;
      //Compute upper bound
      float Bound;
      Bound= KWF2(i+1, weight, profit,temp,W,it);

      printf("The %d visited node, is profit: %d, weight: %d, bound: %f\n", flag, profit, weight,Bound);
      flag ++;

       if (Bound > (float)maxprofit) return 1;
       return 0;
}

void knapsack(int i, int profit, int weight,int W,node temp[], int include1[],int bestset[],item it[])
{
    if  ( weight  <=  W &&  profit  > maxprofit )
    {
              //  save  better  solution
        int j;
        maxprofit = profit ;//save  new profit
        //num = i; 
        for(j=0;j< n;j++)
          bestset[j]= include1[j];  //save  solution
    }

    if  (promising(i,W,temp,profit,weight,it))
    {
        // include1[i + 1]  = 1;//“yes”
        // knapsack(i+1, profit + it[i+1].profit, weight + it[i+1].weight,W,it,include1,bestset);//p[i+1], weight + w[i+1])
        // include1[i + 1]  = 0;//“no”
        // knapsack(i+1,profit,weight,W,it,include1,bestset);

        include1[i+1]  = 1;//“yes”
        knapsack(i+1, profit + it[temp[i+1].index].profit, weight + it[temp[i+1].index].weight,W,temp,include1,bestset,it);//p[i+1], weight + w[i+1])
        include1[i+1]  = 0;//“no”
        knapsack(i+1,profit,weight,W,temp,include1,bestset,it);
    }
        
}

//====================================
void sort(node a[],int q)
{
    int x,y;
    node temp;
    for(x=0;x<q-1;x++) {
        for(y=x+1;y<q;y++) {
            if(a[x].value < a[y].value) {
                temp=a[x];
                a[x]=a[y];
                a[y]=temp;
            }
        }
    }
}

void brute_force(int n,item it[],int W)
{
	int i,j,sum,temp,W_,P_,P,Sumw;
	P=0;
	int mark[9];
	sum = pow(2,n);
	//==================2^n-1 subset
	for(i=1;i<sum;i++)
	{
         //printf("\n2^n index%d\n",i);
         W_=0;
         P_=0;
        for(j=n-1;j>=0;j--)
        {
        	it[j].mark = 0;
        }
        for(j=n-1;j>=0;j--)
        {
       		//printf("%d",(i>>j)&1);
       		temp = (i>>j)&1;
       		if(temp == 1)
       		{
       			W_ += it[j].weight;
       			P_ += it[j].profit;
       			it[j].mark = 1;
           	}
        } 
        //==================updata the bigest profit and mark the selected item
        if(W_ <= W && P_ > P)
           {
           		for(j=0;j<n;j++)
           		{
           			mark[j]=0;
           		}
           		P = P_;
           		Sumw = W_;
           		for(j=0;j<n;j++)
           		{
           			if(it[j].mark==1)
           				mark[j]=1;
           		}
           }
    }
    //===================print the result
    printf("\n========brute force==========\n");
    printf("total profit:%d total weight:%d\n",P,Sumw);
    for(i=0;i<n;i++)
    {
    	if(mark[i]==1)
    	printf("item:%d ,profit:%d, weight:%d\n",i,it[i].profit,it[i].weight);
    }
}

void print_entry(int *B,int n,int W,item it[])
{
  if( n <= 1)
  {
    return ;
  }
    printf("(%d---%d) value :%d\n",n,W,B[n*(W+1) + W]);
    //printf("\nvalue :%d\n",B[n*(W+1) + W]);

    printf("need :");
    printf("(%d---%d)\t",n-1,W);

   if(W-it[n-1].weight > 0)
   {
      printf("need :");
      printf("(%d---%d)\n",n-1,(W-it[n-1].weight));
   }
   else
    printf("\n");

    print_entry(B,n-1,W,it);
    if(W-it[n-1].weight > 0)
      print_entry(B,n-1,W-it[n-1].weight,it);
  }


void refinement(int n,item it[],int W)
{
  int *B;
  int i,k,w,best;
  best = 0;
  B = (int *)malloc(sizeof(int)*((n+1)* (W+1)));//matrix B
  //mark = (int *)malloc(sizeof(int)*((n+1)* (W+1)));//matrix mark

  for(i=0;i<W+1;i++)
      B[0*(W+1)+i]=0;

  for(k=1;k<n+1;k++)
  {
    //printf("%d  %d  %d  ",k-1,it[k-1].profit,it[k-1].weight);
    B[k*(W+1)+0]=0;
    for(w=1;w<W+1;w++)
    {
      if(it[k-1].weight<= w && (B[(k-1)*(W+1) +(w-it[k-1].weight)] + it[k-1].profit > B[(k-1)*(W+1) + w]))
        {
          B[k*(W+1)+w]=B[(k-1)*(W+1) + (w-it[k-1].weight)] + it[k-1].profit;
          //printf("[%d][%d]=%2d  ",k,w,B[k*(W+1)+w]);
        }
      else
      {
        B[k*(W+1)+w]=B[(k-1)*(W+1) + w];
        //printf("[%d][%d]=%2d  ",k,w,B[k*(W+1)+w]);
      }
    }
    //printf("\n");
  }
  //printf("======%d\n",B[n*(W+1) + W]);
//print_entry(B,n,W,it);
//=================
      int temp,Sumw1;
      temp=W;  
      Sumw1 = 0;
      for(i=0;i<n;i++)
      {
          it[i].mark=0;
      }
      for(i=n;i>=1;i--)  
      {  
        if(B[i*(W+1)+temp] > B[(i-1)*(W+1)+temp] )  
            {  
                it[i-1].mark = 1; 
                Sumw1 += it[i-1].weight;
                temp=temp-it[i-1].weight;
            }  
      }

    printf("\n========refined dynamic programming=========\n");
    printf("total profit:%d total weight:%d\n",B[n*(W+1) + W],Sumw1);
    for(i=0;i<n;i++)
    {
      if(it[i].mark==1)
      printf("item:%d ,profit:%d, weight:%d\n",i,it[i].profit,it[i].weight);
    }
    //=================
    print_entry(B,n,W,it);
    free(B);

}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	int i,W;
	W = 0; 
  n = (rand()%5) + 4;//4-8
  //n=4;
	item it[9];
	printf("all together %d item\n", n);
	//======================initialization
	for(i=0;i<n;i++)
	{
		it[i].profit = (rand()%21) + 10;
		it[i].weight = (rand()%16) + 5;
		it[i].mark = 0;
		printf("item %d,profit:%d,weight:%d\n",i,it[i].profit,it[i].weight);
		W = W + it[i].weight;
	}
	W =(int)floor(W * 0.6);
	printf("W=%d\n",W);

	brute_force(n,it,W);
  refinement(n,it,W);
  //===================================
  int include1[9]={0};
  int bestset[9] ={0};
  node temp[9];
  for(i=0;i<n;i++)
  {
    temp[i].value=(float)it[i].profit/(float)it[i].weight;
    temp[i].index = i;
  }
  sort(temp,n);
  // for(i=0;i<n;i++)
  // {
  //   printf("value%f\n",temp[i].value);
  //   printf("index%d\n",temp[i].index);
  //   printf("profit%d\n",it[temp[i].index].profit);
  //   printf("weight%d\n",it[temp[i].index].weight);
  //   printf("=================\n");
  // }
  printf("\n========back tracking=========\n");
  knapsack(-1,0,0,W,temp,include1,bestset,it);
  //printf("%d\n",maxprofit);
  int Sumw2;
  Sumw2=0;
     for (i = 0 ;  i < n;  i++)
   {
      if(bestset[i]==1)
      Sumw2 += it[temp[i].index].weight;
   }
   printf("\ntotal profit:%d total weight:%d\n",maxprofit,Sumw2);
   for (i = 0 ;  i < n;  i++)
   {
      if(bestset[i]==1)
     printf("item:%d ,profit:%d, weight:%d\n",temp[i].index,it[temp[i].index].profit,it[temp[i].index].weight);
   }

	return 0;
}
