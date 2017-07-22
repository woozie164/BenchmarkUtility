#include "BenchmarkUtility.h"
#include <iostream>
using namespace std;
using namespace chrono;

float foo( int x, float y )
{
	return x + y;
}

void V1(int iterations)
{	
	vector<float> cTimes( iterations );
	vector<float> cTimeStart( iterations );
	vector<float> cTimeEnd( iterations );

	vector<float> wTimes( iterations );
	vector<float> wTimeStart( iterations );
	vector<float> wTimeEnd( iterations );

	for (int i = 0; i < iterations; i++)
	{
		cTimes[i] = ((float)(cTimeEnd[i] - cTimeStart[i])) / CLOCKS_PER_SEC;
		wTimes[i] = duration<float>( wTimeEnd[i] - wTimeStart[i] ).count();
	}

}

void V2(int iterations)
{
	vector<float> cTimes( iterations );
	vector<float> cTimeStart( iterations );
	vector<float> cTimeEnd( iterations );

	for (int i = 0; i < iterations; i++)
	{
		cTimes[i] = ((float)(cTimeEnd[i] - cTimeStart[i])) / CLOCKS_PER_SEC;		
	}

	vector<float> wTimes( iterations );
	vector<float> wTimeStart( iterations );
	vector<float> wTimeEnd( iterations );

	for (int i = 0; i < iterations; i++)
	{
		wTimes[i] = duration<float>( wTimeEnd[i] - wTimeStart[i] ).count();
	}

}

void main()
{
	auto f = std::bind( foo, 1, 2.0f );

	BenchmarkResult result = benchmark( f );	
	BenchmarkResult result2 = benchmark( f, 10000 );

	int iterations = 1'000'000;
	BenchmarkResult result3 = benchmark( std::bind( V1, iterations ) );
	BenchmarkResult result4 = benchmark( std::bind( V2, iterations ) );

	cout << result3.avgCPUTime << endl;
	cout << result3.avgWallTime << endl;

	cout << result4.avgCPUTime << endl;
	cout << result4.avgWallTime << endl;

    BenchmarkResult result5 = benchmark( []() {
        cout << "hello world" << "\n";
    }, 1000 );

    BenchmarkResult result6 = benchmark( []() {
        cout << "hello world" << endl;
    }, 1000 );

    // Pretty print the results
    cout << result5 << endl;
    cout << result6 << endl;

	char c;
	cin >> c;
}
