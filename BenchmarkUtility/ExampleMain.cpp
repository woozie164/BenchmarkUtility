#include "BenchmarkUtility.h"
#include <iostream>
using namespace std;
using namespace chrono;

float foo( int x, float y )
{
	return x + y;
}

void main()
{
	auto f = std::bind( foo, 1, 2.0f );

	BenchmarkResult result = benchmark( f );	
	BenchmarkResult result2 = benchmark( f, 10'000 );

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
