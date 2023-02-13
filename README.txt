This is an MPI program that approximates the value of pi via the
approximation of a definite integral. The integral is calculated
as the summation for a given n, and i is in the set [1, n]:

  (1.0 / n) * 4.0 / (1.0 + pow((1.0 / n) * (i - 0.5), 2))

For p processors, partial sums are calculated by each processor.
Each processor computes approximately n/p terms, and sums these
terms to get a partial sum. Finally, all the partial sums are
reduced to the final total sum via a standard parallel sum
reduction. Terms are allocated to each processor via cyclic
distribution of values of i, which greatly simplifies the logic
for handling values of n that are not evenly divisble by p.

Also note that the above formula is simply the most intuitive
expression of each term. The program rearranges calculations
to both minimize work per iteration, and to reduce floating
point error that can occur for large values of n.



Experiement:

The below are results of an experiment performed on the
Georgia Tech COC-ICE cluster. Each run of the program was
executed on a single node with a varying number of processors.
Experiements not shown here that used multiple nodes had far
worse results, presumably due to decreased locality and thus
increased latency.

p,  n,       s,              t
1,  1000000, 3.141594653589, 0.001549482346
2,  1000000, 3.141594653589, 0.000778675079
4,  1000000, 3.141594653589, 0.000411510468
6,  1000000, 3.141594653589, 0.000294685364
8,  1000000, 3.141594653589, 0.000225543976
10, 1000000, 3.141594653589, 0.000192880630
12, 1000000, 3.141594653589, 0.000189542770
14, 1000000, 3.141594653589, 0.000168323517
16, 1000000, 3.141594653589, 0.000163316727
18, 1000000, 3.141594653589, 0.000166654587
20, 1000000, 3.141594653589, 0.000153779984
22, 1000000, 3.141594653589, 0.000155210495
24, 1000000, 3.141594653589, 0.000158786774
