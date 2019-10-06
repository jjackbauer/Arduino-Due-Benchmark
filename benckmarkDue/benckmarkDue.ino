/*
                                          --Arduino Benchmark --
                                          By: Ricardo Medeiros 
                                          Github:jjackbauer 
*/

void setup()
{
  SerialUSB.begin(1843200);
}
bool entrou=false;
unsigned long Start,End;
long long int c=0,c1;
long long int n=20,n1=10000000;
double A=0,B=1,C=1;
double uTime[20],uMeanTime,sd,HV,LV,overheadMeanTime;
double studentT19=2.0930;// Retirado do livro Modelagem e Simulação Discreta do professor Brauliro.


void benchStatistics()
{
    for(c=0;c<n;c++)
    {
      uTime[c]-=overheadMeanTime;
      uTime[c] = fabs(uTime[c]);
    }

    uMeanTime/=(n);
    uMeanTime-=overheadMeanTime;
    uMeanTime = fabs(uMeanTime);
    
    sd=0;
    for(c=0;c<n;c++)
    {
      SerialUSB.print("Iteration ");
      SerialUSB.print((int)c);
      SerialUSB.print(" : ");
      SerialUSB.print(uTime[c],16);
      SerialUSB.println(" us");
      sd+=pow((uTime[c]-uMeanTime),2);
    }
    sd/=(n-1);
    sd=sqrt(sd);
    HV = uMeanTime +studentT19*sd/sqrt(n);
    LV = uMeanTime -studentT19*sd/sqrt(n);
    SerialUSB.print("Mean:");
    SerialUSB.print(uMeanTime,8);
    SerialUSB.println(" us");
    SerialUSB.print("Standard Deviation:");
    SerialUSB.print(sd,8);
    SerialUSB.println(" us^2");
    SerialUSB.print("Confiance Interval: [");
    SerialUSB.print(LV,8);
    SerialUSB.print(" , ");
    SerialUSB.print(HV,8);
    SerialUSB.println("] us");

}
void overheadBench()
{
  SerialUSB.println("<Overhead>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
      {
        asm("nop");
      }
      End=micros();
      uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }  
    overheadMeanTime = uMeanTime/n;
    benchStatistics();
}
void divisionBench()
{
  SerialUSB.println("<Division>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
        {
          B/C;
        }
      End=micros();
      uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }
    benchStatistics();  
    
}
void multiplicationBench()
{
  SerialUSB.println("<Multiplication>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
        {
          B*C;
        }
      End=micros();
       uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }  
    benchStatistics();
}
void sumBench()
{
  SerialUSB.println("<Sum>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
        {
          B+C;
        }
      End=micros();
       uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }  
    benchStatistics();
}
void subBench()
{
  SerialUSB.println("<Subtraction>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
        {
          B-C;
        }
      End=micros();
       uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }  
    benchStatistics();
}
void atributionBench()
{
  SerialUSB.println("<Atribuition>");
  uMeanTime=0;
   for(c=0;c<n;c++)
    {
      Start=micros();
      for(c1=0;c1<n1;c1++)
        {
          A = B;
        }
      End=micros();
      uMeanTime+=((double)(End-Start))/n1;
      uTime[c]=(((double)(End-Start)))/n1;
    }  
    benchStatistics();
}


void greeting()
{
    SerialUSB.println("<Ricardo Medeiros's Benchmark for Arduino Due>");
    SerialUSB.println("<send 's' to start Benchmark>");
    
}
void loop() {

  
    if(SerialUSB && !entrou)
    {
      greeting();
      entrou=true;
    }
    
    char msg=SerialUSB.read();
    
    if(msg== 's')
    {
      SerialUSB.println("STARTING");
      overheadBench();
      sumBench();
      subBench();
      multiplicationBench();
      divisionBench();
      atributionBench();
      SerialUSB.println("ENDED");
    }
}
