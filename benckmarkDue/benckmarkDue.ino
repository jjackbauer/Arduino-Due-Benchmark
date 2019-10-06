/*
			--Arduino Benchmark --
			By: Ricardo Medeiros 
			Github: jjackbauer 
*/
HardwareSerial *LogSerial = &SerialUSB; // Serial for Programming USB Port, SerialUSB for Native USB Port

#define N_ITERATIONS 20			 // has to be 20 for the used T student
#define N_BATCH_ITERATIONS 10000 // the bigger the better, but it will take more time to finish

void setup()
{
	LogSerial->begin(115200);
}
bool entrou = false;
unsigned long Start, End;
long long int c = 0, c1;
double A = 0, B = 1, C = 1;
double uTime[20], uMeanTime, standardDeviation, HV, LV, overheadMeanTime;
double studentT19 = 2.0930; // Retirado do livro Modelagem e Simulação Discreta do professor Brauliro, 2016.

void benchStatistics()
{
	for (c = 0; c < N_ITERATIONS; c++)
	{
		uTime[c] -= overheadMeanTime;
		uTime[c] = fabs(uTime[c]);
	}

	uMeanTime /= (N_ITERATIONS);
	uMeanTime -= overheadMeanTime;
	uMeanTime = fabs(uMeanTime);

	standardDeviation = 0;
	for (c = 0; c < N_ITERATIONS; c++)
	{
		LogSerial->print("Iteration ");
		LogSerial->print((int)c);
		LogSerial->print(" : ");
		LogSerial->print(uTime[c], 16);
		LogSerial->println(" us");
		standardDeviation += pow((uTime[c] - uMeanTime), 2);
	}
	standardDeviation /= (N_ITERATIONS - 1);
	standardDeviation = sqrt(standardDeviation);
	HV = uMeanTime + studentT19 * standardDeviation / sqrt(N_ITERATIONS);
	LV = uMeanTime - studentT19 * standardDeviation / sqrt(N_ITERATIONS);
	LogSerial->print("Mean:");
	LogSerial->print(uMeanTime, 8);
	LogSerial->println(" us");
	LogSerial->print("Standard Deviation:");
	LogSerial->print(standardDeviation, 8);
	LogSerial->println(" us^2");
	LogSerial->print("Confiance Interval: [");
	LogSerial->print(LV, 8);
	LogSerial->print(" , ");
	LogSerial->print(HV, 8);
	LogSerial->println("] us");
}
void overheadBench()
{
	LogSerial->println("<Overhead>");
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
	benchStatistics();
}
void divisionBench()
{
	LogSerial->println("<Division>");
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
	LogSerial->println("<Multiplication>");
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
	LogSerial->println("<Sum>");
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
	LogSerial->println("<Subtraction>");
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
	LogSerial->println("<Atribuition>");
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

void greeting()
{
	LogSerial->println("<Ricardo Medeiros's Benchmark for Arduino Due>");
	LogSerial->println("<send 's' to start Benchmark>");
}
void loop()
{

	if (LogSerial && !entrou)
	{
		greeting();
		entrou = true;
	}

	char msg = LogSerial->read();

	if (msg == 's')
	{
		LogSerial->println("STARTING");
		overheadBench();
		sumBench();
		subBench();
		multiplicationBench();
		divisionBench();
		atributionBench();
		LogSerial->println("ENDED");
	}
}