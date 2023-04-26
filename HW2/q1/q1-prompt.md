"""
Level 1 BLAS (Basic Linear Algebra Subprograms). Given the following specification, write a C++ function that computes $y=a*x+y$ where $x,y \in \mathbb{R^n}, \alpha \in \mathbb{R}$ and measures the performance for $n = 2$ to $n = 1024$. Let each $n$ be measured $ntrial$ times and plot the average performance for each case versus $n$, $ntrial \geq 3$ (performance is FLOPs=(floating point operations)/(total duration in seconds)). Use $ntrial=1000$ to avoid dividing by zero. You may initialize your problem with any non-zero values you desire (ie: random numbers). The correctness of your function will be tested against a test system with known result. Check for and flag incorrect cases. Submit C++ function file, main source file, and performance plot. Your function will be called with: 
`void daxpy (double a, const std::vector<double> &x, std::vector<double> &y);`
"""