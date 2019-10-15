/*
			--Arduino Benchmark --
			By: Ricardo Medeiros 
			Github: jjackbauer 
*/
#include <math.h>

#define N_ITERATIONS 20			 // has to be 20 for the used T student
#define N_BATCH_ITERATIONS 10000 // the bigger the better, but it will take more time to finish

void setup()
{
	SerialUSB.begin(115200);
	randomSeed(analogRead(4));
}

bool entrou = false;
unsigned long Start, End;
int i=1,j=1,k=1;
long long int c = 0, c1;
double A = 1, B = 1, C = 1;
double uTime[20], uMeanTime, standardDeviation, HV5, LV5,HV1,LV1,overheadMeanTime;
double studentT195 = 2.0930; // gl = 19, alpha = 5 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.
double studentT191 = 2.5395;// gl = 19, alpha = 1 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.

void randomizeVariables()// By @ruanmed
{
	A = (double)random(1000);
	B = (double)random(1000);
	C = 2*PI*(double)random(1000)/1000;
	i = random(1000);
	j = random(1000);
	k = random(1000);
}
void benchStatistics()
{
	for (c = 0; c < N_ITERATIONS; c++)
	{
		uTime[c]*=1000;
		uTime[c] -= overheadMeanTime;
		uTime[c] = fabs(uTime[c]);
	}

	uMeanTime /= (N_ITERATIONS);
	uMeanTime*=1000;
	uMeanTime -= overheadMeanTime;
	uMeanTime = fabs(uMeanTime);

	standardDeviation = 0;
	for (c = 0; c < N_ITERATIONS; c++)
	{
		SerialUSB.print("Iteration ");
		SerialUSB.print((int)c);
		SerialUSB.print(" : ");
		SerialUSB.print(uTime[c], 16);
		SerialUSB.println(" ns");
		standardDeviation += pow((uTime[c] - uMeanTime), 2);
	}
	standardDeviation /= (N_ITERATIONS - 1);
	standardDeviation = sqrt(standardDeviation);
	HV5 = uMeanTime + studentT195 * standardDeviation / sqrt(N_ITERATIONS);
	LV5 = uMeanTime - studentT195 * standardDeviation / sqrt(N_ITERATIONS);
  HV1 = uMeanTime + studentT191 * standardDeviation / sqrt(N_ITERATIONS);
	LV1 = uMeanTime - studentT191 * standardDeviation / sqrt(N_ITERATIONS);
	SerialUSB.print("Mean:");
	SerialUSB.print(uMeanTime, 8);
	SerialUSB.println(" ns");
	SerialUSB.print("Standard Deviation:");
	SerialUSB.print(standardDeviation, 8);
	SerialUSB.println(" ns^2");
	SerialUSB.print("Confiance Interval (Alpha 5%): [");
	SerialUSB.print(LV5, 8);
	SerialUSB.print(" , ");
	SerialUSB.print(HV5, 8);
	SerialUSB.println("] ns");
  SerialUSB.print("Confiance Interval (Alpha 1%): [");
	SerialUSB.print(LV1, 8);
	SerialUSB.print(" , ");
	SerialUSB.print(HV1, 8);
	SerialUSB.println("] ns");
}
void overheadbenchStatistics()
{
  for (c = 0; c < N_ITERATIONS; c++)
  {
    uTime[c]*=1000;
    uTime[c] -= overheadMeanTime;
    uTime[c] = uTime[c];
  }

  uMeanTime /= (N_ITERATIONS);
  uMeanTime*=1000;
  uMeanTime -= overheadMeanTime;
  uMeanTime = uMeanTime;

  standardDeviation = 0;
  for (c = 0; c < N_ITERATIONS; c++)
  {
    SerialUSB.print("Iteration ");
    SerialUSB.print((int)c);
    SerialUSB.print(" : ");
    SerialUSB.print(uTime[c], 16);
    SerialUSB.println(" ns");
    standardDeviation += pow((uTime[c] - uMeanTime), 2);
  }
  standardDeviation /= (N_ITERATIONS - 1);
  standardDeviation = sqrt(standardDeviation);
  HV5 = uMeanTime + studentT195 * standardDeviation / sqrt(N_ITERATIONS);
  LV5 = uMeanTime - studentT195 * standardDeviation / sqrt(N_ITERATIONS);
  HV1 = uMeanTime + studentT191 * standardDeviation / sqrt(N_ITERATIONS);
  LV1 = uMeanTime - studentT191 * standardDeviation / sqrt(N_ITERATIONS);
  SerialUSB.print("Mean:");
  SerialUSB.print(uMeanTime, 8);
  SerialUSB.println(" ns");
  SerialUSB.print("Standard Deviation:");
  SerialUSB.print(standardDeviation, 8);
  SerialUSB.println(" ns^2");
  SerialUSB.print("Confiance Interval (Alpha 5%): [");
  SerialUSB.print(LV5, 8);
  SerialUSB.print(" , ");
  SerialUSB.print(HV5, 8);
  SerialUSB.println("] ns");
  if(LV5*HV5 < 0)
    SerialUSB.println("Constant overhead hypotesis suported with Alpha 5% in Zero Mean Test");
  SerialUSB.print("Confiance Interval (Alpha 1%): [");
  SerialUSB.print(LV1, 8);
  SerialUSB.print(" , ");
  SerialUSB.print(HV1, 8);
  SerialUSB.println("] ns");
    if(LV1*HV1 < 0)
    SerialUSB.println("Constant overhead hypotesis suported with Alpha 1% in Zero Mean Test");
}
void overheadBench()
{
	SerialUSB.println("<Overhead>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B;
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	overheadMeanTime = uMeanTime / N_ITERATIONS;
	overheadMeanTime*=1000;
	overheadbenchStatistics();
}
void divisionBench()
{
	SerialUSB.println("<Division>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B / C;
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void multiplicationBench()
{
	SerialUSB.println("<Multiplication>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B *C;
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void sumBench()
{
	SerialUSB.println("<Sum>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B + C;
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void subBench()
{
	SerialUSB.println("<Subtraction>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B - C;
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void atributionBench()
{
	SerialUSB.println("<Atribuition>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{	
			i=j;
			B = C;
			A = B;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void remainderBench()
{
	SerialUSB.println("<Remainder>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B;
			i= j%k;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void andBench()
{
	SerialUSB.println("<Logic AND>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B;
			i = j&k;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void orBench()
{
	SerialUSB.println("<Logic OR>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			A = B;
			i = j|k;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}

void expBench()
{
	SerialUSB.println("<Exponential>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B = exp(A);
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void logBench()
{
	SerialUSB.println("<Natural Logarithm>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B = log(A);
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void sinBench()
{
	SerialUSB.println("<Sine>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B = sin(C);
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void cosBench()
{
	SerialUSB.println("<Cossine>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B = cos(C);
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}
void tanBench()
{
	SerialUSB.println("<Tangent>");
	uMeanTime = 0;
	randomizeVariables();
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B = tan(C);
			i = j;
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	benchStatistics();
}


void greeting()
{
	SerialUSB.println("<Ricardo Medeiros's Benchmark for Arduino Due>");
	SerialUSB.println("<send 's' to start Benchmark>");
}
void loop()
{

	if (SerialUSB && !entrou)
	{
		greeting();
		entrou = true;
	}

	char msg = SerialUSB.read();

	if (msg == 's')
	{
		SerialUSB.println("STARTING");
		overheadBench();
		sumBench();
		subBench();
		multiplicationBench();
		divisionBench();
		atributionBench();
    	remainderBench();
    	andBench();
    	orBench();
    	expBench();
		logBench();
		sinBench();
		cosBench();
		tanBench();
		SerialUSB.println("ENDED");
	}
}
