#pragma once

#include <functional>
#include <chrono>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>


struct BenchmarkResult
{
	float minCPUTime;
	float maxCPUTime;
	float avgCPUTime; // geometric average
	float stdDeviationCPUTime;
	float varianceCPUTime;

	float minWallTime;
	float maxWallTime;
	float avgWallTime; // geometric average
	float stdDeviationWallTime;
	float varianceWallTime;
};

static 
std::ostream& operator <<(std::ostream& os, const BenchmarkResult& result)
{
    using namespace std;
    os << "-- CPU Time (nanoseconds) --"                        << endl;
    os << "Average: "            << result.avgCPUTime           << endl;
    os << "Min: "                << result.minCPUTime           << endl;
    os << "Max: "                << result.maxCPUTime           << endl;
    os << "Variance: "           << result.varianceCPUTime      << endl;
    os << "Standard deviation: " << result.stdDeviationCPUTime  << endl;

    os << "-- Wall Time (nanoseconds) --"                       << endl;
    os << "Average: "            << result.avgWallTime          << endl;
    os << "Min: "                << result.minWallTime          << endl;
    os << "Max: "                << result.maxWallTime          << endl;
    os << "Variance: "           << result.varianceWallTime     << endl;
    os << "Standard deviation: " << result.stdDeviationWallTime << endl;

    return os;
}

// Runs the function f multiple times and returns performance data.
// Usage: BenchmarkResult result = benchmark( std::bind(foo, 1, 2) );
static
BenchmarkResult benchmark( const std::function<void( void )>& f, int iterations = 10 )
{
	using namespace std;
	using namespace chrono;

	// Calculates CPU time, which may be smaller than actual time passed (wall time) if the CPU is shared by other processes.
	// CPU time may be larger than wall time if the function is multithreaded.
	vector<clock_t> cTimesStart( iterations );
	vector<clock_t> cTimesEnd( iterations );

	// Wall time, the actual amount of time that passed in real-life
	vector<time_point<steady_clock>> wTimesStart( iterations );
	vector<time_point<steady_clock>> wTimesEnd( iterations );


	for (int i = 0; i < iterations; i++)
	{
		cTimesStart[i] = clock();
		wTimesStart[i] = high_resolution_clock::now();

		f();

		cTimesEnd[i] = clock();
		wTimesEnd[i] = high_resolution_clock::now();
	}


	vector<float> cTimes( iterations );	
	for (int i = 0; i < iterations; i++)
	{
		cTimes[i] = ((float)(cTimesEnd[i] - cTimesStart[i])) / CLOCKS_PER_SEC;		
	}

	vector<float> wTimes( iterations );
	for (int i = 0; i < iterations; i++)
	{
		wTimes[i] = duration<float>( wTimesEnd[i] - wTimesStart[i] ).count();
	}

	// helper functions
	auto arithmeticMean = []( auto start, auto end, int count )
	{
		float mean = 0;

		for (auto it = start; it != end; ++it)
		{
			mean += *it;
		}

		return mean / count;
	};
	auto geometricMean = []( auto start, auto end )
	{
		auto it = start;

		float geomAvg = *it;
		++it;

		size_t i = 1;
		for (; it != end; ++it)
		{
			geomAvg *= *it;
			i++;
		}

		return pow( geomAvg, 1.0f / i );
	};
	auto variance = []( auto start, auto end, float mean )
	{
		vector<float> temp;

		for (auto it = start; it != end; ++it)
		{
			float x = *it - mean;
			temp.push_back( x * x );
		}

		float sum = 0;
		for (auto it = start; it != end; ++it)
		{
			sum += *it;
		}
		float variance = sum / temp.size();

		return variance;
	};

	BenchmarkResult result;
	result.minCPUTime = *min_element( cTimes.begin(), cTimes.end() );
	result.maxCPUTime = *max_element( cTimes.begin(), cTimes.end() );
	result.avgCPUTime = geometricMean( cTimes.begin(), cTimes.end() );
	float meanCPUTime = arithmeticMean( cTimes.begin(), cTimes.end(), cTimes.size() );
	result.varianceCPUTime = variance( cTimes.begin(), cTimes.end(), meanCPUTime );
	result.stdDeviationCPUTime = sqrt( result.varianceCPUTime );

	result.minWallTime = *min_element( wTimes.begin(), wTimes.end() );
	result.maxWallTime = *max_element( wTimes.begin(), wTimes.end() );
	result.avgWallTime = geometricMean( wTimes.begin(), wTimes.end() );
	float meanWallTime = arithmeticMean( cTimes.begin(), cTimes.end(), cTimes.size() );
	result.varianceWallTime = variance( cTimes.begin(), cTimes.end(), meanWallTime );
	result.stdDeviationWallTime = sqrt( result.varianceWallTime );

	return result;
}