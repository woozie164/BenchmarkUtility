# BenchmarkUtility

## What is it
A function that runs a piece of code multiple times and measures performance (min, max, avg, variance and standard deviation). 

## How to use it

### Alternative 1
```C++
auto f = std::bind( foo, 1, 2.0f );
BenchmarkResult result2 = benchmark( f, 10000 ); // Run function f 10000 times
```

### Alternative 2
Useful when you want to benchmark a piece of code inside a big function, and feel don't like extracting it to a seperate function. 
```C++
BenchmarkResult result5 = benchmark( []() {
	cout << "hello world" << "\n";
}, 1000 );
```