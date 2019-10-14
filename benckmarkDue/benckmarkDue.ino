/*
			--Arduino Benchmark --
			By: Ricardo Medeiros 
			Github: jjackbauer 
*/
#include <math.h>
#include <SomeSerial.h>

#define N_BATCHES 20 // has to be 20 for the used T student
#define DEFAULT_BATCH_ITERATIONS 1000
#define FULL_STATISTICS 1 // use full statistics function

#if defined(ARDUINO_ARCH_SAM)
SomeSerial LogSerial = SomeSerial(&SerialUSB);
#else
SomeSerial LogSerial = SomeSerial(&Serial);
#endif

void setup()
{
	LogSerial.begin(115200);
	// Se o pino de entrada analógica 4 é deixado desconectado,
	// o ruído aleatório analógico irá causar a chamada de randomSeed()
	// gerar sementes aleatórias diferentes cada vez que o sketch roda.
	// randomSeed() basicamente "embaralha" a função random().
	randomSeed(analogRead(4));
}
bool entrou = false;
unsigned long Start, End;
int i = 1, j = 1, k = 1;
int numberBatchIterations = DEFAULT_BATCH_ITERATIONS; // the bigger the better, but it will take more time to finish
long long int c = 0, c1;
double A = 1, B = 1, C = 1;
double uTime[20], uMeanTime, standardDeviation, HV5, LV5, HV1, LV1, overheadMeanTime, uMeanTimeMinusOverhead;
double studentT195 = 2.0930; // gl = 19, alpha = 5 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.
double studentT191 = 2.5395; // gl = 19, alpha = 1 Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.

void randomizeVariables()
{
	A = (double)random(1000) / (double)random(1000);
	B = (double)random(1000) / (double)random(1000);
	C = (double)random(1000) / (double)random(1000);
	i = random(1000);
	j = random(1000);
	k = random(1000);
}
void statistics()
{
	if (FULL_STATISTICS == 1)
		benchStatisticsAlternative();
	else
		benchStatistics();
}
void benchStatisticsAlternative()
{
	uMeanTime /= (N_BATCHES);
	uMeanTime *= 1000;
	uMeanTimeMinusOverhead = uMeanTime - overheadMeanTime;
	uMeanTimeMinusOverhead = fabs(uMeanTimeMinusOverhead);

	standardDeviation = 0;
	LogSerial.println("BATCH_NUMBER: \t TIME_PER_OPERATION \t TIME_MINUS_OVERHEAD");
	for (c = 0; c < N_BATCHES; c++)
	{
		uTime[c] *= 1000;
		LogSerial.print("Iteration ");
		LogSerial.print((int)c);
		LogSerial.print(": \t");
		LogSerial.print(uTime[c], 8);
		LogSerial.print(" ns \t");

		uTime[c] -= overheadMeanTime;

		LogSerial.print(uTime[c], 8);
		LogSerial.println(" ns");

		uTime[c] = fabs(uTime[c]);
		standardDeviation += pow((uTime[c] - uMeanTimeMinusOverhead), 2);
	}
	standardDeviation /= (N_BATCHES - 1);
	standardDeviation = sqrt(standardDeviation);
	HV5 = uMeanTimeMinusOverhead + studentT195 * standardDeviation / sqrt(N_BATCHES);
	LV5 = uMeanTimeMinusOverhead - studentT195 * standardDeviation / sqrt(N_BATCHES);
	HV1 = uMeanTimeMinusOverhead + studentT191 * standardDeviation / sqrt(N_BATCHES);
	LV1 = uMeanTimeMinusOverhead - studentT191 * standardDeviation / sqrt(N_BATCHES);
	LogSerial.print("Mean: ");
	LogSerial.print(uMeanTime, 8);
	LogSerial.println(" ns");
	LogSerial.print("Overhead Mean: ");
	LogSerial.print(overheadMeanTime, 8);
	LogSerial.println(" ns");
	LogSerial.print("Mean Minus Overhead: ");
	LogSerial.print(uMeanTimeMinusOverhead, 8);
	LogSerial.println(" ns");
	LogSerial.print("Standard Deviation: ");
	LogSerial.print(standardDeviation, 8);
	LogSerial.println(" ns^2");
	LogSerial.print("Confidence Interval (Alpha 5%): [");
	LogSerial.print(LV5, 8);
	LogSerial.print(" , ");
	LogSerial.print(HV5, 8);
	LogSerial.println("] ns");
	LogSerial.print("Confidence Interval (Alpha 1%): [");
	LogSerial.print(LV1, 8);
	LogSerial.print(" , ");
	LogSerial.print(HV1, 8);
	LogSerial.println("] ns");
}
void benchStatistics()
{
	for (c = 0; c < N_BATCHES; c++)
	{
		uTime[c] *= 1000;
		uTime[c] -= overheadMeanTime;
		uTime[c] = fabs(uTime[c]);
	}

	uMeanTime /= (N_BATCHES);
	uMeanTime *= 1000;
	uMeanTime -= overheadMeanTime;
	uMeanTime = fabs(uMeanTime);

	standardDeviation = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		LogSerial.print("Iteration ");
		LogSerial.print((int)c);
		LogSerial.print(": ");
		LogSerial.print(uTime[c], 8);
		LogSerial.println(" ns");
		standardDeviation += pow((uTime[c] - uMeanTime), 2);
	}
	standardDeviation /= (N_BATCHES - 1);
	standardDeviation = sqrt(standardDeviation);
	HV5 = uMeanTime + studentT195 * standardDeviation / sqrt(N_BATCHES);
	LV5 = uMeanTime - studentT195 * standardDeviation / sqrt(N_BATCHES);
	HV1 = uMeanTime + studentT191 * standardDeviation / sqrt(N_BATCHES);
	LV1 = uMeanTime - studentT191 * standardDeviation / sqrt(N_BATCHES);
	LogSerial.print("Mean: ");
	LogSerial.print(uMeanTime, 8);
	LogSerial.println(" ns");
	LogSerial.print("Standard Deviation: ");
	LogSerial.print(standardDeviation, 8);
	LogSerial.println(" ns^2");
	LogSerial.print("Confidence Interval (Alpha 5%): [");
	LogSerial.print(LV5, 8);
	LogSerial.print(" , ");
	LogSerial.print(HV5, 8);
	LogSerial.println("] ns");
	LogSerial.print("Confidence Interval (Alpha 1%): [");
	LogSerial.print(LV1, 8);
	LogSerial.print(" , ");
	LogSerial.print(HV1, 8);
	LogSerial.println("] ns");
}
void overheadBench()
{
	LogSerial.println("<Overhead>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{

		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B;
			i = k;
			// asm("nop");
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	overheadMeanTime = uMeanTime / N_BATCHES;
	overheadMeanTime *= 1000;
	statistics();
}
void divisionBench()
{
	LogSerial.println("<Division>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B / C;
			i = k;
			// B / C;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void multiplicationBench()
{
	LogSerial.println("<Multiplication>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B * C;
			i = k;
			// B * C;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void sumBench()
{
	LogSerial.println("<Sum>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B + C;
			i = k;
			// B + C;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void subBench()
{
	LogSerial.println("<Subtraction>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B - C;
			i = k;
			// B - C;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void atributionBench()
{
	LogSerial.println("<Atribution>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			B = C;
			i = k;
			// A = C;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void remainderBench()
{
	LogSerial.println("<Remainder>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B;
			k = i % j;
			// i % j;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void andBench()
{
	LogSerial.println("<Logic AND>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B;
			k = i & j;
			// i & j;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void orBench()
{
	LogSerial.println("<Logic OR>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = B;
			k = i | j;
			// i | j;
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}

void expBench()
{
	LogSerial.println("<Exponential>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = exp(B);
			i = k;
			// exp(B);
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void logBench()
{
	LogSerial.println("<Natural Logarithm>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			A = log(B);
			i = k;
			// log(A);
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void sinBench()
{
	LogSerial.println("<Sine>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			B = sin(A);
			i = k;
			// sin(A);
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void cosBench()
{
	LogSerial.println("<Cossine>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			B = cos(A);
			i = k;
			// cos(A);
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void tanBench()
{
	LogSerial.println("<Tangent>");
	uMeanTime = 0;
	for (c = 0; c < N_BATCHES; c++)
	{
		Start = micros();
		for (c1 = 0; c1 < numberBatchIterations; c1++)
		{
			randomizeVariables();
			B = tan(A);
			i = k;
			// tan(A);
		}
		End = micros();
		uTime[c] = (((double)(End - Start))) / numberBatchIterations;
		uMeanTime += uTime[c];
	}
	statistics();
}
void line()
{
	LogSerial.println("________________________________________________");
}
void greeting()
{
	LogSerial.println("<Ricardo Medeiros's Benchmark for Arduino Due>");
	LogSerial.println("<send 's' to start Benchmark>");
}
void loop()
{
#if defined(ARDUINO_ARCH_SAM)
	if (SerialUSB && !entrou) // Detects when Arduino Due SerialUSB is connected
#else
	if (&LogSerial && !entrou)
#endif
	{
		greeting();
		entrou = true;
	}

	char msg = LogSerial.read();

	if (entrou)
	{
		String inString = "";
		LogSerial.println("Enter the number of iterations in a batch: ");
		while (!LogSerial.available())
			;
		while (LogSerial.available() > 0)
		{
			int inChar = LogSerial.read();
			// convert the incoming byte to a char and add it to the string:
			inString += (char)inChar;
			// if you get a newline, print the string, then the string's value:
			if (inChar == '\n')
				break;
		}
		numberBatchIterations = inString.toInt();
		LogSerial.println("STARTING");
		LogSerial.print("Batches: ");
		LogSerial.println(N_BATCHES);
		LogSerial.print("Iterations in a Batch: ");
		LogSerial.println(numberBatchIterations);
		line();
		overheadBench();
		line();
		sumBench();
		line();
		subBench();
		line();
		multiplicationBench();
		line();
		divisionBench();
		line();
		atributionBench();
		line();
		remainderBench();
		line();
		andBench();
		line();
		orBench();
		line();
		expBench();
		line();
		logBench();
		line();
		sinBench();
		line();
		cosBench();
		line();
		tanBench();
		LogSerial.println("ENDED");
	}
}
