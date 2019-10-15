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
}
bool entrou = false;
unsigned long Start, End;
int i=1,j=1,k=1;
long long int c = 0, c1;
double A = 1, B = 1, C = 1;
double uTime[20], uMeanTime, standardDeviation, HV5, LV5,HV1,LV1,overheadMeanTime;
double studentT195 = 2.0930; // gl = 19, alpha = 5 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.
double studentT191 = 2.5395;// gl = 19, alpha = 1 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.

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
void overheadBench()
{
	SerialUSB.println("<Overhead>");
	uMeanTime = 0;
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			asm("nop");
		}
		End = micros();
		uMeanTime += ((double)(End - Start)) / N_BATCH_ITERATIONS;
		uTime[c] = (((double)(End - Start))) / N_BATCH_ITERATIONS;
	}
	overheadMeanTime = uMeanTime / N_ITERATIONS;
	overheadMeanTime*=1000;
	benchStatistics();
}
void divisionBench()
{
	SerialUSB.println("<Division>");
	uMeanTime = 0;
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B / C;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B *C;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B + C;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			B - C;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			i%j;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			i&j;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			i|j;
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			exp(A);
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			log(A);
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			sin(A);
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			cos(A);
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
	for (c = 0; c < N_ITERATIONS; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < N_BATCH_ITERATIONS; c1++)
		{
			tan(A);
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
